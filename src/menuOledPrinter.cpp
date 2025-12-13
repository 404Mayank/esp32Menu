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

void menuOledPrinter::setStyle(int style) {
    this->style = style;
}

int menuOledPrinter::getStyle() const {
    return this->style;
}

void menuOledPrinter::toggleStyle() {
    this->style = (this->style == 0) ? 1 : 0;
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
    // Always start from a known draw color; SSD1306Wire keeps color state across frames.
    display->setColor(WHITE);
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
            const int rowX = 0;
            const int rowY = y + this->gap;
            const int rowW = screenWidth;
            const int rowH = itemFontHeight + 2 * this->padding;

            if (this->style == 0) {
                // Filled highlight (current behavior)
                display->setColor(WHITE);
                display->fillRect(rowX, rowY, rowW, rowH);
                display->setColor(BLACK);
            } else {
                // Hollow highlight: border controls line thickness
                display->setColor(WHITE);
                const int thickness = (this->border <= 0) ? 1 : this->border;
                for (int t = 0; t < thickness; t++) {
                    const int x = rowX + t;
                    const int yy = rowY + t;
                    const int w = rowW - 2 * t;
                    const int h = rowH - 2 * t;
                    if (w <= 0 || h <= 0) break;
                    display->drawRect(x, yy, w, h);
                }
                // SSD1306 is monochrome; keep text WHITE for this style.
                display->setColor(WHITE);
            }
        } else {
            display->setColor(WHITE);
        }
        display->drawString(0 + this->border + this->padding, y + this->gap + this->border, String((topItem->getChild() != nullptr ? "+ " : "  ")) + topItem->getName());
        y += itemFontHeight + (this->gap + this->padding + this->border) * 2;
        topItem = topItem->getSiblingNext();
    }
    display->display();
}

