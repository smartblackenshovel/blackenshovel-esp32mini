#pragma once
#include "models/User.h"
#include <vector>

class ISessionUpdater {
    public:
        virtual ~ISessionUpdater() {}
        virtual void selectUser(String id) = 0;
        virtual void completeSpot(String id) = 0;
};