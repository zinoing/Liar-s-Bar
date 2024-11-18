#pragma once

enum class ClientPacketType {
    REQ_LOG_IN = 1,
    REQ_REGISTER,
    REQ_HOST_ROOM,
    REQ_ENTER_ROOM,
};

enum class ServerPacketType {
    ACCEPT_LOG_IN = 1,
    REJECT_LOG_IN,

    ACCEPT_REGISTER,
    REJECT_REGISTER,
    
    ALLOW_HOST_ROOM,
    REJECT_HOST_ROOM,

    ALLOW_ENTER_ROOM,
    REJECT_ENTER_ROOM,
};