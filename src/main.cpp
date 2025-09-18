
/**
 *
 * - NTP sync at startup + every day at 4:30 AM
 * - afterward, WiFi turn off  
 * - The display is off during bedtime
 *
 * Hardware: SEED XIAO-ESP32C6 + SH1106 OLED 128x64 I2C
 * 
 * Dependencies:
 * - Arduino core for ESP32
 * - U8g2 library for OLED display
 *           https://github.com/olikraus/u8g2/wiki/fntlistallplain#u8g2-font-list
 *
 * - create file "secrets.h" for WiFi credentials
 * 
 *          #define SECRET_SSID "..."		  // replace MySSID with your WiFi network name
 *          #define SECRET_PASS "XXXXXX"	// replace MyPassword with your WiFi password
 *
 * 
 * Author: Wolfgang-Berlin
 * License: MIT License
 * Date: 15-09-2025
 *
 *
 *                |    |
 *                |    |
 *            -    ----     -          
 *            -             -     GND       
 *            -             - + 3.3 V  
 *            -             -          
 *        SDA -  GPIO 21    -          
 *        SCK -  GPIO 22    -          
 *            -             -          
 *
 *
*/

#include <Arduino.h>
#include "globals.h"

// Berlin/Europa mit DST
#define TIMEZONE "CET-1CEST,M3.5.0/02,M10.5.0/3"

static int lastDisplayedMinute = -1; 
static int lastSyncDay = -1;

// --- Setup ---
void setup() {
  Serial.begin(115200);

  pinMode(WIFI_ANT_CONFIG, OUTPUT);    // pinMode(14, OUTPUT);
  digitalWrite(WIFI_ANT_CONFIG, HIGH); // digitalWrite(14, HIGH); // Use external antenna

  initDisplay();
  
  // erster NTP-Sync beim Start
  syncTime();
  delay(500);
}

// --- Loop ---

static bool syncDoneThisMinute = false;

#define Sync_Stunde 4         
#define Sync_Min    30
const int sleepTime_Start = 23;  // 23:00 Uhr
const int sleepTime_End  =  6;   // 06:00 Uhr
const int SYNC_OK_DISPLAY_HOURS = 4;   

void loop() {
  time_t now = time(nullptr);
  struct tm nowLocal;
  localtime_r(&now, &nowLocal);

   // synchronize daily
  if (nowLocal.tm_hour == Sync_Stunde && nowLocal.tm_min == Sync_Min && !syncDoneThisMinute) {
      syncTime();
      syncDoneThisMinute = true;
      }
  if (nowLocal.tm_min != Sync_Min) {
      syncDoneThisMinute = false;
      }


  if (nowLocal.tm_hour >= sleepTime_Start || nowLocal.tm_hour < sleepTime_End) {
    if (nowLocal.tm_min != lastDisplayedMinute) {
          clearDisplay();
          lastDisplayedMinute = nowLocal.tm_min;
          }
  } else {
    // Night
      oled.setPowerSave(0);

    if (nowLocal.tm_min != lastDisplayedMinute) {
      // Day
        drawTime(&nowLocal);
        Serial.println(&nowLocal);
        time_t lastSync = getLastSyncTime();
            if (lastSync > 0 && (now - lastSync) < (SYNC_OK_DISPLAY_HOURS * 3600)) {
              lastDisplayedMinute = nowLocal.tm_min;
        }
    }
   } 
  delay(1000);
}
