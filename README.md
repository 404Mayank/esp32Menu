# esp32Menu

A flexible, hierarchical menu system for ESP32 microcontrollers with SSD1306 OLED display support. This library provides an easy-to-use framework for creating nested menu structures with keyboard/serial navigation.

## Features

- 🎯 **Hierarchical Menu Structure**: Support for multi-level nested menus
- 📺 **OLED Display Support**: Optimized for SSD1306 128x64 displays
- ⌨️ **Keyboard Navigation**: Navigate menus using arrow keys and Enter via Serial
- 🔄 **Dynamic Menu Creation**: Build menus programmatically at runtime
- 💾 **Memory Efficient**: Linked-list based structure with minimal overhead
- 🎨 **Visual Feedback**: Selected item highlighting with inverted colors
- 📱 **Scrollable Lists**: Automatically scrolls through long menu lists (shows 3 items at once)

## Hardware Requirements

- **ESP32 Development Board** (tested on ESP32 DOIT DevKit V1)
- **SSD1306 OLED Display** (128x64 pixels, I2C interface)
- **Connecting Wires**

## Wiring Diagram

Connect the OLED display to your ESP32 board:

| OLED Pin | ESP32 Pin | Description |
|----------|-----------|-------------|
| VCC      | 3.3V      | Power supply |
| GND      | GND       | Ground |
| SCL      | GPIO 22   | I2C Clock |
| SDA      | GPIO 21   | I2C Data |

## Software Dependencies

This project uses PlatformIO and requires the following library:

- **ESP8266 and ESP32 OLED driver for SSD1306 displays** v4.4.0 or higher
  - By ThingPulse
  - Automatically installed via PlatformIO

## Installation

### Using PlatformIO

1. **Clone the repository**:
   ```bash
   git clone https://github.com/404Mayank/esp32Menu.git
   cd esp32Menu
   ```

2. **Open in PlatformIO**:
   - Open the project folder in VSCode with PlatformIO extension installed
   - Or use PlatformIO CLI

3. **Build the project**:
   ```bash
   pio run
   ```

4. **Upload to ESP32**:
   ```bash
   pio run --target upload
   ```

5. **Open Serial Monitor**:
   ```bash
   pio device monitor -b 9600
   ```

## Usage

### Basic Navigation

The menu system supports keyboard navigation via the Serial Monitor:

- **↑ (Up Arrow)**: Move to previous menu item
- **↓ (Down Arrow)**: Move to next menu item
- **← (Left Arrow)**: Go back to parent menu
- **→ (Right Arrow)**: Enter submenu
- **Enter**: Execute selected menu action

### Menu Display

The display shows 3 menu items at a time with the following features:
- Selected item has inverted background (white text on black)
- Other items have outlined borders (black text on white)
- Auto-scrolls when navigating beyond visible items

## Code Structure

### Key Components

#### 1. Menu Item Structure
```cpp
struct menuItem {
  const char* name;        // Display name
  void (*action)();        // Function to execute on selection
  menuItem* parent;        // Parent menu (for going back)
  menuItem* child;         // First submenu item
  menuItem* next;          // Next sibling item
  menuItem* prev;          // Previous sibling item
  menuItem* lastMenuHead;  // Tracks scroll position
};
```

#### 2. Menu Definition Structure
```cpp
struct menuDef {
  const char* name;        // Menu item name
  void (*action)();        // Function pointer
  struct menuDef* subs;    // Submenu array
};
```

### Core Functions

#### `createMenu(menuDef* defs, menuItem* root)`
Creates a menu hierarchy from menu definitions.

**Parameters:**
- `defs`: Array of menu definitions
- `root`: Root menu item to attach to

#### `navigateMenu(enum Navigate direction)`
Handles menu navigation based on direction.

**Parameters:**
- `direction`: UP, DOWN, LEFT, RIGHT, or SELECT

#### `drawMenu()`
Renders the current menu state to the OLED display.

#### `addMenuEntry(const char* name, void (*action)(), menuItem* menuPtr)`
Adds a menu item as a sibling to the current menu.

**Parameters:**
- `name`: Display name for the menu item
- `action`: Function to execute when selected
- `menuPtr`: Current menu position

**Returns:** Pointer to newly created menu item

#### `addSubMenuEntry(const char* name, void (*action)(), menuItem* menuPtr)`
Adds a submenu item to the current menu.

**Parameters:**
- `name`: Display name for the menu item
- `action`: Function to execute when selected
- `menuPtr`: Parent menu item

**Returns:** Pointer to newly created submenu item

## Creating Custom Menus

### Step 1: Define Menu Actions

Create functions that will be executed when menu items are selected:

