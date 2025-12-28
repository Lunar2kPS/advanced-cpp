#include <iostream>
#include <WinSock2.h>

using std::cout;
using std::cerr;
using std::endl;

bool getCLIArg(int argCount, char** args, const char* name);
int main(int argCount, char** args) {
    cout << "Win32 Sockets Example Program!" << endl;

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

    End:
    errorCode = WSACleanup();
    if (errorCode != 0) {
        cerr << "Failed to unload WSA (Windows Sockets API): Error " << errorCode << "." << endl;
        return 2;
    }

    return 0;
}
