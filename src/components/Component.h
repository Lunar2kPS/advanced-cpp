#pragma once

namespace carlos {
    class GameObject;

    /// @brief An object that defines data and/or behavior, and attaches to an GameObject.
    /// Without Components, GameObjects would be empty.
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
