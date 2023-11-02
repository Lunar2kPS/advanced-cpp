#pragma once

namespace carlos {
    class GameObject;

    class Component {
        private:
            GameObject* gameObject;

        public:
            Component() { }
            virtual ~Component() { }

            GameObject* getGameObject() { return gameObject; }
            void setGameObject(GameObject* gameObject) { this->gameObject = gameObject; }
    };
}
