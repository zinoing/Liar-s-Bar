#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <cstring> 
#pragma once

#define DATA_BUF_SIZE 4096

#include "PacketType.h"

using namespace std;

class PacketBuffer {
private:
    char buffer[DATA_BUF_SIZE];
    int offset = 0;

public:
    PacketBuffer();
    PacketBuffer(const char* packet);
    ~PacketBuffer();

    int size() const;
    int currentOffset() const;

    const char* getData() const;

    // write functions
    void writeType(ClientPacketType type);
    void writeChar(char value);
    void writeInt(int value);
    void writeFloat(float value);
    void writeString(const string& value);
    void writeBuffer(const PacketBuffer& packetBuffer);

    // read functions
    ClientPacketType readType();
    char readChar();
    int readInt();
    float readFloat();
    string readString();
};