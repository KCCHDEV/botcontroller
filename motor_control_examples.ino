/*
 * Motor Control Examples and Test Functions
 * For Robot Controller System with Arduino Mega
 * 
 * This file contains various motor control functions and examples
 * Use these functions to test your motor setup and understand movement patterns
 */

// Motor Driver Pins (same as main controller)
const int motorA_pin1 = 22;  // Left motor direction pin 1
const int motorA_pin2 = 23;  // Left motor direction pin 2
const int motorA_enable = 2; // Left motor PWM speed control

const int motorB_pin1 = 24;  // Right motor direction pin 1
const int motorB_pin2 = 25;  // Right motor direction pin 2
const int motorB_enable = 3; // Right motor PWM speed control

void setup() {
  Serial.begin(115200);
  
  // Initialize motor pins
  pinMode(motorA_pin1, OUTPUT);
  pinMode(motorA_pin2, OUTPUT);
  pinMode(motorA_enable, OUTPUT);
  pinMode(motorB_pin1, OUTPUT);
  pinMode(motorB_pin2, OUTPUT);
  pinMode(motorB_enable, OUTPUT);
  
  Serial.println("Motor Control Test System Ready");
  Serial.println("Available commands:");
  Serial.println("1 - Move Forward");
  Serial.println("2 - Move Backward");
  Serial.println("3 - Turn Left");
  Serial.println("4 - Turn Right");
  Serial.println("5 - Spin Left");
  Serial.println("6 - Spin Right");
  Serial.println("7 - Figure 8 Pattern");
  Serial.println("8 - Speed Test");
  Serial.println("0 - Stop Motors");
  Serial.println("Enter command:");
}

void loop() {
  if (Serial.available()) {
    char command = Serial.read();
    
    switch (command) {
      case '1':
        moveForward(200, 2000);
        break;
      case '2':
        moveBackward(200, 2000);
        break;
      case '3':
        turnLeft(150, 1000);
        break;
      case '4':
        turnRight(150, 1000);
        break;
      case '5':
        spinLeft(180, 1500);
        break;
      case '6':
        spinRight(180, 1500);
        break;
      case '7':
        figure8Pattern();
        break;
      case '8':
        speedTest();
        break;
      case '0':
        stopMotors();
        Serial.println("Motors stopped");
        break;
      default:
        Serial.println("Invalid command");
        break;
    }
  }
}

// Basic Movement Functions

void moveForward(int speed, int duration) {
  Serial.println("Moving forward...");
  setMotorSpeeds(speed, speed);
  delay(duration);
  stopMotors();
}

void moveBackward(int speed, int duration) {
  Serial.println("Moving backward...");
  setMotorSpeeds(-speed, -speed);
  delay(duration);
  stopMotors();
}

void turnLeft(int speed, int duration) {
  Serial.println("Turning left...");
  setMotorSpeeds(speed/2, speed);  // Left motor slower
  delay(duration);
  stopMotors();
}

void turnRight(int speed, int duration) {
  Serial.println("Turning right...");
  setMotorSpeeds(speed, speed/2);  // Right motor slower
  delay(duration);
  stopMotors();
}

void spinLeft(int speed, int duration) {
  Serial.println("Spinning left...");
  setMotorSpeeds(-speed, speed);  // Left backward, right forward
  delay(duration);
  stopMotors();
}

void spinRight(int speed, int duration) {
  Serial.println("Spinning right...");
  setMotorSpeeds(speed, -speed);  // Left forward, right backward
  delay(duration);
  stopMotors();
}

// Advanced Movement Patterns

void figure8Pattern() {
  Serial.println("Executing figure 8 pattern...");
  
  // First loop
  for (int i = 0; i < 4; i++) {
    setMotorSpeeds(200, 100);  // Right turn
    delay(500);
  }
  
  // Transition
  setMotorSpeeds(150, 150);
  delay(300);
  
  // Second loop (opposite direction)
  for (int i = 0; i < 4; i++) {
    setMotorSpeeds(100, 200);  // Left turn
    delay(500);
  }
  
  stopMotors();
  Serial.println("Figure 8 complete");
}

void speedTest() {
  Serial.println("Speed test starting...");
  
  for (int speed = 50; speed <= 255; speed += 50) {
    Serial.print("Testing speed: ");
    Serial.println(speed);
    
    setMotorSpeeds(speed, speed);
    delay(1000);
    stopMotors();
    delay(500);
  }
  
  Serial.println("Speed test complete");
}

