/*
 * Robot Controller System with Enhanced PWM Speed Control
 * Arduino Mega + Xbox 360 Controller
 * 
 * This code controls a robot car using Xbox 360 controller input
 * Features:
 * - Enhanced PWM motor control with proper speed constraints
 * - Smooth acceleration/deceleration functions
 * - Configurable PWM frequency for optimal motor performance
 * - Motor control for movement (forward, backward, left, right)
 * - Speed control using analog sticks with deadzone
 * - Emergency stop functionality
 * - LED indicators for status
 */

#include <USB_Host_Shield_2.0.h>
#include <XBOXRECV.h>

// USB Host Shield and Xbox receiver
USB usb;
XBOXRECV Xbox(&usb);

// Motor Driver Pins (L298N or similar)
const int motorA_pin1 = 22;  // Left motor direction pin 1
const int motorA_pin2 = 23;  // Left motor direction pin 2
const int motorA_enable = 2; // Left motor PWM speed control

const int motorB_pin1 = 24;  // Right motor direction pin 1
const int motorB_pin2 = 25;  // Right motor direction pin 2
const int motorB_enable = 3; // Right motor PWM speed control

// LED Status Indicators
const int ledConnected = 13;  // Built-in LED for connection status
const int ledPower = 12;      // Power indicator LED
const int ledEmergency = 11;  // Emergency stop LED

// Robot Control Variables
int leftMotorSpeed = 0;
int rightMotorSpeed = 0;
bool emergencyStop = false;
bool robotEnabled = false;

// Controller deadzone (to prevent drift)
const int deadzone = 20;

// PWM and Speed Control Settings
const int minPWM = 0;          // Minimum PWM value (motor stop)
const int maxPWM = 255;        // Maximum PWM value (full speed)
const int startupPWM = 50;     // Minimum PWM to start motor movement

// PWM Frequency Control (optional - for advanced users)
void setPWMFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 2 || pin == 3 || pin == 5 || pin == 6 || pin == 7 || pin == 8) {
    switch(divisor) {
      case 1: mode = 0x01; break;    // 31372.55 Hz
      case 8: mode = 0x02; break;    // 3921.16 Hz
      case 64: mode = 0x03; break;   // 490.20 Hz (default)
      case 256: mode = 0x04; break;  // 122.55 Hz
      case 1024: mode = 0x05; break; // 30.64 Hz
      default: return;
    }
    if(pin == 2 || pin == 3) {
      TCCR3B = TCCR3B & 0b11111000 | mode;
    }
  }
}

void setup() {
  Serial.begin(115200);
  
  // Initialize USB Host Shield
  if (usb.Init() == -1) {
    Serial.print(F("OSC did not start"));
    while (1); // Halt
  }
  Serial.print(F("Xbox Wireless Receiver Library Started"));
  
  // Initialize motor pins
  pinMode(motorA_pin1, OUTPUT);
  pinMode(motorA_pin2, OUTPUT);
  pinMode(motorA_enable, OUTPUT);
  pinMode(motorB_pin1, OUTPUT);
  pinMode(motorB_pin2, OUTPUT);
  pinMode(motorB_enable, OUTPUT);
  
  // Set PWM frequency for smoother motor control (optional)
  // Uncomment the lines below to use higher frequency PWM for quieter operation
  // setPWMFrequency(motorA_enable, 8);  // ~3.9kHz instead of default 490Hz
  // setPWMFrequency(motorB_enable, 8);  // ~3.9kHz instead of default 490Hz
  
  // Initialize LED pins
  pinMode(ledConnected, OUTPUT);
  pinMode(ledPower, OUTPUT);
  pinMode(ledEmergency, OUTPUT);
  
  // Initial state
  digitalWrite(ledPower, HIGH);  // Power on indicator
  stopAllMotors();
  
  Serial.println("Robot Controller System Ready");
  Serial.println("Waiting for Xbox 360 controller connection...");
}

void loop() {
  usb.Task();
  
  // Check if Xbox controller is connected
  if (Xbox.XboxReceiverConnected) {
    digitalWrite(ledConnected, HIGH);
    
    for (uint8_t i = 0; i < 4; i++) {
      if (Xbox.Xbox360Connected[i]) {
        handleXboxController(i);
        break; // Use first connected controller
      }
    }
  } else {
    digitalWrite(ledConnected, LOW);
    robotEnabled = false;
    stopAllMotors();
  }
  
  // Update emergency stop LED
  digitalWrite(ledEmergency, emergencyStop ? HIGH : LOW);
  
  delay(20); // Small delay for stability
}

