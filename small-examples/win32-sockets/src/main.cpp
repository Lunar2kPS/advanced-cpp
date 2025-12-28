#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h> //Windows Sockets TCP/IP header. Contains inet_ntop + inet_pton helper functions to convert between Network and Presentation representation (binary and string, respectively) of IP addresses.

using std::cout;
using std::cerr;
using std::endl;

bool tryGetCLIArg(int argCount, char** args, const char* name);

int main(int argCount, char** args) {
    bool isClient = true; //NOTE: This program is a client by default.
    if (tryGetCLIArg(argCount, args, "--server"))
        isClient = false;
    bool isServer = !isClient;

    cout << "Win32 Sockets Example Program!" << (isClient ? " (CLIENT)" : "") << (isServer ? " (SERVER)" : "") << endl;
    for (int i = 0; i < argCount; i++) {
        cout << "    args[" << i << "] = " << args[i] << "\n";
    }
    cout << endl;

    int delayedExitCode = 0;

    //STEP 1: Initialize WSA
    //NOTE: The current WSA (Windows Sockets API) version is 2.2 and the current DLL is Ws2_32.dll.
    //  See: https://learn.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-wsastartup
    WSADATA data = { };
    int errorCode = WSAStartup(MAKEWORD(2, 2), &data);
    if (errorCode != 0) {
        cerr << "Failed to load WSA (Windows Sockets API): Error " << errorCode << "." << endl;
        return 1;
    }

    //According to their API (see URL above),
    //  If we request 2.2 and it loads successfully, it WILL still return v2.2 even if a newer version is supported.
    //NOTE: For a clear example, let's say we wanted to use WSA v2.0.
    //  In that case, we'd use:
    //      MAKEWORD(2, 0), and thus:
    //      LOBYTE(data.wVersion) will be equivalent to 2.
    //      HIBYTE(data.wVersion) will be equivalent to 0.
    if (LOBYTE(data.wVersion) != 2 || HIBYTE(data.wVersion) != 2) {
        cerr << "Failed to load exactly v2.2 of WSA (Windows Sockets API)!" << endl;
        goto End;
    }
    cout << "Successfully loaded WSA (Windows Sockets API) v" << (int) LOBYTE(data.wVersion)
        << "." << (int) HIBYTE(data.wVersion) << "." << endl;

    if (isServer) {
        //STEP 2 on SERVER: Create a Socket
        SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (server == INVALID_SOCKET) {
            cerr << "Failed to create valid server listening socket. Error code: " << WSAGetLastError() << "." << endl;
            delayedExitCode = 502;
            goto End;
        }
        cout << "Successfully created the server listening socket." << endl;

        //STEP 3 on SERVER: Bind the server socket
        //  or equivalently, you may check if bind(...) == SOCKET_ERROR (-1)
        //SOCKADDR_IN is used for IPv4 (AF_INET).
        //"sin" prefix just stands for "socket internet".

        //NOTE: Endianness determines how a machine orders a single piece of data (ONE VALUE, NOT AN ARRAY) that's spread over multiple bytes, such as (integers) a 16bit short, 32bit int, or a 64bit long, as we're used to in Java/C#.
        //  Big Endian:     MSB comes first (at "lowest storage address"). HUMAN READABLE ORDER.    MSB = Most Significant Byte.    (NETWORK BYTE ORDER)
        //  Little Endian:  LSB comes first (at "lowest storage address"). CPU-FRIENDLY ORDER.      LSB = Least Significant Byte.
        //  htons(...) = Host TO Network SHORT (16bit integers)
        //  htonl(...) = Host TO Network LONG  (32bit integers)
        //See: https://en.wikipedia.org/wiki/Endianness
        sockaddr_in serverInfo = { };
        serverInfo.sin_family = AF_INET;
        serverInfo.sin_port = htons(8085); //NOTE: Must be in network byte order (big-endian), so use htons (Host TO Network SHORT).
        serverInfo.sin_addr.s_addr = inet_addr("127.0.0.1");
        //NOTE: Technically not needed, since we already zero-assigned with = { }.
        //memset(&serverInfo.sin_zero, 0, 8 * sizeof(CHAR)); //This is just padding so sockaddr_in can be the same size as the older sockaddr.

        if (bind(server, (sockaddr*) &serverInfo, sizeof(serverInfo)) != 0) {
            cerr << "Failed to bind the server listening socket. Error code: " << WSAGetLastError() << "." << endl;
            delayedExitCode = 503;
            goto End;
        }
        cout << "Successfully bound the server listening socket to 127.0.0.1:8085!" << endl;

        //STEP 4 on SERVER: Listen on the server socket
        //NOTE: backlog = 1, meaning we only allow listening for up to 1 client at a time.
        if (listen(server, 1) != 0) {
            cerr << "Failed to begin listening on the server socket. Error code: " << WSAGetLastError() << "." << endl;
            delayedExitCode = 504;
            goto End;
        }
        cout << "Successfully began listening on the server socket!" << endl;

        //STEP 5 on SERVER: Accept a new client
        //NOTE: This is a BLOCKING API CALL.
        //  Also, we can use the following:
        //  getsockname(...)    Gets local address/port of a socket.
        //  getpeername(...)    Gets remote address/port of a socket.
        sockaddr_in clientInfo = { };
        int clientInfoLength = sizeof(clientInfo);
        SOCKET client = accept(server, (sockaddr*) &clientInfo, &clientInfoLength); //NOTE: We pass null because we don't have any filter for what client address (or etc.) we want to accept.
        if (client == INVALID_SOCKET) {
            cerr << "Failed to accept a client on the server socket. Error code: " << WSAGetLastError() << "." << endl;
            delayedExitCode = 505;
            goto End;
        }

        char ipString[INET_ADDRSTRLEN]; //NOTE: This requires WS2tcpip.h
        inet_ntop(AF_INET, &(clientInfo.sin_addr), ipString, INET_ADDRSTRLEN); //Internal Network (binary) to Presentation format

        cout << "Successfully accepted a client from " << ipString << ":" << (int) ntohs(clientInfo.sin_port) << "!" << endl;

        //STEP 6 on SERVER: Send and receive data
        //  (vaguely. Is it the same as the client below?)

        //STEP 7 on SERVER: Close the socket
        //  or equivalently, you may check if closesocket(...) == SOCKET_ERROR (-1)
        if (closesocket(server) != 0) {
            cerr << "Failed to gracefully close server listening socket. Error code: " << WSAGetLastError() << "." << endl;
            delayedExitCode = 507;
            goto End;
        }
        cout << "Successfully closed the server listening socket." << endl;
    } else if (isClient) {
        //STEP 2 on CLIENT: Create a Socket
        SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (client == INVALID_SOCKET) {
            cerr << "Failed to create valid client socket. Error code: " << WSAGetLastError() << "." << endl;
            delayedExitCode = 402;
            goto End;
        }
        cout << "Successfully created the client socket." << endl;

        //WARNING: connect(...) will wait 75 seconds by default! (for the server to respond)

        //STEP 3 on CLIENT: Connect to the server (Bind happens automatically. Local binding auto-finds a local port, remote binding uses the IP/port we're trying to connect to for the server).
        sockaddr_in serverInfo = { };
        serverInfo.sin_family = AF_INET;
        serverInfo.sin_port = htons(8085);
        serverInfo.sin_addr.s_addr = inet_addr("127.0.0.1");
        if (connect(client, (sockaddr*) &serverInfo, sizeof(serverInfo)) != 0) {
            cerr << "Failed to connect to the server. Error code: " << WSAGetLastError() << "." << endl;
            delayedExitCode = 403;
            goto End;
        }
        cout << "Successfully connected to the server at 127.0.0.1:8085!" << endl;

        //STEP 4 on CLIENT: Send and receive data
        //  (recv, send, recvfrom, sendto)

        //STEP 5 on CLIENT: Close the socket
        if (closesocket(client) != 0) {
            cerr << "Failed to gracefully close client socket. Error code: " << WSAGetLastError() << "." << endl;
            delayedExitCode = 405;
            goto End;
        }
        cout << "Successfully closed the client socket." << endl;
    }


    //STEP N: Uninitialize WSA
    End:
    errorCode = WSACleanup();
    if (errorCode != 0) {
        cerr << "Failed to unload WSA (Windows Sockets API): Error " << errorCode << "." << endl;
        if (delayedExitCode == 0)
            return 2;
    }

    return delayedExitCode;
}

bool tryGetCLIArg(int argCount, char** args, const char* name) {
    for (int i = 0; i < argCount; i++)
        if (strcmp(args[i], name) == 0)
            return true;
    return false;
}