```cpp
void myAction1() {
  Serial.println("Action 1 executed!");
  // Your custom code here
}

void myAction2() {
  Serial.println("Action 2 executed!");
  // Your custom code here
}
```

### Step 2: Define Menu Structure

Define your menu hierarchy (bottom-up approach):

```cpp
// Define submenu first
menuDef subMenu[] = {
  {"SubItem 1", myAction1, nullptr},
  {"SubItem 2", myAction2, nullptr},
  {nullptr, nullptr, nullptr}  // Terminator
};

// Define main menu
menuDef mainMenu[] = {
  {"Menu with Submenu", nullptr, subMenu},
  {"Direct Action", myAction1, nullptr},
  {nullptr, nullptr, nullptr}  // Terminator
};
```

### Step 3: Initialize Menu in setup()

```cpp
void setup() {
  // ... display initialization ...
  
  createMenu(mainMenu, &root);
  selectedItem = root.child;
  menuHeadPtr = selectedItem;
  drawMenu();
}
```

## Example Menu Structure

The default implementation includes:

```
Main Menu
├── Wifi
│   ├── Connect to WiFi
│   └── Start Access Point
└── QR Codes
    ├── Show AP QR
    ├── Show WiFi IP QR
    ├── Show AP IP QR
    ├── Toggle WiFi
    └── Toggle AP
```

## Configuration

### Display Settings

Modify these constants in `main.cpp` to customize the display:

```cpp
#define SCREEN_ADDRESS 0x3C           // I2C address of OLED
#define OLED_SDA  21                  // SDA pin
#define OLED_SCL  22                  // SCL pin
#define ITEMS_SHOW_AT_ONCE 3          // Visible items
#define ITEM_FONT_HEIGHT 16           // Font height in pixels
```

### Changing I2C Pins

If you need to use different GPIO pins for I2C:

1. Update the defines:
   ```cpp
   #define OLED_SDA  <your_sda_pin>
   #define OLED_SCL  <your_scl_pin>
   ```

2. The display initialization automatically uses these pins:
   ```cpp
   Wire.begin(OLED_SDA, OLED_SCL);
   ```

## Customization Examples

### Adding a New Menu Item

```cpp
// 1. Create the action function
void myCustomAction() {
  Serial.println("Custom action triggered!");
  // Add your logic here
}

// 2. Add to menu definition
menuDef myMenu[] = {
  {"My Custom Item", myCustomAction, nullptr},
  {nullptr, nullptr, nullptr}
};
```

### Creating Multi-Level Menus

```cpp
// Level 3 (deepest)
menuDef level3[] = {
  {"Level 3 Item", myAction, nullptr},
  {nullptr, nullptr, nullptr}
};

// Level 2
menuDef level2[] = {
  {"Go Deeper", nullptr, level3},
  {"Level 2 Action", myAction, nullptr},
  {nullptr, nullptr, nullptr}
};

// Level 1 (top)
menuDef level1[] = {
  {"Enter Level 2", nullptr, level2},
  {nullptr, nullptr, nullptr}
};
```

## Troubleshooting

### Display Not Working

1. **Check I2C Address**: Some displays use `0x3D` instead of `0x3C`
   ```cpp
   #define SCREEN_ADDRESS 0x3D  // Try this if 0x3C doesn't work
   ```

2. **Verify Wiring**: Double-check SDA/SCL connections

3. **I2C Scanner**: Run an I2C scanner sketch to detect the display address

### Serial Monitor Issues

1. **Baud Rate**: Ensure Serial Monitor is set to `9600` baud
2. **Line Ending**: Set line ending to "Newline" or "Both NL & CR"
3. **Arrow Keys**: Some serial monitors may not support arrow keys; try a different terminal emulator

### Memory Issues

If you get "Memory allocation failed":
- Reduce the number of menu items
- Simplify menu structure
- Check for memory leaks in action functions

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

### Development Guidelines

1. Follow the existing code style
2. Test on actual hardware before submitting
3. Update documentation for new features
4. Keep memory usage minimal

## Project Structure

```
esp32Menu/
├── src/
│   └── main.cpp          # Main application code
├── include/              # Header files (if any)
├── lib/                  # Private libraries
├── test/                 # Test files
├── platformio.ini        # PlatformIO configuration
└── README.md            # This file
```

## License

This project is open source. Please check the repository for license information.

## Acknowledgments

- ThingPulse for the excellent ESP8266/ESP32 OLED library
- Arduino and ESP32 communities for their support and resources

## Contact

For issues, questions, or contributions, please visit:
- **GitHub**: [404Mayank/esp32Menu](https://github.com/404Mayank/esp32Menu)

---

Made with ❤️ for the ESP32 community
