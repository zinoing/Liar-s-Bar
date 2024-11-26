#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#define DATA_BUF_SIZE 4096

#define RELAY_SERVER_PORT 8000
#define LOGIN_SERVER_PORT 8001

#define READ 2
#define WRITE 3

#include <iostream>
#include <winsock2.h>
#include <thread>
#include <vector>

using namespace std;