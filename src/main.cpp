#include <Arduino.h>
#include <SSD1306Wire.h>   

//Display Defines
#define SCREEN_ADDRESS 0x3C
#define OLED_SDA  21
#define OLED_SCL  22
#define ITEMS_SHOW_AT_ONCE 3
#define ITEM_FONT_HEIGHT 16

//SCREEN START
//margin 4px
//fisrt MENU ITEM (16px)
//MARGIN 4px
//SECOND MENU ITEM (16px)
//MARGIN 4px
//THIRD MENU ITEM (16px)
//MARGIN 4px
//SCREEN END

//TOTAL HEIGHT 64PX
//VISIBLE ITEMS 3

struct menuItem {
  const char* name;
  void (*action)();
  menuItem* parent;
  menuItem* child;
  menuItem* next;
  menuItem* prev;
  menuItem* lastMenuHead;
};

struct menuDef {
  const char* name;
  void (*action)();
  struct menuDef* subs;
};

// Globals
SSD1306Wire display(SCREEN_ADDRESS, OLED_SDA, OLED_SCL, GEOMETRY_128_64);
//MenuGlobals
enum Navigate {UP, DOWN, LEFT, RIGHT, SELECT};

void fun1() {
  Serial.println("fun1: Connecting to WiFi...");
}

void fun2() {
  Serial.println("fun2: Starting Access Point...");
}

void fun3() {
  Serial.println("fun3: Showing AP QR Code...");
}

void fun4() {
  Serial.println("fun4: Showing WiFi IP QR Code...");
}

void fun5() {
  Serial.println("fun5: Showing AP IP QR Code...");
}

void fun6() {
  Serial.println("fun6: Toggling WiFi...");
}

void fun7() {
  Serial.println("fun7: Toggling AP...");
}

// Menu Defination = {name, action, sub menu definition, last menu head position(self-managed) }
// Declare menu heigrarchy Bottom -> Top 

menuDef QRCodesDef[] = {
  {"Show AP QR", fun3, nullptr},
  {"Show WiFi IP QR", fun4, nullptr},
  {"Show AP IP QR", fun5, nullptr},
  {"Toggle WiFi", fun6, nullptr},
  {"Toggle AP", fun7, nullptr},
  {nullptr, nullptr, nullptr}
};

menuDef WifiDef[] = {
  {"Connect to WiFi", fun1, nullptr},
  {"Start Access Point", fun2, nullptr},
  {nullptr, nullptr, nullptr}
};

menuDef menu[] = {
  {"Wifi", nullptr, WifiDef},
  {"QR Codes", nullptr, QRCodesDef},
  {nullptr, nullptr, nullptr}
};

menuItem root = { "Main Menu", nullptr, nullptr, nullptr, nullptr, nullptr };

menuItem* addMenuEntry(const char* name, void (*action)(), menuItem* menuPtr) {
  menuItem* currentMenu = menuPtr;
  menuItem* newMenu = (struct menuItem*)malloc(sizeof(struct menuItem));
  if (newMenu == NULL) {
    Serial.println("Memory allocation failed");
    return nullptr;
  }
  newMenu->name = name;
  newMenu->action = action;
  newMenu->parent = currentMenu->parent;
  newMenu->child = nullptr;
  newMenu->next = currentMenu->next;
  newMenu->prev = currentMenu;
  newMenu->lastMenuHead = nullptr;
  if(currentMenu->next != nullptr) {
    currentMenu->next->prev = newMenu;
  }
  currentMenu->next = newMenu;
  currentMenu = newMenu;
  return newMenu;
}

 menuItem* addSubMenuEntry(const char* name, void (*action)(), menuItem* menuPtr) {
  menuItem* currentMenu = menuPtr;
  if (currentMenu->child == nullptr) {
      menuItem* newMenu = (struct menuItem*)malloc(sizeof(struct menuItem));
    if (newMenu == NULL) {
      Serial.println("Memory allocation failed");
      return nullptr;
    }
    newMenu->name = name;
    newMenu->action = action;
    newMenu->parent = currentMenu;
    newMenu->child = nullptr;
    newMenu->next = nullptr;
    newMenu->prev = nullptr;
    newMenu->lastMenuHead = nullptr;
    currentMenu->child = newMenu;
    currentMenu = newMenu;
    return newMenu;
  } else {
    currentMenu = currentMenu->child;
    while (currentMenu->next != nullptr) {
    currentMenu = currentMenu->next;
    }
    return addMenuEntry(name, action, currentMenu);
  }
}

