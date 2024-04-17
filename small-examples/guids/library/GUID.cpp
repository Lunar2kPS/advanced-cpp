#include "GUID.h"
#include "platforms.h"

#if defined(WINDOWS)
    #include "rpc.h"
#endif

using std::cout;
using std::cerr;
using std::endl;

namespace carlos {
    extern "C" __declspec(dllexport) GUID __cdecl GUID::createNew() {
#if defined(WINDOWS)
        RPC_STATUS status;
        UUID id;
        status = UuidCreate(&id);
        if (status != RPC_S_OK) {
            cerr << "Failed to create UUID (Win32)." << endl;
            return GUID();
        }
        
        unsigned char* rawValues = nullptr;
        status = UuidToStringA(&id, &rawValues);
        if (status != RPC_S_OK) {
            cerr << "Failed to output UUID to string (Win32)." << endl;
            return GUID();
        }

        char values[37];
        for (int i = 0; i < 36; i++)
            values[i] = (char) rawValues[i];
        values[36] = 0;

        status = RpcStringFreeA(&rawValues);
        if (status != RPC_S_OK) {
            cerr << "Failed to free memory after UuidToStringA (Win32)!" << endl;
            return GUID();
        }

        return GUID(values);
#endif
    }

    extern "C" __declspec(dllexport) ostream& operator <<(ostream& output, const GUID& guid) {
        output << guid.characters;
        return output;
    }
}
