#include <iostream>

//NOTE: Just for glm::to_string(...)
#define GLM_ENABLE_EXPERIMENTAL

#include "entt/entt.hpp"
#include "glm/ext.hpp" //NOTE: Just for glm::to_string(...)
#include "Transform.h"

using std::cout;
using std::endl;

using namespace entt;
using namespace carlos;

struct Velocity {
    vec2 velocity;
};


void update(registry& registry, float dt) {
    //TODO: Figure out the data types here.
    //I think I was close?
    //  basic_view<get_t<Position, Velocity>, exclude_t<>>
    auto view = registry.view<Transform, Velocity>();
    
    //NOTE: The 1st parameter is optional
    view.each([](const entity e, const Transform& p, const Velocity& v) {
        
    });

    for (auto [entity, pos, vel] : view.each()) {

    }

    for (entity e : view) {
        Transform& t = view.get<Transform>(e);
        Velocity& v = view.get<Velocity>(e);

        vec2 localPos = t.getLocalPosition();
        t.setLocalPosition(localPos + v.velocity * dt);
    }
}

void test(registry& registry) {
    auto view = registry.view<Transform, Velocity>();

    for (entity e : view) {
        Transform& t = view.get<Transform>(e);
        //NOTE: https://stackoverflow.com/questions/11515469/how-do-i-print-vector-values-of-type-glmvec3-that-have-been-passed-by-referenc
        cout << "pos = (" << glm::to_string(t.getLocalPosition()) << ")" << endl;
    }
    cout << endl;
}

void testECS() {
    registry registry;

    for (int i = 0; i < 3; i++) {
        entity e = registry.create();
        //NOTE: According to https://skypjack.github.io/entt/md_docs_md_entity.html
        //  We don't even need to write constructors for our component types.
        //  Instead, they say:
        //      The default storage detects aggregate types internally and exploits aggregate initialization when possible.
        //      Therefore, it's not strictly necessary to define a constructor for each type.
        
        //TODO: Compiler errors.. perhaps related to:
        //  "The component must have a proper constructor or be of aggregate type"
        // registry.emplace<Transform>(e, 0, 0);

        registry.emplace<Transform>(e);

        if (i % 2 == 0) {
            // registry.emplace<Velocity>(e, i, (float) i / 2);
            Velocity& v = registry.emplace<Velocity>(e);
            v.velocity = vec2(
                i,
                (float) i / 2
            );
        }
    }

    for (int i = 0; i < 10; i++) {
        update(registry, 0.1f);
        test(registry);
    }
}
