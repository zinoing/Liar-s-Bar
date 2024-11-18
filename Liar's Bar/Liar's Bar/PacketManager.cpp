#include "PacketManager.h"
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

std::string PacketManager::SerializePacket(const google::protobuf::Message& message) {
    // ����ȭ�� �����͸� ������ ���ڿ�
    std::string serialized_data;

    // ������ ũ�⸸ŭ ���۸� �Ҵ�
    if (message.SerializeToString(&serialized_data)) {
        return serialized_data;
    }
    else {
        std::cerr << "Failed to serialize message!" << std::endl;
        return "";
    }
}

bool PacketManager::DeserializePacket(const std::string& data, google::protobuf::Message& message) {
    // ������ȭ�� �� �����͸� �Է����� ���
    return message.ParseFromString(data);
}
