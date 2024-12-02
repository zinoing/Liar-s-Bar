#include "LoginServerConnector.h"

void LoginServerConnector::runLoginServerConnector()
{
	runConnector("127.0.0.1", LOGIN_SERVER_PORT);
}
