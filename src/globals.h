#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <time.h>
#include <U8g2lib.h>

// Pins
#define oled_CLK 23
#define oled_SDA 22

// --- gemeinsame Daten (nur Deklarationen) ---
extern int Synchron_counter;   // 0 = noch nicht, 1 = ok, -1 = fehler
extern time_t lastSyncTime;    // letzter erfolgreicher Sync (unix timestamp)

// Display-Objekt (nur extern, Definition in genau einer .cpp)
extern U8G2_SH1106_128X64_NONAME_F_HW_I2C oled;

// --- TimeSync API (Implementierung in TimeSync.cpp) ---
bool syncTime();
void disconnectWiFi();
time_t getLastSyncTime();

// --- Display API (Implementierung in Display.cpp) ---
void initDisplay();
void clearDisplay();
void drawStatus(const char* msg);
void drawInfo(const char* msg);
void drawTime(const struct tm* timeinfo);

#endif