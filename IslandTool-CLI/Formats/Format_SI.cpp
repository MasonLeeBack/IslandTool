/*

IslandTool
Copyright (c) 2022 Mason Lee Back

File name: Format_SI.cpp

*/

#include "stdafx.h"

using namespace tinyxml2;

MxObject* objects;

MxObject DeweaveObject(std::ifstream* stream, int indent, bool child = false);

// This function decodes any MxOb objects in lists, and
// returns an array of the objects.
void HandleList(MxObject* object, std::ifstream* stream, int offset)
{
    // Verify we're handling a list!
    MxList list = {};
    stream->read((char*)&list, sizeof(MxList));

    if (list.Signature != MXLIST_TAG) {
        printf("%8sERROR: Not a valid list! Got: %x at address 0x%X\n", "", list.Signature, (int)stream->tellg());
        return;
    }

    printf("%*sChildren count: %lu\n", 4 + offset, "", list.NumberOfObjects);

    object->Children = new MxObject[list.NumberOfObjects];
    object->ChildrenCount = list.NumberOfObjects;
    for (unsigned int i = 0; i < list.NumberOfObjects; i++) {
        object->Children[i] = DeweaveObject(stream, 8 + offset, true);
    }
}

MxObject DeweaveObject(std::ifstream* stream, int indent, bool child)
{
    MxObject mxObject = {};

    printf("%*s0x%X:\n", indent, "", (int)stream->tellg());

    mxObject.Offset = (int)stream->tellg();

    stream->read((char*)&mxObject.Signature, sizeof(uint32_t));
    stream->read((char*)&mxObject.ObjectSize, sizeof(uint32_t));
    stream->read((char*)&mxObject.ObjectType, sizeof(uint16_t));

    // If this isn't a child object, it's not priority
    // on the list!
    if (child == false) {
        mxObject.Weaved = true;
    }

    if (mxObject.Signature != MXOBJECT_TAG) {
        printf("Mx Object header does not match!\n");
        return mxObject;
    }

    char* tempString = GetNextString(stream);

    // Check to see if we have a ClassHandler
    for (int i = 0; i < ClassListSize; i++) {
        if (!strcmp(tempString, classesList[i]->GetClassName()))
        {
            mxObject.HandlerClass = tempString;
        }
    }

    if (mxObject.HandlerClass == NULL) {
        mxObject.Name = tempString;
    }
    else {
        mxObject.Name = GetNextString(stream);
    }

    printf("    %*sType: %s\n", indent, "", ObjectDefToString(mxObject.ObjectType));
    printf("    %*sName: %s\n", indent, "", mxObject.Name);

    if (mxObject.HandlerClass != NULL) {
        printf("    %*sHandler class: %s\n", indent, "", mxObject.HandlerClass);
    }

    stream->read((char*)&mxObject.StreamID, sizeof(uint32_t));

    printf("    %*sStream ID: %lu\n", indent, "", mxObject.StreamID);

    stream->read((char*)&mxObject.Flags, sizeof(uint32_t));

    if (mxObject.Flags & MX_FLAG_LOOP_STREAM) {
        printf("    %*sLoopMethod: STREAM\n", indent, "");
    }

    if (mxObject.Flags & MX_FLAG_LOOP_CACHE) {
        printf("    %*sLoopMethod: CACHE\n", indent, "");
    }

    if (mxObject.Flags & MX_FLAG_TRANSPARENT) {
        printf("    %*sTransparency: TRUE\n", indent, "");
    }

    stream->read((char*)&mxObject.Unknown, sizeof(uint32_t));

    stream->read((char*)&mxObject.Duration, sizeof(uint32_t));
    stream->read((char*)&mxObject.Loops, sizeof(uint32_t));

    if (mxObject.Duration == MX_DURATION_INDEFINITE) {
        printf("    %*sDuration: INDEFINITE\n", indent, "");
    }

    if (mxObject.Loops != 1) {
        printf("    %*sLoops: %lu\n", indent, "", mxObject.Loops);
    }

    // Coordinate Data
    mxObject.Position.Deserialize(stream);
    mxObject.Direction.Deserialize(stream);
    mxObject.Up.Deserialize(stream);

    // Display position, direction, and Up value if not their default values
    if (mxObject.Position != Vector3{ 0,0,0 }) {
        printf("    %*sPosition: { %f, %f, %f }\n", indent, "", mxObject.Position.X, mxObject.Position.Y, mxObject.Position.Z);
    }

    if (mxObject.Direction != Vector3{ 0,0,1 }) {
        printf("    %*sDirection: { %f, %f, %f }\n", indent, "", mxObject.Direction.X, mxObject.Direction.Y, mxObject.Direction.Z);
    }

    if (mxObject.Up != Vector3{ 0,1,0 }) {
        printf("    %*sUp: { %f, %f, %f }\n", indent, "", mxObject.Up.X, mxObject.Up.Y, mxObject.Up.Z);
    }

    stream->read((char*)&mxObject.ExtraData_Size, sizeof(uint16_t));

    // ExtraData is defined when the size is not NULL.
    if (mxObject.ExtraData_Size != 0) {
        mxObject.ExtraData = GetNextString(stream);

        printf("    %*sExtra data: %s\n", indent, "", mxObject.ExtraData);
    }

    // Object types of Parallel and Linear do not support filenames,
    // and any objects that don't reference files directly use the "inline"
    // identifier, with the actual entry referenced in the extra data.
    if (mxObject.ObjectType != ObjectDef::Parallel && mxObject.ObjectType != ObjectDef::Linear) {
        mxObject.FileName = GetNextString(stream);
        printf("    %*sFile name: %s\n", indent, "", mxObject.FileName);

        // Extra data after file name
        stream->read((char*)&mxObject.Unknown2, sizeof(uint32_t) * 3);

        stream->read((char*)&mxObject.FileFormat, sizeof(uint32_t));

        stream->read((char*)&mxObject.Unknown3, sizeof(uint32_t) * 2);


        // WAV files require extra data, TODO: figure this out
        if (mxObject.FileFormat == 'VAW ') {
            stream->read((char*)&mxObject.ExtraData_2, sizeof(uint32_t));
        }
    }
    else {
        // Process children
        HandleList(&mxObject, stream, indent);
    }

    // Assign to objects list
    objects[mxObject.StreamID] = mxObject;

    // Seek to end of object to avoid padding
    FixPadding(stream);

    return mxObject;
}

