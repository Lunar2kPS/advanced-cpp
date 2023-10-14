#include <iostream>

#include "entt/entt.hpp"

using std::cout;
using std::endl;

using namespace entt;

struct Transform {
    float positionX;
    float positionY;
};

struct Velocity {
    float velocityX;
    float velocityY;
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

        t.positionX = (t.positionX + v.velocityX * dt);
        t.positionY = (t.positionY + v.velocityY * dt);
    }
}

void test(registry& registry) {
    auto view = registry.view<Transform, Velocity>();

    for (entity e : view) {
        Transform& t = view.get<Transform>(e);
        //NOTE: https://stackoverflow.com/questions/11515469/how-do-i-print-vector-values-of-type-glmvec3-that-have-been-passed-by-referenc
        cout << "pos = (" << t.positionX << ", " << t.positionY << ")" << endl;
    }
    cout << endl;
}

int main() {
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
            v.velocityX = i;
            v.velocityY = (float) i / 2;
        }
    }

    for (int i = 0; i < 10; i++) {
        update(registry, 0.1f);
        test(registry);
    }
    return 0;
}
