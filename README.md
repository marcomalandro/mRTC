# **mRTC - Custom DS3231 Real-Time Clock Manager for ESP32**

(M. Maia, Version 0.1)

A lightweight **Custom DS3231 RTC manager** for ESP32 using the **RTClib** library. It provides an easy-to-use interface for:

- **Initializing and detecting** a DS3231 RTC module
- **Persistently storing boot timestamps** in ESP32's non-volatile memory (NVS) using Preferences
- **Calculating uptime** by comparing boot timestamps
- **Fetching and returning the current RTC time**

**Note:** This library is **specific to the DS3231 RTC** and may not work with other RTC modules like **DS1307**.

This libary is just to declutter code.

## Features
- Detects and initializes the DS3231 RTC  
- Saves and retrieves boot timestamps using ESP32 Preferences  
- Calculates **time elapsed since the last boot**  
- Provides a `DateTime` object for the **current RTC time**  
- Encapsulated in a **clean, reusable C++ class**  

---

## **Usage**

```cpp
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

```

## **API Reference**
### **Public Methods**
| Method | Description |
|--------|------------|
| `mRTC()` | Constructor, initializes variables. |
| `bool init()` | Initializes DS3231, fetches previous boot time, calculates `boot_dt_value`, and saves the current time. Returns `false` if RTC is not found. |
| `bool connected()` | Returns `true` if DS3231 is detected. |
| `void print()` | Print time to Serial (format: `YYYY-MM-DD HH:MM:SS\n`). |
| `String str()` | Same as print() but returns string. |
| `bool update()` | Updates RTC variables and recalculates `boot_dt_value`. |
| `int boot_dt()` | Returns the time difference (in seconds) since the last boot. |
| `DateTime now()` | Returns the current RTC time as a `DateTime` object. |
| `int delta()` | Calculates the difference between the current and previous boot time in seconds. Returns `INT_MAX` if previous boot time is invalid. |

### **Private Methods**
| Method | Description |
|--------|------------|
| `bool fetch()` | Retrieves the last boot time from Preferences; returns `false` if invalid. |
| `void save()` | Stores the current RTC time as the new boot timestamp in Preferences. |

## **Dependencies**
- [RTClib](https://github.com/adafruit/RTClib) (for DS3231 communication)
- [Arduino Preferences](https://www.arduino.cc/reference/en/libraries/preferences/) (for ESP32 non-volatile storage)
- [SPI](https://github.com/arduino-libraries/SPI) (included automatically)

### Versions

As of this writing, these are the versions deployed:

- RTClib @ 2.1.4
- SPI @ 2.0.0
- Wire @ 2.0.0

## **License**
This project is licensed under the **MIT License**.  
Feel free to modify and distribute!

## **Contributing**
Pull requests are welcome! If you find any issues, please submit them via the **Issues** tab.

