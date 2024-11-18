#include "PacketBuffer.h"

PacketBuffer::PacketBuffer() : offset(0) {
    memset(buffer, 0, DATA_BUF_SIZE);
}

PacketBuffer::PacketBuffer(const char* packet) : offset(0) {
    memcpy(buffer, packet, DATA_BUF_SIZE);
}

PacketBuffer::~PacketBuffer() {

}

int PacketBuffer::size() const {
    return offset;
}

int PacketBuffer::currentOffset() const {
    return offset;
}

const char* PacketBuffer::getData() const
{
    return buffer;
}

void PacketBuffer::writeType(ClientPacketType type)
{
    if (offset + sizeof(type) > DATA_BUF_SIZE) {
        throw out_of_range("Buffer overflow");
    }
    memcpy(&buffer[offset], &type, sizeof(type));
    offset += sizeof(type);
}

void PacketBuffer::writeChar(char value) {
    if (offset + sizeof(value) > DATA_BUF_SIZE) {
        throw out_of_range("Buffer overflow");
    }
    buffer[offset] = value;
    offset += sizeof(value);
}

void PacketBuffer::writeInt(int value) {
    if (offset + sizeof(value) > DATA_BUF_SIZE) {
        throw out_of_range("Buffer overflow");
    }
    memcpy(&buffer[offset], &value, sizeof(value));
    offset += sizeof(value);
}

void PacketBuffer::writeFloat(float value) {
    if (offset + sizeof(value) > DATA_BUF_SIZE) {
        throw out_of_range("Buffer overflow");
    }
    memcpy(&buffer[offset], &value, sizeof(value));
    offset += sizeof(value);
}

void PacketBuffer::writeString(const string& value) {
    int strSize = value.size();
    if (offset + sizeof(strSize) + strSize > DATA_BUF_SIZE) {
        throw out_of_range("Buffer overflow");
    }
    memcpy(&buffer[offset], &strSize, sizeof(strSize));
    offset += sizeof(strSize);
    memcpy(&buffer[offset], value.c_str(), strSize);
    offset += strSize;
}

void PacketBuffer::writeBuffer(const PacketBuffer& packetBuffer)
{
    int bufferOffset = packetBuffer.currentOffset();
    if (offset + bufferOffset > DATA_BUF_SIZE) {
        throw out_of_range("Buffer overflow");
    }

    memcpy(&buffer[offset], packetBuffer.getData(), bufferOffset);
    offset += bufferOffset;
}

ClientPacketType PacketBuffer::readType()
{
    if (offset + sizeof(int) > DATA_BUF_SIZE) {
        throw out_of_range("Buffer underflow");
    }
    int value;
    memcpy(&value, &buffer[offset], sizeof(value));
    offset += sizeof(int);
    return (ClientPacketType)value;
}

char PacketBuffer::readChar() {
    if (offset + sizeof(char) > DATA_BUF_SIZE) {
        throw out_of_range("Buffer underflow");
    }
    char value = buffer[offset];
    offset += sizeof(char);
    return value;
}

int PacketBuffer::readInt() {
    if (offset + sizeof(int) > DATA_BUF_SIZE) {
        throw out_of_range("Buffer underflow");
    }
    int value;
    memcpy(&value, &buffer[offset], sizeof(value));
    offset += sizeof(int);
    return value;
}

float PacketBuffer::readFloat() {
    if (offset + sizeof(float) > DATA_BUF_SIZE) {
        throw out_of_range("Buffer underflow");
    }
    float value;
    memcpy(&value, &buffer[offset], sizeof(value));
    offset += sizeof(float);
    return value;
}

string PacketBuffer::readString() {
    if (offset + sizeof(int) > DATA_BUF_SIZE) {
        throw out_of_range("Buffer underflow");
    }
    int strSize = 0;
    memcpy(&strSize, &buffer[offset], sizeof(strSize));
    offset += sizeof(strSize);

    if (offset + strSize > DATA_BUF_SIZE) {
        throw out_of_range("Buffer underflow");
    }

    string value(buffer + offset, strSize);
    offset += strSize;
    return value;
}