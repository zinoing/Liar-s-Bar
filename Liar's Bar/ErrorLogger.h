#pragma once

#include "Stdfax.h"

#define WRONG_PACKET_TYPE 9999

class ErrorLogger {
public:
    static void logAndExit(const string& context, int errorCode, SOCKET sock = INVALID_SOCKET) {

        cerr << "[" << context << "] Error: " << errorCode
            << " (" << getErrorMessage(errorCode) << ")\n";

        if (sock != INVALID_SOCKET) {
            closesocket(sock);
            std::cerr << "[" << context << "] Socket closed.\n";
        }

        WSACleanup();

        exit(-1);
    }

    static void log(const string& context, int errorCode) {
        cerr << "[" << context << "] Error: " << errorCode
            << " (" << getErrorMessage(errorCode) << ")\n";
    }

private:
    static string getErrorMessage(int errorCode) {
        // handle custom error code
        switch (errorCode) {
        case WRONG_PACKET_TYPE:
            return "Invalid packet type received.";
        default:
            break;
        }

        // handle system error code
        char* errorMsg = nullptr;
        FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPSTR)&errorMsg, 0, NULL);

        string message = errorMsg ? errorMsg : "Unknown error";
        LocalFree(errorMsg);
        return message;
    }
};

