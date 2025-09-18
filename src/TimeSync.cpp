#include <Arduino.h>
#include "globals.h"
#include <WiFi.h>
#include "esp_wifi.h"   
#include <secrets.h>

// SSID and password come from secrets.h
const char *ssid = SECRET_SSID;
const char *password = SECRET_PASS;

static time_t lastSyncTimestamp = 0;  
int Synchron_counter = 0; // Counter for successful synchronizations

time_t getLastSyncTime() { 
  return lastSyncTimestamp; 
}


void disconnectWiFi() {
  WiFi.disconnect(true, true);
  WiFi.mode(WIFI_OFF);
}

bool syncTime() {
  Serial.println("NTP-Sync start ..");
  drawStatus("WLAN on …");

  esp_wifi_set_ps(WIFI_PS_NONE);        
  setCpuFrequencyMhz(160);             
  delay(500);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  unsigned long t0 = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - t0 < 30000) {  
    delay(250);
  }
  if (WiFi.status() != WL_CONNECTED) {
    drawStatus("WLAN Timeout");
    disconnectWiFi();
    return false;
  }

  drawStatus("NTP Sync…");

  // Berlin/Europa with DST
  #define TIMEZONE "CET-1CEST,M3.5.0/02,M10.5.0/3"
  configTzTime(TIMEZONE, "de.pool.ntp.org");
  tzset();

  time_t now = 0;
  struct tm ti = {0};
  bool ok = false;
  for (int i = 0; i < 60; ++i) {
    time(&now);
    localtime_r(&now, &ti);
    if (ti.tm_year >= (2024 - 1900)) { 
      ok = true; 
      break; 
    }
    delay(500);
  }

  if (!ok) {
    drawStatus("NTP fail");
    disconnectWiFi();
    return false;
  }

  drawStatus("Time OK");
  delay(2000);
  time(&lastSyncTimestamp); 
  Synchron_counter = 1;
  disconnectWiFi();
  setCpuFrequencyMhz(40);              // power save
  WiFi.setSleep(true);
  esp_wifi_set_ps(WIFI_PS_MIN_MODEM);  // Modem-Sleep

  return true;
}
