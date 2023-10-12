#include <iostream>
#include <sstream>

#if defined(_WIN32)
    #include <Windows.h>
    #pragma execution_character_set("utf-8")
    using std::locale;
#endif

using std::cout;
using std::endl;

int main(int argCount, char** args) {
    #if defined(_WIN32)
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
        setlocale(LC_ALL, "en_US.UTF-8");

        // Enable buffering to prevent VS from chopping up UTF-8 byte sequences
        setvbuf(stdout, nullptr, _IOFBF, 1000);

        locale utf8 = locale("en_US.UTF-8");
        locale::global(utf8); //https://en.cppreference.com/w/cpp/locale/locale/global
    #endif

    cout << u8"UTF-8 cout test: lΔφρυθ" << endl;
    cout << "UTF-8 cout test: lΔφρυθ" << endl;
    return 0;
}
