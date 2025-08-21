# Required Libraries Setup
## Arduino IDE Library Installation

### 1. USB Host Shield 2.0 Library
This library is required for Xbox 360 controller communication.

#### Installation via Arduino IDE:
1. Open Arduino IDE
2. Go to **Sketch** → **Include Library** → **Manage Libraries**
3. Search for "USB Host Shield Library 2.0"
4. Install the library by Oleg Mazurov

#### Manual Installation:
1. Download from: https://github.com/felis/USB_Host_Shield_2.0
2. Extract to Arduino/libraries/ folder
3. Restart Arduino IDE

### 2. Required Library Files
The main libraries needed in your sketch:
```cpp
#include <USB_Host_Shield_2.0.h>
#include <XBOXRECV.h>
```

### 3. Library Dependencies
The USB Host Shield 2.0 library includes:
- SPI communication for the shield
- Xbox 360 controller protocols
- USB host functionality

### 4. Board Configuration
#### Arduino IDE Settings:
- **Board**: Arduino Mega or Mega 2560
- **Processor**: ATmega2560 (Mega 2560)
- **Port**: Select appropriate COM port
- **Programmer**: AVRISP mkII (or Arduino as ISP)

### 5. Testing Library Installation
Create a simple test sketch to verify library installation:

```cpp
#include <USB_Host_Shield_2.0.h>
#include <XBOXRECV.h>

USB usb;
XBOXRECV Xbox(&usb);

void setup() {
  Serial.begin(115200);
  if (usb.Init() == -1) {
    Serial.print(F("OSC did not start"));
    while (1);
  }
  Serial.print(F("Xbox Wireless Receiver Library Test"));
}

void loop() {
  usb.Task();
  
  if (Xbox.XboxReceiverConnected) {
    Serial.println("Xbox Receiver Connected!");
    for (uint8_t i = 0; i < 4; i++) {
      if (Xbox.Xbox360Connected[i]) {
        Serial.print("Controller ");
        Serial.print(i);
        Serial.println(" connected!");
      }
    }
  }
  delay(1000);
}
```

### 6. Common Issues and Solutions

#### "USB_Host_Shield_2.0.h: No such file or directory"
- Ensure library is properly installed in Arduino/libraries/ folder
- Restart Arduino IDE after installation
- Check library folder name (should not have spaces or special characters)

#### "XBOXRECV.h: No such file or directory"  
- This file is included with USB Host Shield 2.0 library
- Re-install the library if missing
- Check that you're using the correct version (2.0)

#### USB Host Shield Not Detected
- Verify shield is properly seated on Arduino Mega
- Check for bent pins or poor connections
- Test with known working USB device

#### Controller Won't Connect
- Ensure Xbox 360 Wireless Receiver is connected to shield
- Press sync button on receiver, then on controller
- Check that controller batteries are charged
- Try different controller if available

### 7. Alternative Libraries (if needed)

#### For Wired Xbox 360 Controller:
```cpp
#include <XBOXUSB.h>
// Use XBOXUSB instead of XBOXRECV for wired controllers
```

#### For Different Controller Types:
```cpp
#include <PS3USB.h>     // For PlayStation 3 controllers
#include <PS4USB.h>     // For PlayStation 4 controllers
```

### 8. Memory Considerations
Arduino Mega has sufficient memory for this project:
- Flash Memory: 256KB (plenty for code)
- SRAM: 8KB (sufficient for variables)
- EEPROM: 4KB (for persistent settings if needed)

### 9. Compilation Flags (if needed)
Add to your sketch if you encounter memory issues:
```cpp
// Reduce memory usage
#define ENABLE_UHS_DEBUGGING 0
#define DEBUG_USB_HOST
```