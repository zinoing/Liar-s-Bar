#include "PacketBuffer.h"

PacketBuffer::PacketBuffer() : readOffset(0), writeOffset(0) {
    memset(buffer, 0, DATA_BUF_SIZE);
}

PacketBuffer::PacketBuffer(const char* packet) : readOffset(0), writeOffset(0) {
    memcpy(buffer, packet, DATA_BUF_SIZE);
}

PacketBuffer::~PacketBuffer() {

}

int PacketBuffer::getReadOffset() const {
    return readOffset;
}

int PacketBuffer::getWriteOffset() const {
    return writeOffset;
}

const char* PacketBuffer::getData() const
{
    return buffer;
}

void PacketBuffer::setWriteOffset(int val)
{
    writeOffset = val;
    memcpy(buffer, &writeOffset, sizeof(int));
    return;
}

void PacketBuffer::writeType(ClientPacketType type)
{
    if (writeOffset + sizeof(type) > DATA_BUF_SIZE) {
        throw out_of_range("Buffer overflow");
    }
    memcpy(&buffer[writeOffset], &type, sizeof(type));
    writeOffset += sizeof(type);
}

void PacketBuffer::writeType(ServerPacketType type)
{
    if (writeOffset + sizeof(type) > DATA_BUF_SIZE) {
        throw out_of_range("Buffer overflow");
    }
    memcpy(&buffer[writeOffset], &type, sizeof(type));
    writeOffset += sizeof(type);
}

void PacketBuffer::writeChar(char value) {
    if (writeOffset + sizeof(value) > DATA_BUF_SIZE) {
        throw out_of_range("Buffer overflow");
    }
    buffer[writeOffset] = value;
    writeOffset += sizeof(value);
}

void PacketBuffer::writeInt(int value) {
    if (writeOffset + sizeof(value) > DATA_BUF_SIZE) {
        throw out_of_range("Buffer overflow");
    }
    memcpy(&buffer[writeOffset], &value, sizeof(value));
    writeOffset += sizeof(value);
}

void PacketBuffer::writeFloat(float value) {
    if (writeOffset + sizeof(value) > DATA_BUF_SIZE) {
        throw out_of_range("Buffer overflow");
    }
    memcpy(&buffer[writeOffset], &value, sizeof(value));
    writeOffset += sizeof(value);
}

void PacketBuffer::writeString(const string& value) {
    int strSize = value.size();
    if (writeOffset + sizeof(strSize) + strSize > DATA_BUF_SIZE) {
        throw out_of_range("Buffer overflow");
    }
    memcpy(&buffer[writeOffset], &strSize, sizeof(strSize));
    writeOffset += sizeof(strSize);
    memcpy(&buffer[writeOffset], value.c_str(), strSize);
    writeOffset += strSize;
}

void PacketBuffer::writeBuffer(const PacketBuffer& packetBuffer)
{
    int bufferwriteOffset = packetBuffer.getWriteOffset();
    if (writeOffset + bufferwriteOffset > DATA_BUF_SIZE) {
        throw out_of_range("Buffer overflow");
    }

    memcpy(&buffer[writeOffset], packetBuffer.getData(), bufferwriteOffset);
    writeOffset += bufferwriteOffset;
}

char PacketBuffer::readChar() {
    if (readOffset + sizeof(char) > DATA_BUF_SIZE) {
        throw out_of_range("Buffer underflow");
    }
    char value = buffer[readOffset];
    readOffset += sizeof(char);
    return value;
}

int PacketBuffer::readInt() {
    if (readOffset + sizeof(int) > DATA_BUF_SIZE) {
        throw out_of_range("Buffer underflow");
    }
    int value;
    memcpy(&value, &buffer[readOffset], sizeof(value));
    readOffset += sizeof(int);
    return value;
}

float PacketBuffer::readFloat() {
    if (readOffset + sizeof(float) > DATA_BUF_SIZE) {
        throw out_of_range("Buffer underflow");
    }
    float value;
    memcpy(&value, &buffer[readOffset], sizeof(value));
    readOffset += sizeof(float);
    return value;
}

string PacketBuffer::readString() {
    if (readOffset + sizeof(int) > DATA_BUF_SIZE) {
        throw out_of_range("Buffer underflow");
    }
    int strSize = 0;
    memcpy(&strSize, &buffer[readOffset], sizeof(strSize));
    readOffset += sizeof(strSize);

    if (readOffset + strSize > DATA_BUF_SIZE) {
        throw out_of_range("Buffer underflow");
    }

    string value(buffer + readOffset, strSize);
    readOffset += strSize;
    return value;
}