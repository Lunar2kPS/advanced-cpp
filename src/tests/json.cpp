#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "nlohmann/json.hpp"
#include "json.h"

using std::cout;
using std::endl;
using std::getline;

using std::ifstream;
using std::string;
using std::stringstream;

using json = nlohmann::json;

void testJSON() {
    cout << "JSON testing..." << endl;

    ifstream file = ifstream("resources/Example.json");
    json test = json::parse(file);

    // string line;
    // stringstream fileText;
    // if (file.is_open()) {
    //     while (file.good()) {
    //         getline(file, line);
    //         fileText << line << "\n";
    //     }
    // }

    // cout << "File text:\n" << fileText.str() << endl;
    
    cout << "json = " << test.dump(4) << endl;
    cout << "json[\"people\"][\"Carlos\"] = " << test["people"]["Carlos"] << endl;
}
