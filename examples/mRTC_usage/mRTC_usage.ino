#include <Arduino.h>
#include "mRTC.h"

// mRTC - Custom DS3231 Real-Time Clock wrapper for ESP32
mRTC rtc;

void setup() {
    Serial.begin(115200);
    Serial.println();
    Wire.begin();
    rtc.init();

    if(rtc.connected()) {
        // display date using
        Serial.print("RTC Time: ");
        rtc.print();
        // or
        Serial.print("RTC Time: ");
        Serial.println(rtc.str());

        Serial.printf("Since last boot: %d seconds\n", rtc.boot_dt());
    }
    else Serial.println("RTC not found.");
}

void loop() {
    // print time & seconds since boot every second
    static unsigned long lastUpdate = 0;
    if (millis() - lastUpdate > 1000) {
        Serial.print(rtc.str());
        int dt_s = rtc.delta();
        Serial.printf(" (%d seconds since last boot)\n", dt_s);
        lastUpdate = millis();
    }
}
