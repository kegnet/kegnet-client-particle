#include "Display.h"

#define _SSD1306 1
#define _SERIAL 1

Display::Display(int sda, int scl, int rst) {
  #ifdef _SSD1306
    //Wire.begin(sda, scl);
   	display = new Adafruit_SSD1306(rst);
  	display->begin(SSD1306_SWITCHCAPVCC, I2C_ADDRESS);
    //display->invertDisplay(1);
    display->setTextColor(WHITE);
    display->setTextWrap(false);
    display->setTextSize(2);
    display->clearDisplay();
  #endif
}

void Display::clear() {
    display->clearDisplay();
}

void Display::showPouring(float p, boolean isOz) {
  String s = String(p, 1);
  s.concat(isOz ? "oz" : "L");
  writeCenter(2, s);
  pouring = true;
}

void Display::showPour(float p, boolean isOz) {
  String s = String(p, 1);
  s.concat(isOz ? "oz" : "L");
  writeCenter(2, s, String("Enjoy!"));

  pouring = false;
  lastMessageTs = millis();
  lastMessageDelay = pourMessageDelay;
}

void Display::showNoPour() {
  writeCenter(2, String("No Pour"));

  pouring = false;
  lastMessageTs = millis();
  lastMessageDelay = noPourMessageDelay;
}

void Display::showTemp(float t, bool isF) {
  if (pouring || millis() - lastMessageTs < lastMessageDelay)
    return;

  String s = String(t, 0);
  s.concat(isF ? "F" : "C"); // TODO: add a degrees symbol (ascii 248)

  writeCenter(3, s);
}

void Display::showStatus(String s) {
  writeCenter(2, s);
}

void Display::showStatus(String s1, String s2) {
  writeCenter(2, s1, s2);
}

void Display::writeCenter(int textSize, String s) {
  #ifdef _SSD1306
    int x = (display->width() - (s.length() * textSize * 6))/2;
    int y = (display->height() - (textSize * 8))/2;
    display->clearDisplay();
    display->setTextSize(textSize);
    display->setCursor(x, y);
    display->println(s);
    display->display();
  #endif
  #ifdef _SERIAL
    Serial.println(s);
  #endif
}

void Display::writeCenter(int textSize, String s1, String s2) {
  #ifdef _SSD1306
    int y = (display->height() - (2 * textSize * 8))/2;
    display->clearDisplay();
    display->setTextSize(textSize);
    //display->setCursorY(y);

    int x1 = (display->width() - (s1.length() * textSize * 6))/2;
    //display->setCursorX(x1);
    display->setCursor(x1, y);
    display->println(s1);

    int x2 = (display->width() - (s2.length() * textSize * 6))/2;
    //display->setCursorX(x2);
    //display->setCursor(x2, display->getCursorY());
    display->setCursor(x2, 0);
    display->println(s2);

    display->display();
  #endif
  #ifdef _SERIAL
    Serial.println(s1);
    Serial.println(s2);
  #endif
}
