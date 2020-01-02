/*
                              $$\                            $$$$$$\  
                              $$ |                          $$  __$$\ 
  $$$$$$$\  $$$$$$\   $$$$$$$\ $$$$$$$\  $$\   $$\ $$\   $$\ $$ /  \__|
$$  _____| \____$$\ $$  _____|$$  __$$\ $$ |  $$ |$$ |  $$ |$$$$$$$\  
\$$$$$$\   $$$$$$$ |\$$$$$$\  $$ |  $$ |$$ |  $$ |$$ |  $$ |$$  __$$\ 
  \____$$\ $$  __$$ | \____$$\ $$ |  $$ |$$ |  $$ |$$ |  $$ |$$ /  $$ |
$$$$$$$  |\$$$$$$$ |$$$$$$$  |$$ |  $$ |\$$$$$$  |\$$$$$$  | $$$$$$  |
\_______/  \_______|\_______/ \__|  \__| \______/  \______/  \______/

 * Project Name: 2 Factor Door Lock
 * Repository link: https://github.com/sashuu6/2-factor-door-lock
 * Developed by: Sashwat K
 * Created on: 1 Jan 2020
 * Last Modified: 2 Jan 2020
 * Revision: 2
*/

#include <LiquidCrystal_I2C.h> // LCD Library
#include <Adafruit_Fingerprint.h> // R307 Library
#include <Keypad.h> // Keypad Library


LiquidCrystal_I2C lcd(0x3F,20,4); // LCD

// R307 fingerprint sensor
SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
uint8_t id;

// Keypad 4 * 3
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

// Functions
void dashboard();
uint8_t resetFingerprint();
int verifyFingerprint();

long passCode = 0;
int temp,n;
long correctPassCode = 963258;
long resetPin = 742690;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //Serial printing
  Serial.println("2-Factor Door lock");
  Serial.println("By Sashwat K");
  Serial.println("Checkut www.sashwat.in");
  
  //Welcome Screen
  lcd.init();
  lcd.backlight();
  lcd.setCursor(1,0);
  lcd.print("2-Factor");
  lcd.setCursor(6,1);
  lcd.print("Door Lock");
  delay(1000);
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("By,");
  lcd.setCursor(6,1);
  lcd.print("Sashwat K");
  delay(1000);

  // Initialising fingerprint sensor
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Booting sensors");
  lcd.setCursor(0,1);
  lcd.print("Please Wait...");
  finger.begin(57600);
  delay(500);

  // Verifying Fingerprint Sensr
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } 
  else {
    Serial.println("Did not find fingerprint sensor :(");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("sensor error..");
    while (1) { delay(1); }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  dashboard();
  n = 0;
  char key = keypad.getKey();
  if (key) {
    if (key == '*') {
      Serial.println("Reset Fingerprint..");
      while (!resetFingerprint() );
    }
  }

  if (verifyFingerprint() >= 100) {
    Serial.println("Sucess");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Fingerprint");
    lcd.setCursor(0,1);
    lcd.print("Verified..");
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Level - 2");
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter passcode: ");
    lcd.setCursor(0, 1);
    
    while(true) {
      int i=0;
      char key2 = keypad.getKey();
      if (key2) {
        if (key2 == '#') {
          passCode = 0;
          n = 0;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Cleared..");
          delay(500);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Enter passcode: ");
        }
        else if(key2 == '*') {
          if(passCode == correctPassCode) {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Access granted");
            Serial.println("Access granted");
            delay(2000);
            passCode = 0;
            i = 0;
            break;
          }
          else {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Access denied");
            Serial.println("Access denied");
            delay(1000);
            passCode = 0;
            i = 0;
            break;
          }
        }
        else {
          temp = key2 - '0';
          passCode = (passCode * 10) + temp;
          n++;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Enter passcode: ");
          lcd.setCursor(0, 1);
          while(i<n) {
            lcd.print("*");
            i++;
          }
        }
      }
    }
  }  
  delay(500);
}

void dashboard() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Level - 1");
  lcd.setCursor(0,1);
  lcd.print("Place finger..");
}

uint8_t resetFingerprint() {
  int p = -1;
  Serial.println("Resetting fingerprint");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Resetting");
  lcd.setCursor(0,1);
  lcd.print("Fingerprint..");
  delay(1500);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter passcode: ");
  while(true) {
    int i=0;
    char key = keypad.getKey();
    if (key) {
      if (key == '#') {
        passCode = 0;
        n = 0;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Cleared..");
        delay(500);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Enter passcode: ");
      }
      else if(key == '*') {
        if(passCode == resetPin) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Fingerprint");
          lcd.setCursor(0, 1);
          lcd.print("change granted");
          Serial.println("Fingerprint change granted");
          delay(2000);
          passCode = 0;
          i = 0;
          break;
        }
        else {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Fingerprint");
          lcd.setCursor(0, 1);
          lcd.print("change denied");
          Serial.println("Fingerprint change denied");
          delay(1000);
          passCode = 0;
          i = 0;
          n = 0;
          return -1;
          break;
        }
      }
      else {
        temp = key - '0';
        passCode = (passCode * 10) + temp;
        n++;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Enter passcode: ");
        lcd.setCursor(0, 1);
        while(i<n) {
          lcd.print("*");
          i++;
        }
      }
    }
  }
  Serial.println("Place finger");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Place");
  lcd.setCursor(0,1);
  lcd.print("Finger..");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Scanning");
      lcd.setCursor(0,1);
      lcd.print("Finger..");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Error");
      break;
    default:
      Serial.println("Unknown error");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Error");
      break;
    }
  }

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Error");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Error");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Error");
      return p;
    default:
      Serial.println("Unknown error");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Error");
      return p;
  }

  Serial.println("Remove finger");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Remove finger");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Place finger");
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  p = -1;
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Error");
      break;
    default:
      Serial.println("Unknown error");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Error");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Error");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Error");
      return p;
    default:
      Serial.println("Unknown error");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Error");
      return p;
  }
  
  // OK converted!
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Error");
    return p;
  } else {
    Serial.println("Unknown error");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Error");
    return p;
  }   
  
  p = finger.storeModel(1);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Stored..");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Error");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Error");
    return p;
  } else {
    Serial.println("Unknown error");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Error");
    return p;
  }
}

int verifyFingerprint() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  return finger.confidence; 
}
