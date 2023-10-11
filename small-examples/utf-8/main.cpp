#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

#if defined(_WIN32)
    #include <Windows.h>
    #pragma execution_character_set("utf-8")
    using std::locale;
#endif

using std::cout;
using std::endl;
using std::cin;
using std::setfill;
using std::setw;
using std::getline;
using std::ios;
using std::ifstream;
using std::ofstream;
using std::string;
using std::dec;
using std::hex;
using std::uppercase;

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

    //Additional tests:
    cout << "\"u\" with two dots on top: \xc3\xbc\n";
    cout << "chinese glyph for \"world\": \xe5\x80\xbc\n";
    cout << "smiling emoji: \xf0\x9f\x98\x80\n";
    cout.flush();

    ifstream file = { };
    file.open("../test.txt", ios::binary);
    if (file.is_open()) {
        string fileLine;
        while (getline(file, fileLine)) {
            cout << fileLine << '\n';
        }
        file.close();
        cout.flush();
    }

    ofstream outFile("../test-output.txt", ios::binary, ios::trunc);
    outFile.imbue(utf8);
    outFile << "This is some\nText in a file\nwith a couple lines." << endl;
    outFile.close();


    string testInput;
    getline(cin, testInput);
    cout << "You entered:\n" << testInput << endl;

    const char* inputStr = testInput.c_str();
    cout << hex << uppercase << setw(2) << setfill('0');
    for (int i = 0; i < testInput.length(); i++)
        cout << (int) inputStr[i] << " ";
    cout << dec << endl;

    const char* stringA =       "This is a test.";
    const char* stringB =       "This is🤔a test.";
    
    //WARNING: Suffers from double-UTF-8 encoding for some reason.
    //      https://stackoverflow.com/a/59114748
    //      so, might as well not use the u8 prefix,
    //      and just set the program compilation to use utf-8 flags,
    //      and set the runtime/program to use UTF-8 as well.
    const char* stringC =     u8"This is🤔a test.";

    const char* a = "你好";
    cout << a << " (strlen is " << strlen(a) << endl; // Prints 6

    cout << strlen(stringA) << " vs. " << strlen(stringB) << " vs. " << strlen(stringC) << endl;
    cout << stringA << endl;
    cout << stringB << endl;
    cout << stringC << endl;

    cout << endl;

    const char* ch = "€";
    cout << "Character: " << ch << endl;

    // Using u8 prefix for a string
    const char* str = "Hello, 世界!";
    cout << "String: " << str << endl;
    printf(str);
    return 0;
}