void handleXboxController(uint8_t controller) {
  // Emergency Stop - Back button
  if (Xbox.getButtonClick(BACK, controller)) {
    emergencyStop = !emergencyStop;
    Serial.println(emergencyStop ? "EMERGENCY STOP ACTIVATED" : "Emergency stop deactivated");
    if (emergencyStop) {
      stopAllMotors();
      return;
    }
  }
  
  // Enable/Disable Robot - Start button
  if (Xbox.getButtonClick(START, controller)) {
    robotEnabled = !robotEnabled;
    Serial.println(robotEnabled ? "Robot ENABLED" : "Robot DISABLED");
    if (!robotEnabled) {
      stopAllMotors();
      return;
    }
  }
  
  // Only process movement if robot is enabled and not in emergency stop
  if (!robotEnabled || emergencyStop) {
    stopAllMotors();
    return;
  }
  
  // Get analog stick values
  int leftStickY = Xbox.getAnalogHat(LeftHatY, controller);
  int leftStickX = Xbox.getAnalogHat(LeftHatX, controller);
  int rightStickY = Xbox.getAnalogHat(RightHatY, controller);
  
  // Convert from Xbox range (-32768 to 32767) to motor range (-255 to 255)
  int forwardBack = map(leftStickY, -32768, 32767, 255, -255);
  int leftRight = map(leftStickX, -32768, 32767, -255, 255);
  int rotation = map(rightStickY, -32768, 32767, 255, -255);
  
  // Apply deadzone
  if (abs(forwardBack) < deadzone) forwardBack = 0;
  if (abs(leftRight) < deadzone) leftRight = 0;
  if (abs(rotation) < deadzone) rotation = 0;
  
  // Tank drive calculation
  if (abs(rotation) > 10) {
    // Pure rotation mode using right stick
    leftMotorSpeed = rotation;
    rightMotorSpeed = -rotation;
  } else {
    // Normal driving mode using left stick
    leftMotorSpeed = constrain(forwardBack + leftRight, -255, 255);
    rightMotorSpeed = constrain(forwardBack - leftRight, -255, 255);
  }
  
  // Speed boost with triggers
  int speedBoost = 0;
  if (Xbox.getButtonPress(RT, controller)) {
    speedBoost = map(Xbox.getButtonPress(RT, controller), 0, 255, 0, 100);
  }
  
  // Apply speed boost
  if (speedBoost > 0) {
    leftMotorSpeed = constrain(leftMotorSpeed * (100 + speedBoost) / 100, -255, 255);
    rightMotorSpeed = constrain(rightMotorSpeed * (100 + speedBoost) / 100, -255, 255);
  }
  
  // Precision mode with left trigger (slower movement)
  if (Xbox.getButtonPress(LT, controller)) {
    int precisionFactor = map(Xbox.getButtonPress(LT, controller), 0, 255, 50, 100);
    leftMotorSpeed = leftMotorSpeed * precisionFactor / 100;
    rightMotorSpeed = rightMotorSpeed * precisionFactor / 100;
  }
  
  // Apply motor speeds
  setMotorSpeed(leftMotorSpeed, rightMotorSpeed);
  
  // Debug output
  if (abs(leftMotorSpeed) > 0 || abs(rightMotorSpeed) > 0) {
    Serial.print("Left: ");
    Serial.print(leftMotorSpeed);
    Serial.print(" Right: ");
    Serial.println(rightMotorSpeed);
  }
  
  // Handle button presses for additional features
  handleButtonPresses(controller);
}

