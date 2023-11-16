#pragma once

#include <typeinfo>
#include <unordered_map>
#include <vector>

#include "interfaces/IGameLoopSystem.h"

using std::unordered_map;
using std::vector;

namespace carlos {
    enum class SortMode {
        NONE,
        BY_ORDER,
        BY_REVERSE_ORDER
    };

    typedef bool (*Condition)(IGameLoopSystem*);

    /// @brief Provides easy access to the default ServiceLocator.
    class DefaultServiceLocator {
        public:
            template <typename T>
            static T* getSystem();
    };

    //TODO: Make this ServiceLocator generic, to let you operate on IGameLoopSystems, OR any custom base class type!

    /// @brief Represents a main center point for accessing the major systems of a program.
    /// @note (This is based upon the common service locator programming design pattern).
    class ServiceLocator {
        private:
            static ServiceLocator* instance;
        public:
            /// @brief Gets a singleton instance for easy use of the service locator pattern across the entire application.
            /// @note Note that it is up to the caller to call ServiceLocator::createInstance() and ServiceLocator::destroyInstance().
            static ServiceLocator* getInstance();
            static bool createInstance();
            static bool destroyInstance();

            template<typename T>
            static T* getSystem(ServiceLocator* locator);
        private:
            unordered_map<size_t, IGameLoopSystem*> systems;
        public:
            ServiceLocator();

            /// @brief Gets the total number of systems currently registered.
            int getCount();

            /// @brief Gets a system from the internal list of systems, if one exists of this given (exact) type.
            /// @note Note that polymorphism is NOT supported here. Only the exact data type you used with addSystem<T>(T*) works.
            /// @tparam T The type of system to search for.
            /// @return A pointer to the system, if any, or nullptr otherwise.
            template <typename T>
            T* getSystem();

            /// @brief Adds a system to the ServiceLocator's internal list of systems, allowing it to be retrieved by getSystem<T>().
            /// @tparam T The type to bind this system to. This is important, as only this exact type will be usable to search for this system again (with the current implementation).
            /// @param system A pointer to the existing object to add as a system.
            template <typename T>
            void addSystem(T* system);

            void getSystems(vector<IGameLoopSystem*>& results, SortMode mode, Condition condition = nullptr);
    };

    template <typename T>
    T* DefaultServiceLocator::getSystem() {
        return ServiceLocator::getSystem<T>(ServiceLocator::getInstance());
    }

    template <typename T>
    T* ServiceLocator::getSystem(ServiceLocator* locator) {
        if (locator == nullptr)
            return nullptr;
        return locator->getSystem<T>();
    }

    //WARNING: For some reason, the compiler and/or linker need these template functions to be available immediately in the .h header file,
    //      Cause it won't be able to automatically "extend" this template function for all the different types
    //      used with it from the caller's code unless this is present here (instead of in the .cpp file):
    template <typename T>
    T* ServiceLocator::getSystem() {
        //NOTE: According to https://en.cppreference.com/w/cpp/types/type_info
        //  The type_info's hash_code() is guaranteed to be identical for the same types.
        //WARNING: Not sure how likely a collision is, but perhaps we don't need to worry about it?
        size_t key = typeid(T).hash_code();
        unordered_map<size_t, IGameLoopSystem*>::const_iterator search = systems.find(key);
        if (search != systems.end())
            return dynamic_cast<T*>(systems[search->first]);
        return nullptr;
    }

    template <typename T>
    void ServiceLocator::addSystem(T* system) {
        size_t key = typeid(T).hash_code();
        systems[key] = system;
    }
}
