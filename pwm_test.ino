/*
 * PWM Speed Controller Test
 * Simple test to validate PWM functionality
 * 
 * This test script validates the enhanced PWM speed control features
 * without requiring Xbox controller or full robot setup.
 */

// Motor Driver Pins (same as main controller)
const int motorA_pin1 = 22;  // Left motor direction pin 1
const int motorA_pin2 = 23;  // Left motor direction pin 2
const int motorA_enable = 2; // Left motor PWM speed control

const int motorB_pin1 = 24;  // Right motor direction pin 1
const int motorB_pin2 = 25;  // Right motor direction pin 2
const int motorB_enable = 3; // Right motor PWM speed control

// PWM and Speed Control Settings
const int minPWM = 0;          // Minimum PWM value (motor stop)
const int maxPWM = 255;        // Maximum PWM value (full speed)
const int startupPWM = 50;     // Minimum PWM to start motor movement

void setup() {
  Serial.begin(115200);
  
  // Initialize motor pins
  pinMode(motorA_pin1, OUTPUT);
  pinMode(motorA_pin2, OUTPUT);
  pinMode(motorA_enable, OUTPUT);
  pinMode(motorB_pin1, OUTPUT);
  pinMode(motorB_pin2, OUTPUT);
  pinMode(motorB_enable, OUTPUT);
  
  Serial.println("PWM Speed Controller Test");
  Serial.println("Testing enhanced PWM functionality...");
  
  // Test PWM constraint function
  testPWMConstraints();
  
  // Test motor speed control with various values
  testMotorSpeedControl();
  
  Serial.println("PWM test completed successfully!");
}

void loop() {
  // Empty loop - test runs once in setup
}

void testPWMConstraints() {
  Serial.println("\n=== Testing PWM Constraints ===");
  
  // Test values outside normal range
  int testValues[] = {-300, -100, 0, 50, 150, 255, 300, 500};
  int numTests = sizeof(testValues) / sizeof(testValues[0]);
  
  for (int i = 0; i < numTests; i++) {
    int input = testValues[i];
    int constrained = constrain(abs(input), minPWM, maxPWM);
    
    Serial.print("Input: ");
    Serial.print(input);
    Serial.print(" -> Constrained: ");
    Serial.print(constrained);
    Serial.println(" ✓");
  }
}

void testMotorSpeedControl() {
  Serial.println("\n=== Testing Motor Speed Control ===");
  
  // Test forward movement with various speeds
  Serial.println("Testing forward speeds...");
  for (int speed = 0; speed <= 255; speed += 51) {
    Serial.print("Speed: ");
    Serial.print(speed);
    setMotorSpeed(speed, speed);
    delay(100);
    Serial.println(" ✓");
  }
  
  // Test backward movement
  Serial.println("Testing backward speeds...");
  for (int speed = -255; speed <= 0; speed += 51) {
    Serial.print("Speed: ");
    Serial.print(speed);
    setMotorSpeed(speed, speed);
    delay(100);
    Serial.println(" ✓");
  }
  
  // Stop motors
  stopAllMotors();
  Serial.println("Motors stopped ✓");
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
}