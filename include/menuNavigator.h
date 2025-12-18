#pragma once

#include "menuItem.h"

enum Navigate { UP, DOWN, LEFT, RIGHT, SELECT };

class menuNavigator {
    public:
    menuNavigator() = default;
    menuNavigator(menuItem* root);
    void navigate(Navigate direction);
    menuItem* getCurrentItem();

    private:
    menuItem* currentItem = nullptr;
};
