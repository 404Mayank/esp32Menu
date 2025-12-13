#pragma once

#include "menuItem.h"
#include "menuDef.h"
#include "menuBuilder.h"
#include "menuNavigator.h"
#include "menuOledPrinter.h"
#include <SSD1306Wire.h>

class Menu {
public:
    Menu() = default;
    Menu(SSD1306Wire* display, menuDef* menuDefinition, int menuSize, int screenWidth, int screenHeight, int itemFontHeight);
    ~Menu();
    void print();
    void navigate(Navigate direction);

    void updateBorder(int border);
    void updateGap(int gap);
    void updatePadding(int padding);
    void updateFontHeight(int fontHeight);

    private:
    SSD1306Wire* display = nullptr;
    menuDef* menuDefinition = nullptr;
    int menuSize = 0;
    int screenWidth = 0;
    int screenHeight = 0;
    int itemFontHeight = 0;

    menuNavigator* navigator = nullptr;
    menuOledPrinter* printer = nullptr;
    menuItem* menuPool = nullptr;
};