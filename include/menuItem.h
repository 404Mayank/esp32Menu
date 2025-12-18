#pragma once

class menuItem {
public:
  menuItem() = default;
  const char* name = nullptr;
  void (*action)() = nullptr;
  menuItem* navParent = nullptr;

  const char* getName() const;
  void init(const char* itemName, void (*itemAction)(), menuItem* parentItem, menuItem* childItem, menuItem* nextItem, menuItem* prevItem);
  menuItem(const char* itemName, void (*itemAction)(), menuItem* parentItem, menuItem* childItem, menuItem* nextItem, menuItem* prevItem);

  void addChild(menuItem* childItem);
  void addParent(menuItem* parentItem);
  void addSiblingNext(menuItem* nextItem);
  void addSiblingPrev(menuItem* prevItem);

  menuItem* getChild() const;
  menuItem* getParent() const;
  menuItem* getSiblingNext() const;
  menuItem* getSiblingPrev() const;

  void select() const;

private:
  menuItem* parent = nullptr;
  menuItem* child = nullptr;
  menuItem* siblingNext = nullptr;
  menuItem* siblingPrev = nullptr;
};