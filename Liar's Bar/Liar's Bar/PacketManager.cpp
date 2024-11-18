#include "PacketManager.h"
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

std::string PacketManager::SerializePacket(const google::protobuf::Message& message) {
    // 직렬화된 데이터를 저장할 문자열
    std::string serialized_data;

    // 데이터 크기만큼 버퍼를 할당
    if (message.SerializeToString(&serialized_data)) {
        return serialized_data;
    }
    else {
        std::cerr << "Failed to serialize message!" << std::endl;
        return "";
    }
}

bool PacketManager::DeserializePacket(const std::string& data, google::protobuf::Message& message) {
    // 역직렬화할 때 데이터를 입력으로 사용
    return message.ParseFromString(data);
}