void WriteHeader(std::ofstream* stream, uint32_t fileFormat, bool secondPass, const char* fileName = nullptr) {
    // Audio (WAV)
    if (fileFormat == 'VAW ') {
        if (secondPass == true) {
            stream->seekp(0, std::ios::end);
            unsigned int end = (unsigned int)stream->tellp();

            RIFF riffHeader;
            riffHeader.Format = 'EVAW';
            SubchunkHeader subchunk = { ' tmf', 16 };

            stream->seekp(0, std::ios::beg);
            riffHeader.Serialize(stream);
            stream->write((char*)&subchunk, sizeof(SubchunkHeader));

            // Move subchunk 1 data left to add subchunk 2 identifier
            char* subchunk1Data = new char[16];

            std::ifstream is(fileName, std::ios::binary);
            is.seekg(24, std::ios::beg);
            is.read(subchunk1Data, 16);
            is.close();

            stream->write(subchunk1Data, 16);

            // Add subchunk 2 identifier
            stream->write("data", 4);
        }
        else {
            const char* padding = "";
            stream->write(padding, 24);
        }
    }
    // Bitmaps (STL)
    if (fileFormat == 'LTS ') {
        if (secondPass == true) {
            stream->seekp(0, std::ios::end);

            uint16_t Signature = 'MB';
            uint32_t FileSize = (uint32_t)stream->tellp();
            uint32_t Reserved = 0x00000000;
            uint32_t Offset = 54;

            stream->seekp(0, std::ios::beg);

            stream->write((char*)&Signature, sizeof(uint16_t));
            stream->write((char*)&FileSize, sizeof(uint32_t));
            stream->write((char*)&Reserved, sizeof(uint32_t));
            stream->write((char*)&Offset, sizeof(uint32_t));
        }
        else {
            const char* padding = "";
            stream->write(padding, 14);
        }
    }
}

