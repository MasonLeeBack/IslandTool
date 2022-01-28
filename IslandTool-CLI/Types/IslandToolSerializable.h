/*

IslandTool
Copyright (c) 2022 Mason Lee Back

File name: IslandToolSerializable.h

*/

#ifndef _ISLANDTOOLSERIALIZABLE_H_
#define _ISLANDTOOLSERIALIZABLE_H_

// IslandToolSerializable classes allow for serializing
// to 2 types of data streams: standard library streams,
// and TinyXML streams.
class IslandToolSerializable {
public:
    void Serialize(std::ofstream* stream);
    void Serialize(tinyxml2::XMLElement* element);
    void Deserialize(std::ifstream* stream);
    void Deserialize(tinyxml2::XMLElement* element);

    // After serializing the data, you can call
    // back to the original position of this item.
    std::streampos GetPosition() { return position; };

protected:
    std::streampos position = -1;
    std::streampos updatePosition = -1;
    void PreUpdate(std::ofstream* stream);
    void PostUpdate(std::ofstream* stream);
};

#endif // _ISLANDTOOLSERIALIZABLE_H_
