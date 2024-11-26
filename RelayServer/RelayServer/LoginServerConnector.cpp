#include "LoginServerConnector.h"

void LoginServerConnector::runLoginServerConnector(const char* ipAddress, const u_short port)
{
	runConnector("127.0.0.1", LOGIN_SERVER_PORT);
}
