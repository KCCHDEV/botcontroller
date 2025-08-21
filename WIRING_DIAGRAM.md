# Robot Controller Wiring Diagram
## Arduino Mega + Xbox 360 Controller + Motor Driver

### Components Required:
- Arduino Mega 2560
- USB Host Shield 2.0
- Xbox 360 Wireless Receiver
- L298N Motor Driver (or similar dual motor driver)
- 2x DC Motors (for robot wheels)
- 12V Battery Pack
- LEDs for status indicators
- Resistors (220Ω for LEDs)
- Jumper wires
- Breadboard (optional)

### Pin Connections:

#### USB Host Shield to Arduino Mega:
```
USB Host Shield    →    Arduino Mega
VCC               →    5V
GND               →    GND
D+                →    Digital Pin (handled by shield)
D-                →    Digital Pin (handled by shield)
```

#### L298N Motor Driver to Arduino Mega:
```
L298N Pin         →    Arduino Mega Pin    →    Description
IN1               →    Digital Pin 22      →    Left Motor Direction 1
IN2               →    Digital Pin 23      →    Left Motor Direction 2
IN3               →    Digital Pin 24      →    Right Motor Direction 1
IN4               →    Digital Pin 25      →    Right Motor Direction 2
ENA               →    PWM Pin 2          →    Left Motor Speed Control
ENB               →    PWM Pin 3          →    Right Motor Speed Control
VCC               →    5V                 →    Logic Power
GND               →    GND                →    Ground
12V               →    12V Battery+       →    Motor Power
GND               →    12V Battery-       →    Motor Power Ground
```

#### Motor Connections to L298N:
```
L298N Terminal    →    Connection
Motor A+          →    Left Motor Positive
Motor A-          →    Left Motor Negative
Motor B+          →    Right Motor Positive
Motor B-          →    Right Motor Negative
```

#### LED Status Indicators:
```
LED Purpose       →    Arduino Pin    →    Connection
Connection Status →    Pin 13         →    Built-in LED
Power Indicator   →    Pin 12         →    LED + 220Ω resistor to GND
Emergency Stop    →    Pin 11         →    LED + 220Ω resistor to GND
```

#### Power Distribution:
```
Power Source      →    Voltage    →    Connection
12V Battery       →    12V        →    L298N Motor Power + VIN (optional)
Arduino USB       →    5V         →    Arduino Power (for programming)
Arduino 5V        →    5V         →    USB Host Shield, LED resistors
```

### Wiring Steps:

1. **Mount USB Host Shield on Arduino Mega**
   - Carefully align and press the shield onto the Arduino Mega pins
   - Ensure all pins are properly connected

2. **Connect Motor Driver (L298N)**
   - Connect IN1-IN4 to digital pins 22-25
   - Connect ENA, ENB to PWM pins 2, 3
   - Connect VCC to 5V, GND to GND
   - Connect 12V power supply to motor power terminals

3. **Connect Motors**
   - Connect left motor to Motor A terminals
   - Connect right motor to Motor B terminals
   - Test motor direction and swap wires if needed

4. **Connect Status LEDs**
   - Power LED: Pin 12 → 220Ω resistor → LED → GND
   - Emergency LED: Pin 11 → 220Ω resistor → LED → GND
   - Built-in LED (Pin 13) shows connection status

5. **Power Connections**
   - Connect 12V battery pack to L298N motor power
   - Arduino can be powered via USB (programming) or VIN pin (12V)
   - Ensure common ground between all components

### Safety Notes:
- Always disconnect power when wiring
- Double-check polarity on battery connections
- Use appropriate fuses for motor power
- Test with low voltage first
- Ensure proper heat dissipation for motor driver

### Xbox 360 Controller Setup:
- Plug Xbox 360 Wireless Receiver into USB Host Shield
- Pair Xbox 360 controller with receiver (press sync buttons)
- Controller should connect automatically when turned on

### Troubleshooting:
- If motors don't move: Check power connections and motor driver wiring
- If controller doesn't connect: Verify USB Host Shield installation
- If robot moves in wrong direction: Swap motor wire polarity
- If speed control doesn't work: Check PWM pin connections (ENA, ENB)