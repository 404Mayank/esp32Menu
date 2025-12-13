#include "menuOledPrinter.h"
#include <SSD1306Wire.h>

menuOledPrinter::menuOledPrinter(SSD1306Wire* display, int screenWidth, int screenHeight, int itemFontHeight, menuNavigator* navigator)
    : display(display),
      screenWidth(screenWidth),
      screenHeight(screenHeight),
      itemFontHeight(itemFontHeight),
      navigator(navigator)
{
    updateItemsShowAtOnce();
}

void menuOledPrinter::updateItemsShowAtOnce() {
    this->itemsShowAtOnce = (screenHeight) / (itemFontHeight + (this->gap + this->border + this->padding) * 2);
}

void menuOledPrinter::updateBorder(int border) {
    this->border = border;
    updateItemsShowAtOnce();
}

void menuOledPrinter::updateGap(int gap) {
    this->gap = gap;
    updateItemsShowAtOnce();
}

void menuOledPrinter::updatePadding(int padding) {
    this->padding = padding;
    updateItemsShowAtOnce();
}

void menuOledPrinter::updateFontHeight(int fontHeight) {
    this->itemFontHeight = fontHeight;
    updateItemsShowAtOnce();
}

void menuOledPrinter::print() {
    display->clear();
    menuItem* currentItem = navigator->getCurrentItem();

    menuItem* topItem = currentItem;
    for (int i = 0; i < this->itemsShowAtOnce / 2; i++) {
        if (topItem->getSiblingPrev() != nullptr) {
            topItem = topItem->getSiblingPrev();
        }
    }

    int y = (screenHeight - ((((this->padding + this->gap + this->border) * 2 + itemFontHeight) * this->itemsShowAtOnce))) / 2;

    for (int i = 0; i < this->itemsShowAtOnce; i++) {
        if (topItem == nullptr) {
            break;
        }
        if (topItem == currentItem) {
            // Highlight current item
            display->fillRect(0, y + this->gap, screenWidth, itemFontHeight + 2 * this->padding);
        }
        if (topItem == currentItem) {
            display->setColor(BLACK);
        } else {
            display->setColor(WHITE);
        }
        display->drawString(0 + this->border + this->padding, y + this->gap + this->border, String((topItem->getChild() != nullptr ? "+ " : "  ")) + topItem->getName());
        y += itemFontHeight + (this->gap + this->padding + this->border) * 2;
        topItem = topItem->getSiblingNext();
    }
    display->display();
}

