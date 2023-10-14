#include <iostream>
#include "nlohmann/json.hpp"

using std::cout;
using std::endl;
using nlohmann::json;

struct ExampleData {
    int x;
    int y;
    int z;
};

namespace nlohmann {
    template <>
    struct adl_serializer<ExampleData> {
        static void to_json(json& j, const ExampleData& value) {
            j = json({
                {"x", value.x},
                {"y", value.y},
                {"z", value.z}
            });
        }
        
        static void from_json(const json& j, ExampleData& value) {
            j.at("x").get_to(value.x);
            j.at("y").get_to(value.y);
            j.at("z").get_to(value.z);
        }
    };
}
    

int main() {
    cout << "Json Example!" << endl;

    ExampleData position = { 2, -9, 6 };
    json jPosition = position; //ERROR: no suitable user-defined conversion from "glm::ExampleData" to "json" existsC/C++(312)

    cout << jPosition.dump(4) << endl;
    cout << endl;

    return 0;
}
