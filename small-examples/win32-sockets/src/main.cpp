#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h> //Windows Sockets TCP/IP header. Contains inet_ntop + inet_pton helper functions to convert between Network and Presentation representation (binary and string, respectively) of IP addresses.

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::flush;

bool tryGetCLIArg(int argCount, char** args, const char* name);

//Tangential Notes -- OSI Model (Open Systems Interconnection) contains the following 7 Layers:
//  Application     (your code)
//  Presentation    (serialization, buffers)
//  Session         (client/server connection)
//  Transport       (protocol such as TCP, UDP)
//  Network         (IP address)
//  Data Link       (MAC address)
//  Physical        (ethernet/Wifi and hardware)

//COMMAND LINE USAGE:
//  ./Win32SocketsExample.exe [--server] [--udp]
//  The program defaults to being a CLIENT, and using TCP. However, you may mix and match, and use the SERVER program with the --server option, and in either case, change it to use UDP with the --udp option.
int main(int argCount, char** args) {
    bool isClient = true; //NOTE: This program is a client by default.
    if (tryGetCLIArg(argCount, args, "--server"))
        isClient = false;
    bool isServer = !isClient;

    bool useTCP = true; //NOTE: This program uses TCP by default.
    if (tryGetCLIArg(argCount, args, "--udp"))
        useTCP = false;
    bool useUDP = !useTCP;

    cout << "Win32 Sockets Example Program!" << (isClient ? " (CLIENT)" : "") << (isServer ? " (SERVER)" : "") << (useTCP ? " (TCP)" : "") << (useUDP ? " (UDP)" : "") << endl;
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
        SOCKET server = useTCP ? socket(AF_INET, SOCK_STREAM, IPPROTO_TCP) : useUDP ? socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP) : INVALID_SOCKET;
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

        //STEP 4 on SERVER: Listen on the server socket (TCP ONLY)
        //NOTE: backlog = 1, meaning we only allow listening for up to 1 client at a time.
        if (useTCP) {
            if (listen(server, 1) != 0) {
                cerr << "Failed to begin listening on the server socket. Error code: " << WSAGetLastError() << "." << endl;
                delayedExitCode = 504;
                goto End;
            }
            cout << "Successfully began listening on the server socket!" << endl;

            //STEP 5 on SERVER: Accept a new client (TCP ONLY)
            //NOTE: This is a BLOCKING API CALL.
            //  Also, we can use the following:
            //  getsockname(...)    Gets local address/port of a socket.
            //  getpeername(...)    Gets remote address/port of a socket.
            sockaddr_in clientInfo;
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

            //STEP 6 on SERVER: Send and receive data (TCP version)
            //NOTE: TCP ONLY ALLOWS sending data when a connection between a client AND server is established -- so we CANNOT send TCP data over the "server" socket, ONLY on the duplicated, connected "client" socket here on the SERVER.
            const int MESSAGE_BUFFER_SIZE = 16; //NOTE: This means if the buffer size is 16, the max message length (strlen) is actually 15, due to the required NUL terminator as the 16th character. This is consistent with cin.getline(...) behavior.
            char message[MESSAGE_BUFFER_SIZE] = { 0 };
            cout << "[SERVER] Enter your TCP message: " << flush;
            cin.getline(message, MESSAGE_BUFFER_SIZE);
            int messageLength = strlen(message);
            cout << "You entered " << messageLength << " / " << (MESSAGE_BUFFER_SIZE - 1) << " characters." << endl;

            //NOTE: I noticed the client was able to successfully closesocket(...) from their end, and the SERVER here called send(...), but OUR side still thought it was successful and returned 16 (16 bytes sent).
            //  I later tested the same way, but waiting significantly (minutes) longer before sending the data, and THEN we (the SERVER) were able to get a 10053 WSA error (WSAECONNABORTED (See: https://learn.microsoft.com/en-us/windows/win32/winsock/windows-sockets-error-codes-2)).
            //NOTE: strlen(...) is the length of the string WITHOUT the NUL terminator, so +1 here to ensure we send a NUL terminator. cin.getline(...) will always ensure a NUL is at the last char of our message buffer if it WOULD have overflowed from long user input, so we're GOOD!
            int bytesSent = send(client, message, messageLength + 1, 0);
            if (bytesSent == SOCKET_ERROR) {
                int wsaError = WSAGetLastError();
                if (wsaError == WSAECONNABORTED) {
                    cout << "The client likely closed the connection from their end already." << endl;
                } else {
                    cerr << "Failed to send data to client at " << ipString << ":" << (int) ntohs(clientInfo.sin_port) << ": Error code: " << wsaError << "." << endl;
                }
                goto ServerEnd;
            }
            cout << "Sent " << bytesSent << " bytes to client at " << ipString << ":" << (int) ntohs(clientInfo.sin_port) << "." << endl;
        } else if (useUDP) {
            //STEP 6 on SERVER: Send and receive data (UDP version)
            const int MESSAGE_BUFFER_SIZE = 16;
            char message[MESSAGE_BUFFER_SIZE] = { 0 };
            sockaddr_in clientInfo;
            int clientInfoLength = sizeof(clientInfo);
            int bytesReceived = recvfrom(server, message, MESSAGE_BUFFER_SIZE, 0, (sockaddr*) &clientInfo, &clientInfoLength);
            if (bytesReceived == SOCKET_ERROR) {
                cerr << "Failed to receive data from the client. Error code: " << WSAGetLastError() << "." << endl;
                goto ServerEnd;
            }

            char ipString[INET_ADDRSTRLEN]; //NOTE: This requires WS2tcpip.h
            inet_ntop(AF_INET, &(clientInfo.sin_addr), ipString, INET_ADDRSTRLEN); //Internal Network (binary) to Presentation format
            cout << "Successfully received a message from a client at " << ipString << ":" << (int) ntohs(clientInfo.sin_port) << " containing " << bytesReceived << " bytes!\n    ";
            cout.write(message, bytesReceived);
            cout << endl;
        }

        //STEP 7 on SERVER: Close the socket
        //  or equivalently, you may check if closesocket(...) == SOCKET_ERROR (-1)
        ServerEnd:
        if (closesocket(server) != 0) {
            cerr << "Failed to gracefully close server listening socket. Error code: " << WSAGetLastError() << "." << endl;
            delayedExitCode = 507;
            goto End;
        }
        cout << "Successfully closed the server listening socket." << endl;
    } else if (isClient) {
        //STEP 2 on CLIENT: Create a Socket
        SOCKET client = useTCP ? socket(AF_INET, SOCK_STREAM, IPPROTO_TCP) : useUDP ? socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP) : INVALID_SOCKET;
        if (client == INVALID_SOCKET) {
            cerr << "Failed to create valid client socket. Error code: " << WSAGetLastError() << "." << endl;
            delayedExitCode = 402;
            goto End;
        }
        cout << "Successfully created the client socket." << endl;

        //STEP 3 on CLIENT: Connect to the server (TCP ONLY) Bind happens automatically. Local binding auto-finds a local port, remote binding uses the IP/port we're trying to connect to for the server).
        //WARNING: connect(...) will wait 75 seconds by default! (for the server to respond)
        sockaddr_in serverInfo = { };
        serverInfo.sin_family = AF_INET;
        serverInfo.sin_port = htons(8085);
        serverInfo.sin_addr.s_addr = inet_addr("127.0.0.1");
        if (useTCP) {
            if (connect(client, (sockaddr*) &serverInfo, sizeof(serverInfo)) != 0) {
                cerr << "Failed to connect to the server. Error code: " << WSAGetLastError() << "." << endl;
                delayedExitCode = 403;
                goto End;
            }
            cout << "Successfully connected to the server at 127.0.0.1:8085!" << endl;

            sockaddr_in clientInfo;
            int clientInfoLength = sizeof(clientInfo);
            getsockname(client, (sockaddr*) &clientInfo, &clientInfoLength);

            char ipString[INET_ADDRSTRLEN]; //NOTE: This requires WS2tcpip.h
            inet_ntop(AF_INET, &(clientInfo.sin_addr), ipString, INET_ADDRSTRLEN);
            cout << "The client automatically bound to " << ipString << ":" << (int) ntohs(clientInfo.sin_port) << "!" << endl;
        }

        //STEP 4 on CLIENT: Send and receive data (TCP version)
        //  (recv, send, recvfrom, sendto)
        if (useTCP) {
            const int MESSAGE_BUFFER_SIZE = 16;
            char message[MESSAGE_BUFFER_SIZE] = { 0 };
            int bytesReceived = recv(client, message, MESSAGE_BUFFER_SIZE, 0);
            if (bytesReceived == SOCKET_ERROR) {
                cerr << "Failed to receive data from the server. Error code: " << WSAGetLastError() << "." << endl;
                goto ClientEnd;
            }
            cout << "Successfully received a message from the server at 127.0.0.1:8085 containing " << bytesReceived << " bytes!\n    ";
            cout.write(message, bytesReceived);
            cout << endl;
        } else if (useUDP) {
            const int MESSAGE_BUFFER_SIZE = 16; //NOTE: This means if the buffer size is 16, the max message length (strlen) is actually 15, due to the required NUL terminator as the 16th character. This is consistent with cin.getline(...) behavior.
            char message[MESSAGE_BUFFER_SIZE] = { 0 };
            cout << "[CLIENT] Enter your UDP message: " << flush;
            cin.getline(message, MESSAGE_BUFFER_SIZE);
            int messageLength = strlen(message);
            cout << "You entered " << messageLength << " / " << (MESSAGE_BUFFER_SIZE - 1) << " characters." << endl;

            int bytesSent = sendto(client, message, messageLength + 1, 0, (sockaddr*) &serverInfo, sizeof(serverInfo));
            if (bytesSent == SOCKET_ERROR) {
                int wsaError = WSAGetLastError();
                if (wsaError == WSAECONNABORTED) {
                    cout << "The server likely closed the connection from their end already." << endl;
                } else {
                    cerr << "Failed to send data to server at 127.0.0.1:8085: Error code: " << wsaError << "." << endl;
                }
                goto ClientEnd;
            }
            sockaddr_in clientInfo;
            int clientInfoLength = sizeof(clientInfo);
            getsockname(client, (sockaddr*) &clientInfo, &clientInfoLength);

            char ipString[INET_ADDRSTRLEN]; //NOTE: This requires WS2tcpip.h
            inet_ntop(AF_INET, &(clientInfo.sin_addr), ipString, INET_ADDRSTRLEN);
            cout << "The client automatically bound to " << ipString << ":" << (int) ntohs(clientInfo.sin_port) << "!" << endl;
            cout << "Sent " << bytesSent << " bytes to server at 127.0.0.1:8085." << endl;
        }

        ClientEnd:
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
