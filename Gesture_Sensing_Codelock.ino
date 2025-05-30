#include <Wire.h>
#include "paj7620.h"
#include <LiquidCrystal.h>

#define GES_REACTION_TIME 500
#define GES_ENTRY_TIME 800
#define GES_QUIT_TIME 1000

int Contrast = 60;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
char code[4];
int keycount = 0;
int wrongcount = 0;

byte heart[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000
};

byte smile[8] = {
  0b00000,
  0b00000,
  0b01010,
  0b00000,
  0b10001,
  0b01110,
  0b00000,
  0b00000
};

void setup() {
  analogWrite(6, Contrast);
  lcd.begin(16, 2);

  uint8_t error = 0;

  Serial.begin(9600);
  Serial.println("\nPAJ7620U2 TEST DEMO: Recognize 9 gestures.");

  error = paj7620Init();
  if (error) {
    Serial.print("INIT ERROR, CODE: ");
    Serial.println(error);
  } else {
    Serial.println("INIT OK");
  }

  lcd.setCursor(0, 0);
  lcd.print("Enter gesture");

  Serial.println("Please input your gestures:\n");

  lcd.createChar(1, heart);
  lcd.createChar(2, smile);
  lcd.setCursor(0, 1);
  lcd.write(1);
}

void loop() {
  char psd[4] = {'1','2','3','4'};
  uint8_t data = 0, data1 = 0, error;
  error = paj7620ReadReg(0x43, 1, &data);

  if (!error) {
    switch (data) {
      case GES_RIGHT_FLAG:
        delay(GES_ENTRY_TIME);
        paj7620ReadReg(0x43, 1, &data);
        if (data == GES_FORWARD_FLAG) {
          lcd.clear(); lcd.print("Forward");
          code[keycount++] = '0';
          delay(GES_QUIT_TIME);
        } else if (data == GES_BACKWARD_FLAG) {
          lcd.clear(); lcd.print("Backward");
          code[keycount++] = '1';
          delay(GES_QUIT_TIME);
        } else {
          lcd.clear(); lcd.print("Reset");
          keycount = 0;
          for (int i = 0; i < 4; i++) code[i] = '9';
          Serial.println("Right");
        }
        break;

      case GES_LEFT_FLAG:
        delay(GES_ENTRY_TIME);
        paj7620ReadReg(0x43, 1, &data);
        if (data == GES_FORWARD_FLAG) {
          lcd.clear(); lcd.print("Forward");
          code[keycount++] = '0';
          Serial.println("Forward");
          delay(GES_QUIT_TIME);
        } else if (data == GES_BACKWARD_FLAG) {
          lcd.clear(); lcd.print("Backward");
          code[keycount++] = '1';
          Serial.println("Backward");
          delay(GES_QUIT_TIME);
        } else {
          lcd.clear(); lcd.print("Show Result");
          Serial.println("Left");

          if (code[0]=='5' && code[1]=='6' && code[2]=='2' && code[3]=='4' && wrongcount < 2) {
            lcd.clear(); lcd.print("Unlocked");
            keycount = 0;
            for (int i = 0; i < 4; i++) code[i] = '9';
            lcd.write(2);
            Serial.println("Unlocked");
          } else {
            if (wrongcount > 1) {
              lcd.clear(); lcd.print("Completely locked");
            } else if (wrongcount > 0) {
              lcd.clear(); lcd.print("Security alert");
              wrongcount++;
            } else {
              lcd.clear(); lcd.print("Wrong, locked");
              wrongcount++;
            }
            keycount = 0;
            for (int i = 0; i < 4; i++) code[i] = '9';
            Serial.println("Wrong locked");
          }
        }
        break;

      case GES_UP_FLAG:
        delay(GES_ENTRY_TIME);
        paj7620ReadReg(0x43, 1, &data);
        if (data == GES_FORWARD_FLAG) {
          lcd.clear(); lcd.print("Forward");
          code[keycount++] = '0';
          Serial.println("Forward");
          delay(GES_QUIT_TIME);
        } else if (data == GES_BACKWARD_FLAG) {
          lcd.clear(); lcd.print("Backward");
          code[keycount++] = '1';
          Serial.println("Backward");
          delay(GES_QUIT_TIME);
        } else {
          lcd.clear(); lcd.print("Up");
          code[keycount++] = '5';
          Serial.println("Up");
        }
        break;

      case GES_DOWN_FLAG:
        delay(GES_ENTRY_TIME);
        paj7620ReadReg(0x43, 1, &data);
        if (data == GES_FORWARD_FLAG) {
          lcd.clear(); lcd.print("Forward");
          code[keycount++] = '0';
          Serial.println("Forward");
          delay(GES_QUIT_TIME);
        } else if (data == GES_BACKWARD_FLAG) {
          lcd.clear(); lcd.print("Backward");
          code[keycount++] = '1';
          Serial.println("Backward");
          delay(GES_QUIT_TIME);
        } else {
          lcd.clear(); lcd.print("Down");
          code[keycount++] = '6';
          Serial.println("Down");
        }
        break;

      case GES_FORWARD_FLAG:
        lcd.clear(); lcd.print("Forward");
        code[keycount++] = '0';
        Serial.println("Forward");
        delay(GES_QUIT_TIME);
        break;

      case GES_BACKWARD_FLAG:
        lcd.clear(); lcd.print("Backward");
        code[keycount++] = '1';
        Serial.println("Backward");
        delay(GES_QUIT_TIME);
        break;

      case GES_CLOCKWISE_FLAG:
        lcd.clear(); lcd.print("Clockwise");
        code[keycount++] = '2';
        Serial.println("Clockwise");
        break;

      case GES_COUNT_CLOCKWISE_FLAG:
        lcd.clear(); lcd.print("Anti-clockwise");
        code[keycount++] = '3';
        Serial.println("Anti-clockwise");
        break;

      default:
        paj7620ReadReg(0x44, 1, &data1);
        if (data1 == GES_WAVE_FLAG) {
          lcd.clear(); lcd.print("Wave");
          code[keycount++] = '4';
          Serial.println("Wave");
        }
        break;
    }
  }
  delay(100);
}

