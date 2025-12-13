#include "Menu.h"

Menu::Menu(SSD1306Wire* display, menuDef* menuDefinition, int menuSize, int screenWidth, int screenHeight, int itemFontHeight)
    : display(display),
      menuDefinition(menuDefinition),
      menuSize(menuSize),
      screenWidth(screenWidth),
      screenHeight(screenHeight),
      itemFontHeight(itemFontHeight)
{
    this->menuPool = new menuItem[this->menuSize + 1]; // +1 for root item
    menuBuilder builder(menuDefinition, menuPool, menuSize);
    (void)builder;

    this->navigator = new menuNavigator(menuPool[0].getChild());
    this->printer = new menuOledPrinter(display, screenWidth, screenHeight, itemFontHeight, navigator);
}

Menu::~Menu() {
    delete printer;
    delete navigator;
    delete[] menuPool;
}

void Menu::print() {
    printer->print();
}

void Menu::navigate(Navigate direction) {
    navigator->navigate(direction);
}

void Menu::updateBorder(int border) {
    if (printer != nullptr) {
        printer->updateBorder(border);
    }
}

void Menu::updateGap(int gap) {
    if (printer != nullptr) {
        printer->updateGap(gap);
    }
}

void Menu::updatePadding(int padding) {
    if (printer != nullptr) {
        printer->updatePadding(padding);
    }
}

void Menu::updateFontHeight(int fontHeight) {
    this->itemFontHeight = fontHeight;
    if (printer != nullptr) {
        printer->updateFontHeight(fontHeight);
    }
}