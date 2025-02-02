#ifndef MRTC_H
#define MRTC_H

#include <Arduino.h>
#include <Preferences.h>
#include <SPI.h>
#include <Wire.h>
#include <RTClib.h>

/*
mRTC - DS3231 Real-Time Clock Manager for ESP32
================================================
This class provides an interface for managing a **DS3231 RTC** on an ESP32 using the RTClib library.
It handles:

- Initialization & detection of the DS3231 module  
- Persistent storage of boot timestamps using Preferences  
- Fetching & saving boot times to calculate uptime  
- Returning the current RTC time

VARS
    rtc                 → RTC object for DS3231.
    preferences         → Manages persistent storage for boot timestamps.
    has_rtc             → true if RTC is detected, false otherwise.
    dt_previous         → Stores the previous boot time as a DateTime object.
    dt_previous_boot    → Stores the previous boot time as a Unix timestamp.
    boot_dt_value       → Time difference (in seconds) between the current and last boot.

FUNCTIONS
    mRTC()              → Constructor, initializes variables.
    init()              → Initializes RTC, fetches previous boot time, calculates boot_dt_value, and saves the current time.
    connected()         → Returns whether RTC is detected.
    print()             → Print time to Serial
    update()            → Updates vars; right now, just a placeholder
    boot_dt()           → Returns the time difference (in seconds) since the last boot.
    now()               → Returns the current RTC time as a DateTime object.
    fetch()             → Retrieves the last boot time from Preferences; returns false if invalid.
    save()              → Stores the current RTC time as the new boot timestamp in Preferences.
    delta()             → Calculates the difference between the current and previous boot time in seconds.

**NOTE:** This implementation is **specific to the DS3231 RTC** and may not work with other RTC modules like DS1307.
*/


class mRTC {
public:
    mRTC();
    bool init();                        // initialize the RTC; return false if not found;
                                        // fetch() previous time; save() current time;
                                        // determines boot_dt = delta()

    bool connected();                   // return has_rtc value
    void print();                       // prints current time to Serial
    String str();                  // current RTC time as a formatted string
    bool update();                      // update vars; right now, just a placeholder.
    int delta();                        // calculate and returns dt = now - previous boot (in seconds)
                                        // returns INT_MAX if previous boot (ie. fetch()) invalid
    int boot_dt();                      // returns boot_dt
    DateTime now();                     // returns rtc.now()

private:
    bool fetch();                       // fetch previous time from preferences; return false if invalid
    void save();                        // save current time to preferences

    RTC_DS3231 rtc;                     // RTC object for the connected RTC device
    Preferences preferences;            // object for saving/fetching persistent data
    bool has_rtc;                       // flag for if RTC was found or not

    DateTime dt_previous;               // datetime object for previous boot
    unsigned long dt_previous_unix;     // unix timestamp for previous boot
    int boot_dt_value = INT_MAX;        // the delta() value (in seconds); assume high if not initialized
};

#endif