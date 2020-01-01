/*
 * Project Name: 2 Factor Door Lock
 * Developed by: Sashwat K
 * Created on: 1 Jan 2020
 * Last Modified: 1 Jan 2020
 * Revision: 1
*/

#include <LiquidCrystal_I2C.h> // LCD Library
#include <Adafruit_Fingerprint.h> // R307 Library
#include <Keypad.h> // Keypad Library


LiquidCrystal_I2C lcd(0x3F,20,4); // LCD Initialisation

// R307 initialisation
SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
uint8_t id;

// Keypad initialisation
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {10, 9, 8, 7}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {6,5,4}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