struct ExtractOperation {
    std::ofstream stream;
    const char* fileName;
    uint32_t streamID;
    bool finished = false;
};
void ExtractObject(ExtractOperation* operations, std::vector<MxObject> objects, std::ifstream* stream)
{
    for (unsigned int j = 0; j < objects.size(); j++) {
        if (objects[j].ObjectType == ObjectDef::Parallel || objects[j].ObjectType == ObjectDef::Linear) {
            operations[j].finished = true;
            break;
        }
        if (!strcmp("inline", objects[j].FileName)) {
            operations[j].finished = true;
            break;
        }

        operations[j].fileName = strrchr(objects[j].FileName, '\\') + 1;
        operations[j].stream = std::ofstream(operations[j].fileName, std::ios::binary);
        operations[j].streamID = objects[j].StreamID;

        WriteHeader(&operations[j].stream, objects[j].FileFormat, false);
    }

    SubchunkHeader listHeader = {};
    stream->read((char*)&listHeader, sizeof(SubchunkHeader));

    printf("Deweaving %lu objects (%lu bytes)\n", objects.size(), listHeader.ChunkSize);

    // Pass the MxDa object (not a valid subchunk)
    uint32_t* test;
    stream->read((char*)&test, sizeof(uint32_t));

    if ((int)stream->tellg() % 2 != 0) {
        stream->seekg(1, std::ios::cur);
    }

    while (true) {
        // Check all Extraction operations
        int finishedOperations = 0;
        for (unsigned int j = 0; j < objects.size(); j++) {
            if (operations[j].finished == true) {
                finishedOperations++;

                // Write the file header if necessary
                WriteHeader(&operations[j].stream, objects[j].FileFormat, true, operations[j].fileName);

                operations[j].stream.close();
            }
        }
        printf("finished: %lu, total %lu\n",finishedOperations, objects.size());
        if (finishedOperations == objects.size()) {
            break;
        }

        MxChunk chunk = {};
        stream->read((char*)&chunk, sizeof(MxChunk));
       
        if (chunk.Signature != MXCHUNK_TAG) {
            if (chunk.Signature == ' dap') {
                stream->seekg(chunk.ChunkSize - 14, std::ios::cur);
                continue;
            }
            else {
                printf("ERROR: Not a valid chunk!!! 0x%X at address 0x%X\n", chunk.Signature, (unsigned int)stream->tellg() - sizeof(MxChunk));
            }

            break;
        }

        printf("0x%X stream: %lu, time: %ld, size: %lu\n", (unsigned int)stream->tellg(), chunk.StreamID, chunk.Milliseconds, chunk.ChunkSize);

        // Store chunk data
        char* chunkData = new char[chunk.ChunkSize - 14];
        stream->read(chunkData, chunk.ChunkSize - 14);

        // Check stream IDs
        for (unsigned int j = 0; j < objects.size(); j++) {
            if (operations[j].streamID == chunk.StreamID) {
                if (chunk.Flags & MX_FLAG_CHUNK_ENDING) {
                    printf("streamID %lu finished\n", operations[j].streamID);
                    operations[j].finished = true;
                }

                operations[j].stream.write(chunkData, chunk.ChunkSize - 14);
            }
        }

        // If the chunk ends on an odd number, move up one to set to an even number.
        if ((int)stream->tellg() % 2 != 0) {
            stream->seekg(1, std::ios_base::cur);
        }

        delete[] chunkData;
    }
}

