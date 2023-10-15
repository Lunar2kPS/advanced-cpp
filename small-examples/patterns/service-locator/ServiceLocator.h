#pragma once

#include <typeinfo>
#include <unordered_map>

using std::unordered_map;

class ServiceLocator {
    private:
        unordered_map<size_t, void*> systems;
    public:
        ServiceLocator();

        template <typename T>
        T* getSystem();

        template <typename T>
        void addSystem(T* system);
};

//WARNING: For some reason, the compiler and/or linker need these template functions to be available immediately in the .h header file,
//      Cause it won't be able to automatically "extend" this template function for all the different types
//      used with it from the caller's code unless this is present here (instead of in the .cpp file):
template <typename T>
T* ServiceLocator::getSystem() {
    //NOTE: According to https://en.cppreference.com/w/cpp/types/type_info
    //  The type_info's hash_code() is guaranteed to be identical for the same types.
    //WARNING: Not sure how likely a collision is, but perhaps we don't need to worry about it?
    size_t key = typeid(T).hash_code();
    unordered_map<size_t, void*>::const_iterator search = systems.find(key);
    if (search != systems.end())
        return static_cast<T*>(systems[search->first]);
    return nullptr;
}

template <typename T>
void ServiceLocator::addSystem(T* system) {
    size_t key = typeid(T).hash_code();
    systems[key] = system;
}