#pragma once

#include <iostream>
#include <string>
#include <google/protobuf/message.h>  // protobuf 메시지 관련 헤더

// Protocol Buffers로 생성된 클래스 헤더 포함
#include "your_proto_file.pb.h"

class PacketManager {
public:
    // 패킷을 직렬화하는 함수
    static std::string SerializePacket(const google::protobuf::Message& message);

    // 패킷을 역직렬화하는 함수
    static bool DeserializePacket(const std::string& data, google::protobuf::Message& message);
};