void Deweave_SI(std::ifstream* stream, char* outDestination)
{
    RIFF riffHeader;
    riffHeader.Deserialize(stream);

    // Verify RIFF header, and RIFF subtype
    if (riffHeader.Identifier != RIFF_IDENTIFIER) {
        printf("Not a RIFF file!\n");
        return;
    }
    if (riffHeader.Format != 'INMO') {
        printf("RIFF is not an OMNI subtype!\n");
        return;
    }

    MxHeader mxHeader = {};

    stream->read((char*)&mxHeader, sizeof(MxHeader));

    // Verify MX header
    if (mxHeader.Signature != MXHEADER_TAG) {
        printf("Mx header does not match!\n");
        return;
    }

    // Double check header size. ChunkSize doesn't account for the first 8 bytes
    if (mxHeader.ChunkSize + 8 != sizeof(MxHeader)) {
        printf("MX header size mismatch! Expected %lu, got %lu\n", sizeof(MxHeader), mxHeader.ChunkSize);
        return;
    }

    if (mxHeader.MajorVersion != SI_VERSION_MAJOR || mxHeader.MinorVersion != SI_VERSION_MINOR) {
        printf("SI version mismatch! Expected %u.%u, got %u.%u", SI_VERSION_MAJOR, SI_VERSION_MINOR, mxHeader.MajorVersion, mxHeader.MinorVersion);
        return;
    }

    printf("SI version %u.%u\n", mxHeader.MajorVersion, mxHeader.MinorVersion);

    printf("Buffer size: %lu bytes\n", mxHeader.BufferSize);
    printf("Number of buffers: %lu\n", mxHeader.BufferCount);

    // Offsets
    MxOffsets mxOffsets = {};

    stream->read((char*)&mxOffsets, sizeof(MxOffsets));

    // Verify MX header
    if (mxOffsets.Signature != MXOFFSETS_TAG) {
        printf("Mx Offsets header does not match!\n");
        return;
    }

    // Calculate number of chunk offsets, and store them
    unsigned int OffsetsCount = (mxOffsets.ChunkSize - 4) / 4;
    uint32_t* offsetList = new uint32_t[OffsetsCount];

    stream->read((char*)offsetList, sizeof(uint32_t) * OffsetsCount);

    printf("Object count: %lu\n", OffsetsCount);

    objects = new MxObject[mxOffsets.NumOfObjects];

    // Stream offsets have blank spots in them for anything that is not weaved
    printf("Stream offsets:\n");
    for (unsigned int i = 0; i < OffsetsCount; i++) {
        if (offsetList[i] != 0) {
            MxStream mxStream = {};
            stream->seekg(offsetList[i], std::ios::beg);
            stream->read((char*)&mxStream, sizeof(MxStream));

            if (mxStream.Signature != MXSTREAM_TAG) {
                printf("Mx Stream header does not match!\n");
                return;
            }

            DeweaveObject(stream, 4);
        }
    }

    // Pass 1
    // Extract objects (Reference from root objects only!)
    for (unsigned int i = 0; i < mxOffsets.NumOfObjects; i++) {
        if (objects[i].Signature != MXOBJECT_TAG) {
            continue;
        }

        if (objects[i].Weaved == false) {
            continue;
        }

        printf("test %s\n",objects[i].Name);

        if (objects[i].ObjectType == ObjectDef::Parallel || objects[i].ObjectType == ObjectDef::Linear)
        {
            stream->seekg(objects[i].Offset + 8 + objects[i].ObjectSize);

            // We have to recursively check for children here since they're
            // added to the main streaming LIST.
            std::vector<MxObject> children;

            // Add recursive children to the object count
            for (unsigned int j = 0; j < objects[i].ChildrenCount; j++) {
                if (objects[i].Children[j].ChildrenCount != 0) {
                    for (unsigned int k = 0; k < objects[i].Children[j].ChildrenCount; k++) {
                        children.push_back(objects[i].Children[j].Children[k]);
                    }
                }
                children.push_back(objects[i].Children[j]);
            }

            ExtractOperation* operations = new ExtractOperation[children.size()];
            printf("%lu children\n", objects[i].ChildrenCount);
            printf("real children %lu\n", children.size());

            ExtractObject(operations, children, stream);
            
            delete[] operations;
        }
        else {
            stream->seekg(objects[i].Offset + 8 + objects[i].ObjectSize);

            FixPadding(stream);

            std::vector<MxObject> object;
            object.push_back(objects[i]);

            ExtractOperation* operations = new ExtractOperation[object.size()];
            ExtractObject(operations, object, stream);

            delete[] operations;
        }
    }

    // Save all data to an XML document
    XMLDocument doc;
    XMLNode* pRoot = doc.NewElement("Root");
    doc.InsertFirstChild(pRoot);

    // Write configuration to XML
    XMLNode* pConfiguration = doc.NewElement("Configuration");
    XMLElement* bufferSize = doc.NewElement("BufferSize");
    bufferSize->SetText(mxHeader.BufferSize);
    pConfiguration->InsertEndChild(bufferSize);
    XMLElement* numberBuffers = doc.NewElement("BufferCount");
    numberBuffers->SetText(mxHeader.BufferCount);
    pConfiguration->InsertEndChild(numberBuffers);
    pRoot->InsertEndChild(pConfiguration);

    // Pass 2
    // Iterate all valid objects and add to XML
    for (unsigned int i = 0; i < mxOffsets.NumOfObjects; i++) {
        if (objects[i].Signature != MXOBJECT_TAG) {
            continue;
        }

        XMLElement* pObject = doc.NewElement("Object");
        if (objects[i].Weaved == true) {
            pObject->SetAttribute("Weave", true);
        }

        XMLElement* type = doc.NewElement("Type");
        type->SetText(ObjectDefToString(objects[i].ObjectType));
        pObject->InsertEndChild(type);

        XMLElement* name = doc.NewElement("Name");
        name->SetText(objects[i].Name);
        pObject->InsertEndChild(name);

        printf("Writing %s to XML\n", objects[i].Name);

        if (objects[i].HandlerClass != NULL) {
            XMLElement* handlerClass = doc.NewElement("HandlerClass");
            handlerClass->SetText(objects[i].HandlerClass);
            pObject->InsertEndChild(handlerClass);
        }

        if (objects[i].Loops != 1) {
            XMLElement* loops = doc.NewElement("Loops");
            loops->SetText(objects[i].Loops);
            pObject->InsertEndChild(loops);
        }

        if (objects[i].ExtraData_Size != 0) {
            XMLElement* extraData = doc.NewElement("ExtraData");
            extraData->SetText(objects[i].ExtraData);
            pObject->InsertEndChild(extraData);
        }

        if (objects[i].ObjectType != ObjectDef::Linear && objects[i].ObjectType != ObjectDef::Parallel) {
            XMLElement* fileName = doc.NewElement("FileName");
            fileName->SetText(objects[i].FileName);
            pObject->InsertEndChild(fileName);
        }

        pRoot->InsertEndChild(pObject);
    }

    XMLError res = doc.SaveFile("TEST_SI.xml");
    
}

