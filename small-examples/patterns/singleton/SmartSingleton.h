#pragma once

#include <memory>

using std::shared_ptr;
using std::weak_ptr;

class SmartSingleton {
    private:
        static weak_ptr<SmartSingleton> instance;
        SmartSingleton();
    public:
        static shared_ptr<SmartSingleton> getInstance();
        ~SmartSingleton(); //NOTE: It seems for shared_ptr<SmartSingleton> to work, this can't be private
};
