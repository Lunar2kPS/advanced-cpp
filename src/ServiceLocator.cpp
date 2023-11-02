#include "ServiceLocator.h"

#include <algorithm>

using std::pair;
using std::sort;

namespace carlos {
    ServiceLocator* ServiceLocator::instance;
    ServiceLocator* ServiceLocator::getInstance() { return instance; }
    bool ServiceLocator::createInstance() {
        if (instance != nullptr)
            return false;
        instance = new ServiceLocator();
        return true;
    }
    bool ServiceLocator::destroyInstance() {
        if (instance == nullptr)
            return false;
        delete instance;
        return true;
    }

    ServiceLocator::ServiceLocator() { }

    int ServiceLocator::getCount() { return systems.size(); }
    void ServiceLocator::getSystems(vector<IGameLoopSystem*>& results, SortMode mode, Condition condition) {
        results.clear();
        for (pair<const size_t, IGameLoopSystem*>& iterator : systems) {
            if (condition == nullptr || condition(iterator.second))
                results.push_back(iterator.second);
        }

        switch (mode) {
            case SortMode::BY_ORDER:
                sort(results.begin(), results.end(), [](IGameLoopSystem* lhs, IGameLoopSystem* rhs) {
                    return lhs->getOrder() < rhs->getOrder();
                });
                break;
            case SortMode::BY_REVERSE_ORDER:
                sort(results.begin(), results.end(), [](IGameLoopSystem* lhs, IGameLoopSystem* rhs) {
                    return lhs->getOrder() > rhs->getOrder();
                });
                break;
        }
    }
}