void setMotorSpeed(int leftSpeed, int rightSpeed) {
  // Left Motor Control
  if (leftSpeed > 0) {
    digitalWrite(motorA_pin1, HIGH);
    digitalWrite(motorA_pin2, LOW);
    // Apply minimum PWM for reliable motor startup
    leftSpeed = max(leftSpeed, startupPWM);
  } else if (leftSpeed < 0) {
    digitalWrite(motorA_pin1, LOW);
    digitalWrite(motorA_pin2, HIGH);
    leftSpeed = -leftSpeed;
    // Apply minimum PWM for reliable motor startup
    leftSpeed = max(leftSpeed, startupPWM);
  } else {
    digitalWrite(motorA_pin1, LOW);
    digitalWrite(motorA_pin2, LOW);
  }
  analogWrite(motorA_enable, constrain(leftSpeed, minPWM, maxPWM));
  
  // Right Motor Control
  if (rightSpeed > 0) {
    digitalWrite(motorB_pin1, HIGH);
    digitalWrite(motorB_pin2, LOW);
    // Apply minimum PWM for reliable motor startup
    rightSpeed = max(rightSpeed, startupPWM);
  } else if (rightSpeed < 0) {
    digitalWrite(motorB_pin1, LOW);
    digitalWrite(motorB_pin2, HIGH);
    rightSpeed = -rightSpeed;
    // Apply minimum PWM for reliable motor startup
    rightSpeed = max(rightSpeed, startupPWM);
  } else {
    digitalWrite(motorB_pin1, LOW);
    digitalWrite(motorB_pin2, LOW);
  }
  analogWrite(motorB_enable, constrain(rightSpeed, minPWM, maxPWM));
}

void stopAllMotors() {
  digitalWrite(motorA_pin1, LOW);
  digitalWrite(motorA_pin2, LOW);
  digitalWrite(motorB_pin1, LOW);
  digitalWrite(motorB_pin2, LOW);
  analogWrite(motorA_enable, 0);
  analogWrite(motorB_enable, 0);
  leftMotorSpeed = 0;
  rightMotorSpeed = 0;
}

// Smoothed motor speed control for gradual acceleration/deceleration
void setMotorSpeedSmooth(int targetLeftSpeed, int targetRightSpeed, int accelerationRate = 10) {
  static int currentLeftSpeed = 0;
  static int currentRightSpeed = 0;
  
  // Gradually adjust current speeds toward target speeds
  if (currentLeftSpeed < targetLeftSpeed) {
    currentLeftSpeed = min(currentLeftSpeed + accelerationRate, targetLeftSpeed);
  } else if (currentLeftSpeed > targetLeftSpeed) {
    currentLeftSpeed = max(currentLeftSpeed - accelerationRate, targetLeftSpeed);
  }
  
  if (currentRightSpeed < targetRightSpeed) {
    currentRightSpeed = min(currentRightSpeed + accelerationRate, targetRightSpeed);
  } else if (currentRightSpeed > targetRightSpeed) {
    currentRightSpeed = max(currentRightSpeed - accelerationRate, targetRightSpeed);
  }
  
  // Apply the smoothed speeds
  setMotorSpeed(currentLeftSpeed, currentRightSpeed);
}

void handleButtonPresses(uint8_t controller) {
  // Quick stop with B button
  if (Xbox.getButtonClick(B, controller)) {
    stopAllMotors();
    Serial.println("Quick stop activated");
  }
  
  // Preset movements with D-pad
  if (Xbox.getButtonClick(UP, controller)) {
    Serial.println("Forward preset");
    setMotorSpeed(200, 200);
    delay(500);
    stopAllMotors();
  }
  
  if (Xbox.getButtonClick(DOWN, controller)) {
    Serial.println("Backward preset");
    setMotorSpeed(-200, -200);
    delay(500);
    stopAllMotors();
  }
  
  if (Xbox.getButtonClick(LEFT, controller)) {
    Serial.println("Left turn preset");
    setMotorSpeed(-150, 150);
    delay(300);
    stopAllMotors();
  }
  
  if (Xbox.getButtonClick(RIGHT, controller)) {
    Serial.println("Right turn preset");
    setMotorSpeed(150, -150);
    delay(300);
    stopAllMotors();
  }
  
  // Horn/Buzzer with A button
  if (Xbox.getButtonPress(A, controller)) {
    // Add buzzer code here if you have one connected
    Serial.println("Horn activated");
  }
  
  // Status report with Y button
  if (Xbox.getButtonClick(Y, controller)) {
    printStatus();
  }
}

void printStatus() {
  Serial.println("=== Robot Status ===");
  Serial.print("Robot Enabled: ");
  Serial.println(robotEnabled ? "YES" : "NO");
  Serial.print("Emergency Stop: ");
  Serial.println(emergencyStop ? "ACTIVE" : "INACTIVE");
  Serial.print("Left Motor Speed: ");
  Serial.println(leftMotorSpeed);
  Serial.print("Right Motor Speed: ");
  Serial.println(rightMotorSpeed);
  Serial.print("Controller Connected: ");
  Serial.println(Xbox.XboxReceiverConnected ? "YES" : "NO");
  Serial.println("==================");
}