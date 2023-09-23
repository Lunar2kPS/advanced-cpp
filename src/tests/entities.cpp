#include <iostream>

#include "entt/entt.hpp"

using std::cout;
using std::endl;

using namespace entt;

struct Position {
    float x;
    float y;
};

struct Velocity {
    float x;
    float y;
};



void update(registry& registry, float dt) {
    //TODO: Figure out the data types here.
    //I think I was close?
    //  basic_view<get_t<Position, Velocity>, exclude_t<>>
    auto view = registry.view<Position, Velocity>();
    
    //NOTE: The 1st parameter is optional
    view.each([](const entity e, const Position& p, const Velocity& v) {
        
    });

    for (auto [entity, pos, vel] : view.each()) {

    }

    for (entity e : view) {
        Position& p = view.get<Position>(e);
        Velocity& v = view.get<Velocity>(e);

        p.x += v.x * dt;
        p.y += v.y * dt;
    }
}

void test(registry& registry) {
    auto view = registry.view<Position, Velocity>();

    for (entity e : view) {
        Position& p = view.get<Position>(e);
        cout << "pos = (" << p.x << ", " << p.y << ")" << endl;
    }
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
        // registry.emplace<Position>(e, 0, 0);

        registry.emplace<Position>(e);

        if (i % 2 == 0) {
            // registry.emplace<Velocity>(e, i, (float) i / 2);
            Velocity& v = registry.emplace<Velocity>(e);
            v.x = i;
            v.y = (float) i / 2;
        }
    }

    for (int i = 0; i < 10; i++) {
        update(registry, 0.1f);
        test(registry);
    }
}
