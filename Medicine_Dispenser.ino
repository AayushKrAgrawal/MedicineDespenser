#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

Servo motor1;
Servo motor2;
Servo motor3;
Servo motor4;

// Current position of each motor (initially set to 0 degrees)
int position1 = 0;
int position2 = 0;
int position3 = 0;
int position4 = 0;

// Incremental rotation angle
const int rotationIncrement = 45;

// Pin definitions for LEDs
const int led1Pin = 10;
const int led2Pin = 11;
const int led3Pin = 12;
const int led4Pin = 13;

// Initialize the LCD with I2C address 0x27 (change if needed)
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);   // Start serial communication at 9600 baud rate

  motor1.attach(2);     // Attach motor 1 to pin 2
  motor2.attach(3);     // Attach motor 2 to pin 3
  motor3.attach(4);     // Attach motor 3 to pin 4
  motor4.attach(5);     // Attach motor 4 to pin 5

  // Set all motors to their initial position
  motor1.write(0);
  motor2.write(0);
  motor3.write(0);
  motor4.write(0);

  // Set LED pins as OUTPUT
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(led3Pin, OUTPUT);
  pinMode(led4Pin, OUTPUT);

  // Initialize the LCD
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Medicine Dispenser");
}

void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read();  // Read the incoming byte

    // Check if the command is for a motor and read name if available
    String name = "";
    if (Serial.available() > 0) {
      name = Serial.readStringUntil('\n');  // Read the name until newline
    }

    switch (command) {
      case '1':
        rotateMotor(motor1, led1Pin, position1, name);
        break;
      case '2':
        rotateMotor(motor2, led2Pin, position2, name);
        break;
      case '3':
        rotateMotor(motor3, led3Pin, position3, name);
        break;
      case '4':
        rotateMotor(motor4, led4Pin, position4, name);
        break;
      default:
        break; // Unknown command
    }
  }
}

void rotateMotor(Servo &motor, int ledPin, int &position, String name) {
  // Update the position
  position += rotationIncrement;
  
  // Ensure the position does not exceed 180 degrees
  if (position > 180) {
    position = 180;  // Cap at 180 degrees
  }

  motor.write(position);  // Rotate to the updated position
  digitalWrite(ledPin, HIGH);   // Turn on the corresponding LED
  delay(1000);                  // Wait for the motor to reach the position
  digitalWrite(ledPin, LOW);    // Turn off the LED

  // Display the name on the LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Dispensing for:");
  lcd.setCursor(0, 1);
  lcd.print(name);  // Print the person's name
}
