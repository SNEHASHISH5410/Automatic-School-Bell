/*
 * Automatic School Bell Controller
 * Hardware: Arduino UNO, DS3231 RTC, LCD 16x2, EEPROM, Relay, Buzzer, Push buttons
 * 
 * Buttons:
 *  - SET   -> A0
 *  - NEXT  -> A1
 *  - UP    -> A2
 *  - DOWN  -> A3
 * 
 * Relay -> pin 8
 * Buzzer -> pin 13
 * LCD pins: RS(2), EN(3), D4(4), D5(5), D6(6), D7(7)
 */

#include <Wire.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include "RTClib.h"

// LCD pins
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

// RTC
RTC_DS3231 rtc;

// Buttons
#define BT_SET   A0
#define BT_NEXT  A1
#define BT_UP    A2
#define BT_DOWN  A3

// Outputs
#define RELAY   8
#define BUZZER 13

// Bell schedule (example: 8 bells max)
#define MAX_BELLS 8
struct BellTime {
  byte hour;
  byte minute;
  byte duration;  // seconds
};
BellTime bells[MAX_BELLS];

// State variables
bool settingMode = false;
byte currentBell = 0;
byte settingField = 0;

// Helper for button reading
bool readButton(int pin) {
  return analogRead(pin) < 100; // active LOW with pullup
}

void setup() {
  pinMode(RELAY, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(RELAY, LOW);
  digitalWrite(BUZZER, LOW);

  pinMode(BT_SET, INPUT_PULLUP);
  pinMode(BT_NEXT, INPUT_PULLUP);
  pinMode(BT_UP, INPUT_PULLUP);
  pinMode(BT_DOWN, INPUT_PULLUP);

  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("School Bell Ctrl");

  if (!rtc.begin()) {
    lcd.setCursor(0, 1);
    lcd.print("RTC ERROR!");
    while (1);
  }

  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  loadSchedule();
  delay(1500);
}

void loop() {
  DateTime now = rtc.now();

  // Check bell ringing
  for (int i = 0; i < MAX_BELLS; i++) {
    if (now.hour() == bells[i].hour &&
        now.minute() == bells[i].minute &&
        now.second() == 0) {
      ringBell(bells[i].duration);
    }
  }

  // Display
  lcd.setCursor(0, 0);
  printTime(now);
  lcd.setCursor(0, 1);
  lcd.print("Bell ");
  lcd.print(currentBell + 1);
  lcd.print(": ");
  printBellTime(bells[currentBell]);

  // Handle buttons
  handleButtons();
  delay(200);
}

void ringBell(byte duration) {
  digitalWrite(RELAY, HIGH);
  digitalWrite(BUZZER, HIGH);
  delay(duration * 1000);
  digitalWrite(RELAY, LOW);
  digitalWrite(BUZZER, LOW);
}

void handleButtons() {
  if (readButton(BT_SET)) {
    settingMode = !settingMode;
    delay(300);
  }

  if (settingMode) {
    if (readButton(BT_NEXT)) {
      settingField = (settingField + 1) % 3; // hour, min, dur
      delay(300);
    }
    if (readButton(BT_UP)) {
      adjustBell(1);
      delay(200);
    }
    if (readButton(BT_DOWN)) {
      adjustBell(-1);
      delay(200);
    }
    saveSchedule();
  } else {
    if (readButton(BT_NEXT)) {
      currentBell = (currentBell + 1) % MAX_BELLS;
      delay(300);
    }
  }
}

void adjustBell(int change) {
  switch (settingField) {
    case 0: // hour
      bells[currentBell].hour = (bells[currentBell].hour + change + 24) % 24;
      break;
    case 1: // minute
      bells[currentBell].minute = (bells[currentBell].minute + change + 60) % 60;
      break;
    case 2: // duration
      int d = bells[currentBell].duration + change;
      if (d < 1) d = 1;
      if (d > 30) d = 30;
      bells[currentBell].duration = d;
      break;
  }
}

void printTime(DateTime now) {
  if (now.hour() < 10) lcd.print("0");
  lcd.print(now.hour());
  lcd.print(":");
  if (now.minute() < 10) lcd.print("0");
  lcd.print(now.minute());
  lcd.print(":");
  if (now.second() < 10) lcd.print("0");
  lcd.print(now.second());
}

void printBellTime(BellTime b) {
  if (b.hour < 10) lcd.print("0");
  lcd.print(b.hour);
  lcd.print(":");
  if (b.minute < 10) lcd.print("0");
  lcd.print(b.minute());
  lcd.print(" ");
  lcd.print(b.duration);
  lcd.print("s ");
}

void saveSchedule() {
  for (int i = 0; i < MAX_BELLS; i++) {
    EEPROM.update(i * 3, bells[i].hour);
    EEPROM.update(i * 3 + 1, bells[i].minute);
    EEPROM.update(i * 3 + 2, bells[i].duration);
  }
}

void loadSchedule() {
  for (int i = 0; i < MAX_BELLS; i++) {
    bells[i].hour = EEPROM.read(i * 3);
    bells[i].minute = EEPROM.read(i * 3 + 1);
    bells[i].duration = EEPROM.read(i * 3 + 2);
    if (bells[i].duration == 0) bells[i].duration = 3; // default 3s
  }
}