void createMenu(menuDef* defs, menuItem* root) {
  menuItem* menuPtr = root;
  menuItem* lastItem = nullptr;

  for(int i = 0; defs[i].name != nullptr; i++) {
    menuItem* tmpPtr;
    if(i == 0) {
      tmpPtr = addSubMenuEntry(defs[i].name, defs[i].action, menuPtr);
    } else {
      tmpPtr = addMenuEntry(defs[i].name, defs[i].action, lastItem);
    }
    lastItem = tmpPtr;
    if(defs[i].subs != nullptr) {
      createMenu(defs[i].subs, tmpPtr);
    }
  }
}

  menuItem* selectedItem;
  menuItem* menuHeadPtr;

void navigateMenu(enum Navigate direction) {
  menuItem* ptr = menuHeadPtr;
  switch(direction) {
    case UP:
      if(selectedItem->prev == nullptr) {
        Serial.println("No Prev menu");
        break;
      }
      else {
        Serial.println("Navigating Up");
        selectedItem = selectedItem->prev;
        if(selectedItem == menuHeadPtr->prev) {
          menuHeadPtr = menuHeadPtr->prev;
        }
      }
      break;
    case DOWN:
      if(selectedItem->next == nullptr) {
        Serial.println("No Next menu");
        break;
      }
      else {
        Serial.println("Navigating Down");
        selectedItem = selectedItem->next;
        for(int i = 0; i < (ITEMS_SHOW_AT_ONCE - 1); i++) {
          if(ptr->next == nullptr)
            break;
          else if(selectedItem == ptr)
            break;
          else {
            ptr = ptr->next;
          }
        }
        if(selectedItem != ptr)
          menuHeadPtr = menuHeadPtr->next;
      }
      break;
    case LEFT:
      if(selectedItem->parent->parent != nullptr) {
        Serial.println("Navigating Left");
        if(selectedItem->parent != nullptr) {
          selectedItem = selectedItem->parent;
          menuHeadPtr = selectedItem->lastMenuHead != nullptr ? selectedItem->lastMenuHead : selectedItem;
        }
      }
      else {
        Serial.println("No Parent menu (Reached Root Menu)");
      }
      break;
    case RIGHT:
      if(selectedItem->child != nullptr) {
        Serial.println("Navigating Right");
        selectedItem->lastMenuHead = menuHeadPtr;
        selectedItem = selectedItem->child;
        menuHeadPtr = selectedItem;
      }
      else {
        Serial.println("No Child menu");
      }
      break;
    case SELECT:
      Serial.print("Opening \"");
      Serial.print(selectedItem->name);
      Serial.println("\"");
      if(selectedItem->action != nullptr) {
        selectedItem->action();
      }
      break;
  }
}

void drawMenu() {
  menuItem* ptr = menuHeadPtr;
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  int margin = (display.getHeight() - (ITEM_FONT_HEIGHT * ITEMS_SHOW_AT_ONCE)) / ITEMS_SHOW_AT_ONCE;
  for (int i = 0; i < ITEMS_SHOW_AT_ONCE; i++) {
    if(ptr == selectedItem) {
      display.fillRect(0, (ITEM_FONT_HEIGHT + margin) * i, display.getWidth(), ITEM_FONT_HEIGHT + margin);
      display.setColor(BLACK);
      display.drawString(0, (i  * ITEM_FONT_HEIGHT) + (margin * (i + 1)), String(" ") + ptr->name);
      display.setColor(WHITE);
    } else {
      display.drawRect(0, (ITEM_FONT_HEIGHT + margin) * i, display.getWidth(), ITEM_FONT_HEIGHT + margin); 
      display.drawString(0, (i  * ITEM_FONT_HEIGHT) + (margin * (i + 1)), String(" ") + ptr->name);
    }
    if(ptr->next == nullptr)
      break;
    else
      ptr = ptr->next;
  }
  display.display();
}

void setup() {
  //Display Setup
  Wire.begin(OLED_SDA, OLED_SCL); 
  display.init();
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.flipScreenVertically();

  Serial.begin(9600);

  createMenu(menu, &root);
  selectedItem = root.child;
  menuHeadPtr = selectedItem;
  drawMenu();

}

void loop() {
  if (Serial.available() > 0) {
    int inByte = Serial.read();

    // Enter key
    if (inByte == '\n' || inByte == '\r') {
      navigateMenu(SELECT);
      drawMenu();
      // Flush remaining newline characters
      delay(10);
      while (Serial.available() > 0 && (Serial.peek() == '\n' || Serial.peek() == '\r')) {
        Serial.read();
      }
    }
    // Arrow keys (Escape sequence)
    else if (inByte == 27) {
      delay(20); // Wait for sequence
      if (Serial.available() >= 2) {
        if (Serial.read() == '[') {
          switch(Serial.read()) {
            case 'A': navigateMenu(UP); break;
            case 'B': navigateMenu(DOWN); break;
            case 'D': navigateMenu(LEFT); break;
            case 'C': navigateMenu(RIGHT); break;
          }
          drawMenu();
        }
      }
    }
  }
}