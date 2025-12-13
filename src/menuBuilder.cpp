#include "menuBuilder.h"

menuBuilder::menuBuilder(menuDef* defRoot, menuItem* menuItemPool, int poolSize) {
    this->poolSize = poolSize;
    menuItemPool[poolIndex++].init("Root", nullptr, nullptr, nullptr, nullptr, nullptr);
    menuItem *rootItem = &menuItemPool[poolIndex - 1];
    menuItemPool[poolIndex - 1].addChild(buildMenuRecursive(defRoot, rootItem, menuItemPool));
}

menuItem* menuBuilder::buildMenuRecursive(menuDef* defRoot, menuItem* parentItem, menuItem* menuItemPool) {
    if (defRoot == nullptr || defRoot[0].name == nullptr) {
        return nullptr; // No more items to process
    }
    if (poolSize <= poolIndex) {
        return nullptr; // Pool exhausted
    }
    menuItem* currentItem,* prevSibling = nullptr, *childItem, *currentRoot;
    currentRoot = &menuItemPool[poolIndex];
    for (int i = 0; defRoot[i].name != nullptr; i++) {
        if (poolSize <= poolIndex) {
            return nullptr; // Pool exhausted
        }
        currentItem = &menuItemPool[poolIndex++];
        if(defRoot[i].subs != nullptr) {
            childItem = buildMenuRecursive(defRoot[i].subs, currentItem, menuItemPool);
        } else {
            childItem = nullptr; // No children
        }
        currentItem->init(defRoot[i].name, defRoot[i].action, parentItem, childItem, nullptr, prevSibling);
        if(prevSibling != nullptr) {
            prevSibling->addSiblingNext(currentItem);
        }
        prevSibling = currentItem;
    }
    return currentRoot;
}
