#include "menuItem.h"

const char* menuItem::getName() const {
    return name;
}

void menuItem::init(const char* itemName, void (*itemAction)(), menuItem* parentItem, menuItem* childItem, menuItem* nextItem, menuItem* prevItem) {
    this->name = itemName;
    this->action = itemAction;
    this->parent = parentItem;
    this->child = childItem;
    this->siblingNext = nextItem;
    this->siblingPrev = prevItem;
}

menuItem::menuItem(const char* itemName, void (*itemAction)(), menuItem* parentItem, menuItem* childItem, menuItem* nextItem, menuItem* prevItem)
    : name(itemName),
      action(itemAction),
      parent(parentItem),
      child(childItem),
      siblingNext(nextItem),
      siblingPrev(prevItem) 
{}

void menuItem::addChild(menuItem* childItem) {
    this->child = childItem;
}

void menuItem::addParent(menuItem* parentItem) {
    this->parent = parentItem;
}

void menuItem::addSiblingNext(menuItem* nextItem) {
    this->siblingNext = nextItem;
}

void menuItem::addSiblingPrev(menuItem* prevItem) {
    this->siblingPrev = prevItem;
}

menuItem* menuItem::getChild() const {
    return child;
}

menuItem* menuItem::getParent() const {
    return parent;
}

menuItem* menuItem::getSiblingNext() const {
    return siblingNext;
}

menuItem* menuItem::getSiblingPrev() const {
    return siblingPrev;
}

void menuItem::select() const {
    if (action != nullptr) {
        action();
    }
}
