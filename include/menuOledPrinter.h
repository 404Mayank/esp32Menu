#pragma once

#include "menuItem.h"
#include <SSD1306Wire.h>
#include "menuNavigator.h"

class menuOledPrinter {
    public:
    menuOledPrinter() = default;
    menuOledPrinter(SSD1306Wire* display, int screenWidth, int screenHeight, int itemFontHeight, menuNavigator* navigator);
    void print();
    void updateItemsShowAtOnce();
    void updateBorder(int border);
    void updateGap(int gap);
    void updatePadding(int padding);
    void updateFontHeight(int fontHeight);

    private:
    SSD1306Wire* display;
    int screenWidth;
    int screenHeight;
    int itemsShowAtOnce;
    int itemFontHeight;
    menuNavigator* navigator;
    int gap = 1;
    int border = 1;
    int padding = 2;
};