void gradualAcceleration(int maxSpeed, int duration) {
  Serial.println("Gradual acceleration...");
  
  int steps = 20;
  int stepDelay = duration / steps;
  
  for (int i = 0; i <= steps; i++) {
    int currentSpeed = (maxSpeed * i) / steps;
    setMotorSpeeds(currentSpeed, currentSpeed);
    delay(stepDelay);
  }
  
  // Gradual deceleration
  for (int i = steps; i >= 0; i--) {
    int currentSpeed = (maxSpeed * i) / steps;
    setMotorSpeeds(currentSpeed, currentSpeed);
    delay(stepDelay);
  }
  
  stopMotors();
}

void zigzagPattern() {
  Serial.println("Zigzag pattern...");
  
  for (int i = 0; i < 5; i++) {
    // Move forward and right
    setMotorSpeeds(200, 150);
    delay(800);
    
    // Move forward and left
    setMotorSpeeds(150, 200);
    delay(800);
  }
  
  stopMotors();
  Serial.println("Zigzag complete");
}

// Core Motor Control Functions

void setMotorSpeeds(int leftSpeed, int rightSpeed) {
  // Left Motor Control
  if (leftSpeed > 0) {
    digitalWrite(motorA_pin1, HIGH);
    digitalWrite(motorA_pin2, LOW);
  } else if (leftSpeed < 0) {
    digitalWrite(motorA_pin1, LOW);
    digitalWrite(motorA_pin2, HIGH);
    leftSpeed = -leftSpeed;
  } else {
    digitalWrite(motorA_pin1, LOW);
    digitalWrite(motorA_pin2, LOW);
  }
  analogWrite(motorA_enable, constrain(leftSpeed, 0, 255));
  
  // Right Motor Control
  if (rightSpeed > 0) {
    digitalWrite(motorB_pin1, HIGH);
    digitalWrite(motorB_pin2, LOW);
  } else if (rightSpeed < 0) {
    digitalWrite(motorB_pin1, LOW);
    digitalWrite(motorB_pin2, HIGH);
    rightSpeed = -rightSpeed;
  } else {
    digitalWrite(motorB_pin1, LOW);
    digitalWrite(motorB_pin2, LOW);
  }
  analogWrite(motorB_enable, constrain(rightSpeed, 0, 255));
}

void stopMotors() {
  digitalWrite(motorA_pin1, LOW);
  digitalWrite(motorA_pin2, LOW);
  digitalWrite(motorB_pin1, LOW);
  digitalWrite(motorB_pin2, LOW);
  analogWrite(motorA_enable, 0);
  analogWrite(motorB_enable, 0);
}

void emergencyBrake() {
  // Quick stop by setting both direction pins HIGH (brake mode for some drivers)
  digitalWrite(motorA_pin1, HIGH);
  digitalWrite(motorA_pin2, HIGH);
  digitalWrite(motorB_pin1, HIGH);
  digitalWrite(motorB_pin2, HIGH);
  analogWrite(motorA_enable, 255);
  analogWrite(motorB_enable, 255);
  delay(100);
  stopMotors();
}

// Motor Calibration Functions

void calibrateMotors() {
  Serial.println("Motor calibration starting...");
  
  Serial.println("Testing left motor forward...");
  setMotorSpeeds(150, 0);
  delay(2000);
  stopMotors();
  delay(1000);
  
  Serial.println("Testing left motor backward...");
  setMotorSpeeds(-150, 0);
  delay(2000);
  stopMotors();
  delay(1000);
  
  Serial.println("Testing right motor forward...");
  setMotorSpeeds(0, 150);
  delay(2000);
  stopMotors();
  delay(1000);
  
  Serial.println("Testing right motor backward...");
  setMotorSpeeds(0, -150);
  delay(2000);
  stopMotors();
  delay(1000);
  
  Serial.println("Testing both motors forward...");
  setMotorSpeeds(150, 150);
  delay(2000);
  stopMotors();
  
  Serial.println("Calibration complete");
}

void motorSpeedComparison() {
  Serial.println("Comparing motor speeds...");
  
  int testSpeed = 150;
  
  Serial.println("Both motors at same speed - check if robot moves straight");
  setMotorSpeeds(testSpeed, testSpeed);
  delay(3000);
  stopMotors();
  
  Serial.println("If robot veered, adjust motor speeds in main code");
  Serial.println("Left motor dominant: reduce left speed or increase right speed");
  Serial.println("Right motor dominant: reduce right speed or increase left speed");
}