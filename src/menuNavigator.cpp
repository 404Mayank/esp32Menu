#include "menuNavigator.h"
#include "menuItem.h"

menuNavigator::menuNavigator(menuItem* root) 
    : currentItem(root), rootItem(root)
{}

void menuNavigator::navigate(Navigate direction) {
    switch (direction) {
        case UP:
            if (currentItem->getSiblingPrev() != nullptr) {
                currentItem = currentItem->getSiblingPrev();
            }
            break;
        case DOWN:
            if (currentItem->getSiblingNext() != nullptr) {
                currentItem = currentItem->getSiblingNext();
            }
            break;
        case LEFT:
            if (currentItem->getParent() != nullptr) {
                if (rootItem == nullptr) {
                    rootItem = currentItem->getParent();
                }
                currentItem = rootItem;
            }
            break;
        case RIGHT:
            if (currentItem->getChild() != nullptr) {
                rootItem = currentItem;
                currentItem = currentItem->getChild();
            }
            else if (currentItem->action != nullptr) {
                this->navigate(SELECT);
            }
            break;
        case SELECT:
            currentItem->select();
            break;
    }
}

menuItem* menuNavigator::getCurrentItem() {
    return currentItem;
}