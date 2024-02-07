#include <iostream>
#include <string>

#include "simdjson/simdjson.h"

using std::cout;
using std::endl;
using std::string;

using namespace simdjson;
using namespace simdjson::ondemand;

int main() {
    string someJSON = R"(
{
    "value": 49.3,
    "value2": 0,
    "name": "hi"
}
)";

    padded_string someJSONPadded = padded_string(someJSON);

    parser parser;
    
    document doc = parser.iterate(someJSONPadded);
    size_t count = doc.count_elements();


    return 0;
}
