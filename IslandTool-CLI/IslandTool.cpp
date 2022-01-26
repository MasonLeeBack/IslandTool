/*

IslandTool
Copyright (c) 2022 Mason Lee Back

File name: IslandTool.cpp

*/

#include "stdafx.h"

void PrintUsage(char* filename)
{
    printf("IslandTool\n"
        "Copyright (c) 2022 Mason Lee Back\n"
        "Usage: %s <option(s)> SOURCE\n"
        "Options:\n"
        "\t-h,--help\t\t\tShows this message\n"
        "\t-u,--unpack\t\t\tUnpacks file\n"
        "\t-p,--pack\t\t\tPacks file from XML data.\n"
        "\t-d,--destination DESTINATION\tDestination output\n",
        filename
    );
}

int main(int argc, char* argv[])
{
    bool isUnpacking = false;
    bool isPacking = false;
    char* destination = nullptr;
    char* source = nullptr;

    if (argc == 1 || argc > 5) {
        PrintUsage(argv[0]);
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        char* arg = argv[i];
        if (!strcmp(arg, "-h") || !strcmp(arg, "--help")) {
            PrintUsage(argv[0]);
            return 1;
        }
        else if (!strcmp(arg, "-u") || !strcmp(arg, "--unpack")) {
            isUnpacking = true;
            if (isPacking == true) {
                printf("Cannot unpack and pack at the same time!\n");
                return -2;
            }
        }
        else if (!strcmp(arg, "-p") || !strcmp(arg, "--pack")) {
            isPacking = true;
            if (isUnpacking == true) {
                printf("Cannot unpack and pack at the same time!\n");
                return -2;
            }
        }
        else if (!strcmp(arg, "-d") || !strcmp(arg, "--destination")) {
            if (i + 1 < argc) {
                destination = argv[i++];
            }
            else {
                printf("Destination not specified!\n");
                return -1;
            }
        }
        else {
            source = argv[i];
        }
    }

    if (source == nullptr) {
        printf("Source not specified!\n");
        return -4;
    }

    if (isUnpacking == true) {
        std::ifstream is(source, std::ifstream::binary);

        // Right now, this program only supports .SI files.
        // Later on, it should support .WDB files as well!
        if (is) {
            Deweave_SI(&is, destination);
            is.close();
        }
        else {
            printf("Failed to open %s for reading\n", source);
            return -1;
        }
    }
    else if (isPacking == true) {
        std::ofstream is("TEST.SI", std::ofstream::binary);

        if (is) {
            Weave_SI(&is, source);
        }
        printf("Temporary error: Packing not ready!\n");
        return -5;
    }
    else{
        printf("Must specify -u or -p\n");
        return -3;
    }

    return 0;
}
