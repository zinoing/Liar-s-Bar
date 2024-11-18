#pragma once

#include <iostream>
#include <string>
#include <google/protobuf/message.h>  // protobuf �޽��� ���� ���

// Protocol Buffers�� ������ Ŭ���� ��� ����
#include "your_proto_file.pb.h"

class PacketManager {
public:
    // ��Ŷ�� ����ȭ�ϴ� �Լ�
    static std::string SerializePacket(const google::protobuf::Message& message);

    // ��Ŷ�� ������ȭ�ϴ� �Լ�
    static bool DeserializePacket(const std::string& data, google::protobuf::Message& message);
};