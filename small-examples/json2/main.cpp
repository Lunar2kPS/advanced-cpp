#include <iostream>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

using std::cout;
using std::endl;

using namespace rapidjson;

int main() {
    cout << "Json Example 2" << endl;

    const char* jsonText =
R"({
    "value": 0,
    "value2": 42.3,
    "value3": 17.0382,
    "name": "Hi!"
}
)";

    Document doc = { };
    doc.SetObject();

    Document::AllocatorType& allocator = doc.GetAllocator();
    doc.AddMember("value", 0, allocator);
    doc.AddMember("value2", 0.1f, allocator);
    doc.AddMember("value3", 17.0382, allocator);
    doc.AddMember("name", "Hi there!", allocator);

    Value addressObj = Value(Type::kObjectType);
    addressObj.AddMember("city", "Sun Lakes", allocator);
    addressObj.AddMember("state", "AZ", allocator);
    addressObj.AddMember("zipCode", 85248, allocator);
    doc.AddMember("niceWeather", addressObj, allocator);

    Value hobbies = Value(Type::kArrayType);
    hobbies.PushBack("Game Development", allocator);
    hobbies.PushBack("Gymnastics", allocator);
    hobbies.PushBack("Palm Tree Gardening", allocator);
    hobbies.PushBack("Walking", allocator);
    hobbies.PushBack("Music Composition", allocator);
    doc.AddMember("hobbies", hobbies, allocator);

    StringBuffer buffer = { };
    // Writer<StringBuffer> writer = Writer<StringBuffer>(buffer);
    PrettyWriter<StringBuffer> writer = PrettyWriter<StringBuffer>(buffer);
    writer.SetMaxDecimalPlaces(5);
    doc.Accept(writer);

    const char* output = buffer.GetString();
    cout << output << endl;

    buffer.Clear();
    doc.Parse(jsonText);
    Value& value = doc["name"];
    value.SetString("Hi again!");
    doc.Accept(writer);

    output = buffer.GetString();
    cout << output << endl;
    return 0;
}
