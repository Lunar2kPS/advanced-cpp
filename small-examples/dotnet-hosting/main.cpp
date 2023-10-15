#include "basicnethosting.h"

int
#if defined(WINDOWS)
    wmain
#else
    main
#endif
    (int argCount, char_t** args) {
    string_t path = carlos::getCurrentDirectory(argCount, args);
    carlos::runManagedCode(path);
}
