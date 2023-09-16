#include <iostream>
// #include <mono/jit/jit.h>
// #include <mono/metadata/assembly.h>

#include "tests/json.h"

using std::cout;
using std::endl;

// MonoDomain* rootDomain = nullptr;
// MonoDomain* appDomain = nullptr;

// bool initializeMono() {
//     // mono_set_assemblies_path("./mono/lib");
    
//     rootDomain = mono_jit_init("Custom Runtime");
//     if (rootDomain == nullptr)
//         return false;

//     return true;
// }

// void createAppDomain() {
//     appDomain = mono_domain_create_appdomain("Custom App Domain", nullptr);
//     mono_domain_set(appDomain, true);
// }

int main() {
    cout << "Hello world!" << endl;
    testMethod();

    // if (!initializeMono())
    //     return -1;
    
    // createAppDomain();

    // mono_jit_cleanup(rootDomain);
    // rootDomain = nullptr;
    // appDomain = nullptr;

    return 0;
}
