#include <iostream>
#include <fstream>

#include "nlohmann/json.hpp"
#include "glm/glm.hpp"

using std::cout;
using std::endl;
using std::getline;

using std::ifstream;

using json = nlohmann::json;
using glm::vec3;

namespace nlohmann {
    template <>
    struct adl_serializer<vec3> {
        static void to_json(json& j, const vec3& value) {
            j = json({
                {"x", value.x},
                {"y", value.y},
                {"z", value.z}
            });
        }
        
        static void from_json(const json& j, vec3& value) {
            j.at("x").get_to(value.x);
            j.at("y").get_to(value.y);
            j.at("z").get_to(value.z);
        }
    };
}

void testJSON() {
    cout << "JSON testing..." << endl;
    
    vec3 position = vec3(2, -9, 6);
    json jPosition = position; //ERROR: no suitable user-defined conversion from "glm::vec3" to "json" existsC/C++(312)

    cout << jPosition.dump(4) << endl;
    cout << endl;
}
