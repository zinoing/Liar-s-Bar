// RelayServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <winsock2.h>
#include "IOCPServer.h"

using namespace std;

int main()
{
	IOCPServer server;

	server.init();
	server.bindAndListen();
	server.acceptConnection();
}