void AddPadding(std::ofstream* outStream, MxHeader header, int buffer)
{
    uint32_t paddingSize = 0;

    outStream->seekp(0, std::ios::end);
    uint32_t fileSize = ((uint32_t)outStream->tellp());

    // Calculate size of padding (remove padding header)
    paddingSize = (header.BufferSize - fileSize) - 8;

    // Write padding header
    SubchunkHeader padHeader = { ' dap', paddingSize };
    outStream->write((char*)&padHeader, sizeof(SubchunkHeader));

    FillCharacter(outStream, 0xCD, paddingSize);
}

void WriteObject(std::ofstream* outStream, XMLElement* object) {

}

void Weave_SI(std::ofstream* outStream, char* inputFile) {
    // Open XML document
    XMLDocument doc;
    XMLError eResult = doc.LoadFile(inputFile);
    XMLNode* root = doc.FirstChild();

    int currentBuffer = 0;

    // Write RIFF header (wait to input file size)
    RIFF riffHeader;
    riffHeader.Format = 'INMO';
    riffHeader.Serialize(outStream);

    // Write MX header
    MxHeader mxHeader = {};
    mxHeader.Signature = MXHEADER_TAG;
    mxHeader.ChunkSize = sizeof(MxHeader) - 8;
    mxHeader.MajorVersion = SI_VERSION_MAJOR;
    mxHeader.MinorVersion = SI_VERSION_MINOR;

    // Get configuration data from XML
    XMLElement* configuration = root->FirstChildElement("Configuration");
    if (configuration == nullptr) {
        printf("Failed to get Configuration data from XML\n");
        return;
    }

    configuration->FirstChildElement("BufferSize")->QueryIntText((int*)&mxHeader.BufferSize);
    configuration->FirstChildElement("BufferCount")->QueryIntText((int*)&mxHeader.BufferCount);

    outStream->write((char*)&mxHeader, sizeof(MxHeader));

    // Write MX offsets
    MxOffsets mxOffsets = {};
    mxOffsets.Signature = MXOFFSETS_TAG;

    std::vector<XMLElement*> weavedObjects;

    // Iterate though object count, find objects intended to be weaved, and add them to the offset list.
    for (XMLElement* child = root->FirstChildElement("Object"); child; child = child->NextSiblingElement("Object")) {
        if (child->BoolAttribute("Weave") == true) {
            weavedObjects.push_back(child);
        }

        mxOffsets.NumOfObjects++;
    }

    mxOffsets.ChunkSize = 4 + (weavedObjects.size() * 4);
    outStream->write((char*)&mxOffsets, sizeof(MxOffsets));

    for (unsigned int i = 0; i < weavedObjects.size(); i++) {
        uint32_t offsetPad = 0;
        outStream->write((char*)&offsetPad, sizeof(uint32_t));
    }

    // MxStreams LIST
    unsigned int streamListOffset = (int)outStream->tellp();

    SubchunkHeader streamListHeader = { 'TSIL', 0xFFFFFFFF };
    outStream->write((char*)&streamListHeader, sizeof(SubchunkHeader));

    uint32_t streamListIdentifier = 'tSxM';
    outStream->write((char*)&streamListIdentifier, sizeof(uint32_t));

    uint32_t streamIDs = 0;

    std::vector<uint32_t> streamOffsets;
    for (unsigned int i = 0; i < weavedObjects.size(); i++) {
        streamOffsets.push_back((uint32_t)outStream->tellp());

        Subchunk streamSubchunk;
        streamSubchunk.Identifier = 'tSxM';
        streamSubchunk.Serialize(outStream);

        // Start writing object
        uint32_t objectOffset = (uint32_t)outStream->tellp();
        MxObject object = {};

        object.Signature = 'bOxM';
        object.ObjectSize = 0xFFFFFFFF;
        object.ObjectType = StringToObjectDef(weavedObjects[i]->FirstChildElement("Type")->GetText());

        if (auto handler = weavedObjects[i]->FirstChildElement("HandlerClass")) {
            object.HandlerClass = _strdup(handler->GetText());
        }

        object.Name = _strdup(weavedObjects[i]->FirstChildElement("Name")->GetText());

        printf("Adding object: %s\n", object.Name);

        object.StreamID = streamIDs;
        streamIDs++;

        outStream->write((char*)&object.Signature, sizeof(uint32_t));
        outStream->write((char*)&object.ObjectSize, sizeof(uint32_t));
        outStream->write((char*)&object.ObjectType, sizeof(uint16_t));
        printf("%lu\n", (uint32_t)outStream->tellp());
        
        if (object.HandlerClass != NULL) {
            StoreName(outStream, object.HandlerClass);
        }
        StoreName(outStream, object.Name);

        uint32_t objectSize = (uint32_t)outStream->tellp() - objectOffset + 8;
        outStream->seekp(objectOffset + 4, std::ios::beg);
        outStream->write((char*)&objectSize, sizeof(uint32_t));

        // objectSubchunk.SubchunkSize = (uint32_t)outStream->tellp() - objectOffset + 8;
        // objectSubchunk.Serialize(outStream);

        streamSubchunk.SubchunkSize = (uint32_t)outStream->tellp() - streamOffsets.back() + 8;
        streamSubchunk.Serialize(outStream);

        outStream->seekp(0, std::ios::end);
    }

    // Add padding based on remainder of the buffer
    AddPadding(outStream, mxHeader, currentBuffer);

    // Write MxStream list size
    outStream->seekp(0, std::ios::end);
    int fileSize = ((int)outStream->tellp());

    int streamListSize = fileSize - streamListOffset + 8;

    outStream->seekp(streamListOffset + 4, std::ios::beg);
    outStream->write((char*)&streamListSize, sizeof(uint32_t));

    // Populate stream offsets
    outStream->seekp(44, std::ios::beg);
    outStream->write((char*)&streamOffsets.front(), sizeof(uint32_t) * streamOffsets.size());

    riffHeader.ChunkSize = fileSize;
    riffHeader.Serialize(outStream);
}
