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

    class DefaultServiceLocator {
        public:
            template <typename T>
            static T* getSystem();
    };

    //TODO: Make this ServiceLocator generic, to let you operate on IGameLoopSystems, OR any custom base class type!
    class ServiceLocator {
        private:
            static ServiceLocator* instance;
        public:
            static ServiceLocator* getInstance();
            static bool createInstance();
            static bool destroyInstance();

            template<typename T>
            static T* getSystem(ServiceLocator* locator);
        private:
            unordered_map<size_t, IGameLoopSystem*> systems;
        public:
            ServiceLocator();

            int getCount();

            template <typename T>
            T* getSystem();

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
