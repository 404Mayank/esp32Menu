#pragma once

#include "menuDef.h"
#include "menuItem.h"

class menuBuilder {
public:
    menuBuilder(menuDef* defRoot, menuItem* menuItemPool, int poolSize);
    menuItem* buildMenuRecursive(menuDef* defRoot, menuItem* parentItem, menuItem* menuItemPool);

private:
    int poolIndex = 0;
    int poolSize = 0;
};