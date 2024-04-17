#include <iostream>
#include <iomanip>
#include <string>
#include <codecvt>
#include <math.h>
#include <climits>

using std::hex;
using std::dec;
using std::setfill;
using std::setw;

using std::cout;
using std::wcout;
using std::endl;
using std::string;
using std::wstring;
using stringconverter = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>;

int main() {
    stringconverter converter = { };
    string fromUTF8 = "Carlos";
    wstring toUTF16 = converter.from_bytes(fromUTF8);

    cout << "CHAR_MIN, CHAR_MAX = " << CHAR_MIN << ", " << CHAR_MAX << endl;
    cout << "WCHAR_MIN, WCHAR_MAX = " << WCHAR_MIN << ", " << WCHAR_MAX << endl;

    cout << "\n";
    cout << "(char*)    " << fromUTF8 << ": length() = " << fromUTF8.length() << "\n        ";
    cout << hex;
    for (int i = 0; i < fromUTF8.length(); i++) {
        char current = fromUTF8[i];
        cout << "0x" << (int) current << " ";
    }
    cout << dec;

    // size_t sizeFactor = (WCHAR_MAX - WCHAR_MIN + 1) / (CHAR_MAX - CHAR_MIN + 1);
    // sizeFactor = logf(sizeFactor) / logf(16);
    // cout << "sizeFactor (of the size of a wchar_t)"

    cout << "\n";
    wcout << L"(wchar_t*) " << toUTF16 << L": length() = " << toUTF16.length() << L"\n        ";
    cout << setfill('0') << hex;
    for (int i = 0; i < toUTF16.length(); i++) {
        wchar_t currentWide = toUTF16[i];
        for (int inner = 0; inner < sizeof(wchar_t); inner++) {
            wchar_t current = currentWide & (wchar_t) (0xFF << inner * CHAR_BIT);
            cout << "0x" << setw(2) << current << " ";
        }
    }
    cout << endl;
    cout << setfill(' ') << dec;

    return 0;
}
