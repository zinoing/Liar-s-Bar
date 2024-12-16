#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <cstring> 
#pragma once

#define DATA_BUF_SIZE 4096

#include "PacketType.h"
#include "Card.h"

using namespace std;

class PacketBuffer {
private:
    char buffer[DATA_BUF_SIZE];
    int readOffset = 0;
    int writeOffset = 0;

public:
    PacketBuffer();
    PacketBuffer(const char* packet);
    ~PacketBuffer();

    int getReadOffset() const;
    int getWriteOffset() const;
    const char* getData() const;

    void setWriteOffset(int val);

    // write functions
    void writeType(ClientPacketType type);
    void writeType(ServerPacketType type);
    void writeChar(char value);
    void writeInt(int value);
    void writeFloat(float value);
    void writeString(const string& value);
    void writeBuffer(const PacketBuffer& packetBuffer);
    void writeCards(const vector<Card>& cards);

    // read functions
    char readChar();
    int readInt();
    float readFloat();
    string readString();
    vector<Card> readCards();
};