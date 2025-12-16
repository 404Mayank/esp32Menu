#include <Arduino.h>
#include <SSD1306Wire.h>   
#include "Menu.h"
#include <Preferences.h>

//Display Defines
#define SCREEN_ADDRESS 0x3C
#define OLED_SDA  21
#define OLED_SCL  22
#define ITEMS_SHOW_AT_ONCE 3
#define ITEM_FONT_HEIGHT 10

//Button Defines
#define BTN_UP      19
#define BTN_DOWN    23
#define BTN_SELECT  15
#define BTN_BACK    12   
const uint32_t debounceTimeMs = 40;

//LED Defines
#define LED_PIN 2

//Menu Defines
#define MENU_SIZE 20  // Max number of menu items or exact (preferably)


// Non-blocking LED blink
bool ledBlinking = false;
unsigned long ledBlinkStart = 0;
const unsigned long ledBlinkDuration = 50;

void blinkLED();
extern Menu menu;

static void navigateAndRender(Navigate direction) {
  menu.navigate(direction);
  menu.print();
}

static void handleSerialControl() {
  static char token[16];
  static uint8_t tokenLen = 0;
  static uint8_t escState = 0; // 0=normal, 1=got ESC, 2=got ESC[

  while (Serial.available() > 0) {
    const char c = (char)Serial.read();

    // Handle ANSI escape sequences for arrow keys
    if (escState == 0 && c == '\x1B') {
      escState = 1;
      continue;
    }
    if (escState == 1) {
      if (c == '[') {
        escState = 2;
        continue;
      } else {
        escState = 0; // Not a valid escape sequence
      }
    }
    if (escState == 2) {
      escState = 0;
      switch (c) {
        case 'A': navigateAndRender(UP);    continue;
        case 'B': navigateAndRender(DOWN);  continue;
        case 'C': navigateAndRender(RIGHT); continue;
        case 'D': navigateAndRender(LEFT);  continue;
        default: break; // Unknown escape sequence
      }
      continue;
    }

    // Ignore common whitespace
    if (c == '\r' || c == '\n' || c == ' ' || c == '\t') {
      if ((c == '\r' || c == '\n') && tokenLen > 0) {
        token[tokenLen] = '\0';

        // Normalize simple words
        if (strcmp(token, "up") == 0) {
          navigateAndRender(UP);
        } else if (strcmp(token, "down") == 0) {
          navigateAndRender(DOWN);
        } else if (strcmp(token, "left") == 0) {
          navigateAndRender(LEFT);
        } else if (strcmp(token, "right") == 0) {
          navigateAndRender(RIGHT);
        }

        tokenLen = 0;
      }
      continue;
    }

    // Single-key immediate controls
    switch (c) {
      case 'u':
      case 'U':
      case 'w':
      case 'W':
        navigateAndRender(UP);
        tokenLen = 0;
        continue;
      case 's':
      case 'S':
        navigateAndRender(DOWN);
        tokenLen = 0;
        continue;
      case 'l':
      case 'L':
      case 'a':
      case 'A':
        navigateAndRender(LEFT);
        tokenLen = 0;
        continue;
      case 'r':
      case 'R':
        navigateAndRender(RIGHT);
        tokenLen = 0;
        continue;
      case 'd':
      case 'D':
        // Ambiguous with Down; prefer Down for 'd' to match "down".
        navigateAndRender(DOWN);
        tokenLen = 0;
        continue;
      default:
        break;
    }

    // Accumulate word tokens (lowercased)
    if (tokenLen < sizeof(token) - 1) {
      char lower = c;
      if (lower >= 'A' && lower <= 'Z') lower = (char)(lower - 'A' + 'a');
      token[tokenLen++] = lower;
    }
  }
}

struct DebouncedButton {
  uint8_t pin;
  bool stableState;
  bool lastReading;
  uint32_t lastChangeMs;
};

static bool readPressedEdge(DebouncedButton &btn) {
  const bool reading = (digitalRead(btn.pin) == LOW);

  if (reading != btn.lastReading) {
    btn.lastReading = reading;
    btn.lastChangeMs = millis();
  }

  if ((millis() - btn.lastChangeMs) >= debounceTimeMs) {
    if (reading != btn.stableState) {
      btn.stableState = reading;
      // Return true on the stable falling-edge (button press)
      if (btn.stableState) return true;
    }
  }

  return false;
}

static void handleMenu() {
  handleSerialControl();

  static DebouncedButton btnUp{BTN_UP, false, false, 0};
  static DebouncedButton btnDown{BTN_DOWN, false, false, 0};
  static DebouncedButton btnSelect{BTN_SELECT, false, false, 0};
  static DebouncedButton btnBack{BTN_BACK, false, false, 0};

  if (readPressedEdge(btnUp)) {
    Serial.println("BTN_UP pressed");
    navigateAndRender(UP);
  }

  if (readPressedEdge(btnDown)) {
    Serial.println("BTN_DOWN pressed");
    navigateAndRender(DOWN);
  }

  if (readPressedEdge(btnSelect)) {
    Serial.println("BTN_SELECT pressed");
    // In this menu implementation, RIGHT both enters submenus and selects leaf actions.
    navigateAndRender(RIGHT);
  }

  if (readPressedEdge(btnBack)) {
    Serial.println("BTN_BACK pressed");
    navigateAndRender(LEFT);
  }
}

void blinkLED() {
  ledBlinking = true;
  ledBlinkStart = millis();
  digitalWrite(LED_PIN, HIGH);
}

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


//OLED Globals
SSD1306Wire display(SCREEN_ADDRESS, OLED_SDA, OLED_SCL, GEOMETRY_128_64);

void fun1() {
  Serial.println("fun1: Connecting to WiFi...");
}

void fun2() {
  Serial.println("fun2: Starting Access Point...");
}

void fun3() {
  Serial.println("fun3: Showing AP QR Code...");
}

Preferences preferences;

void fun4() {
  Serial.println("fun4: Showing WiFi IP QR Code...");
  menu.toggleStyle();
  preferences.putInt("menuStyle", menu.getStyle());
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

// Menu Defination = {name, action, sub menu definition}
// Declare menu heigrarchy Bottom -> Top 

menuDef QRCodesDef[] = {
  {"Show AP QR", fun3, nullptr},
  {"Show WiFi IP QR", fun4, nullptr},
  {"Show AP IP QR", fun5, nullptr},
  {"Toggle WiFi", fun6, nullptr},
  {"Toggle AP", fun7, nullptr},
  {} //auto initializes last item as nullptr
};

menuDef WifiDef[] = {
  {"Connect to WiFi", fun1, nullptr},
  {"Start Access Point", fun2, nullptr},
  {}
};

menuDef root[] = {
  {"Wifi", nullptr, WifiDef},
  {"QR Codes", nullptr, QRCodesDef},
  {}
};

//Menu GLobals
Menu menu(&display, root, MENU_SIZE, 128, 64, ITEM_FONT_HEIGHT);


void setup() {
  //Display Setup
  Wire.begin(OLED_SDA, OLED_SCL); 
  display.init();
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);

  //buttons
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_SELECT, INPUT_PULLUP);
  pinMode(BTN_BACK, INPUT_PULLUP);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  preferences.begin("Meshmerize", false); 

  Serial.begin(9600);
  menu.setStyle(preferences.getInt("menuStyle", 0));
  menu.print();
}

void loop() {
  // Handle non-blocking LED blink
  if (ledBlinking && (millis() - ledBlinkStart >= ledBlinkDuration)) {
    digitalWrite(LED_PIN, LOW);
    ledBlinking = false;
  }

  handleMenu();
}