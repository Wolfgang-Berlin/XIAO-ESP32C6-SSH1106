#include "globals.h"
#include <Wire.h>

U8G2_SH1106_128X64_NONAME_F_HW_I2C oled(U8G2_R2, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ oled_CLK, /* data=*/ oled_SDA);

extern int Synchron_counter; // Counter for successful synchronizations

void initDisplay() {
  Wire.begin(oled_SDA, oled_CLK);
  oled.begin();
  oled.setPowerSave(0); // Display off
  oled.setContrast(64);
  oled.clearBuffer();
  oled.sendBuffer();
}

void clearDisplay() {
  oled.setPowerSave(0);
  oled.clearBuffer();
  oled.sendBuffer();
}

void drawStatus(const char* msg) {
  oled.setPowerSave(0);
  oled.clearBuffer();
  oled.setContrast(64);
  oled.setFont(u8g2_font_courR08_tr);
  oled.drawStr(0, 60, msg);
  oled.sendBuffer();
}

void drawInfo(const char* msg) {
  oled.setFont(u8g2_font_courR08_tr);
  oled.drawStr(122, 8, msg);
  oled.sendBuffer();
}

void drawTime(const struct tm* timeinfo) {
  char timeStr[6];
  oled.setPowerSave(0);
  oled.clearBuffer();
  strftime(timeStr, sizeof(timeStr), "%H:%M", timeinfo);
  oled.setContrast(1);
  oled.setFont(u8g2_font_logisoso42_tr);
  oled.drawStr(1, 53, timeStr);
  oled.sendBuffer();
}
