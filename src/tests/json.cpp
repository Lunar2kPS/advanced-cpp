#include <iostream>
#include <fstream>
#include <string>

#include "nlohmann/json.hpp"
#include "json.h"

using std::cout;
using std::endl;
using std::getline;

using std::ifstream;
using std::string;

void testMethod() {
    cout << "JSON testing..." << endl;

    ifstream file;
    file.open("resources/Example.json");

    string line;
    if (file.is_open()) {
        while (file.good()) {
            getline(file, line);
            cout << line << "\n";
        }
        cout << endl;
    }
}
