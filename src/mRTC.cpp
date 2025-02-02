#include "mRTC.h"

mRTC::mRTC() {
    has_rtc = false;
    dt_previous_unix = 0;
    boot_dt_value = INT_MAX; // Assume invalid until initialized
}

bool mRTC::init() {
    if (!rtc.begin()) {
        Serial.println("[mRTC] RTC not found!");
        has_rtc = false;
        return false;
    }

    has_rtc = true;

    // Get current RTC time
    DateTime now = rtc.now();
    Serial.printf("[mRTC] Current RTC Time: %04d-%02d-%02d %02d:%02d:%02d\n",
                  now.year(), now.month(), now.day(),
                  now.hour(), now.minute(), now.second());

    // Fetch previous time from Preferences
    if (fetch()) {
        boot_dt_value = delta();
        Serial.printf("[mRTC] Last boot delta: %d seconds\n", boot_dt_value);
    } else {
        Serial.println("[mRTC] No valid previous boot time found.");
        boot_dt_value = INT_MAX;
    }

    // Save current time as new boot timestamp
    save();

    return true;
}

bool mRTC::connected() {
    return has_rtc;
}

void mRTC::print() {
    DateTime dt_now = now();
    Serial.printf("%04d-%02d-%02d %02d:%02d:%02d\n",
                    dt_now.year(), dt_now.month(), dt_now.day(),
                    dt_now.hour(), dt_now.minute(), dt_now.second());
}

String mRTC::str() {
    DateTime dt_now = now();
    char buffer[20];
    snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d %02d:%02d:%02d",
             dt_now.year(), dt_now.month(), dt_now.day(),
             dt_now.hour(), dt_now.minute(), dt_now.second());

    // note: String is Arduino String
    return String(buffer);
}

bool mRTC::update() {
    if (!has_rtc) return false;
    // do some updates...
    return true;
}

int mRTC::boot_dt() {
    return boot_dt_value;
}

DateTime mRTC::now() {
    return has_rtc ? rtc.now() : DateTime(1970, 1, 1, 0, 0, 0);
}

bool mRTC::fetch() {
    preferences.begin("rtc", false);
    if (preferences.isKey("epoch")) {
        dt_previous_unix = preferences.getULong("epoch", 0);

        if (dt_previous_unix > 0) {
            dt_previous = DateTime(dt_previous_unix);
            Serial.printf("[mRTC] Previous Boot Time: %04d-%02d-%02d %02d:%02d:%02d\n",
                          dt_previous.year(), dt_previous.month(), dt_previous.day(),
                          dt_previous.hour(), dt_previous.minute(), dt_previous.second());
            preferences.end();
            return true;
        }
    }
    preferences.end();
    return false;
}

void mRTC::save() {
    if (!has_rtc) return;

    DateTime now = rtc.now();
    preferences.begin("rtc", false);
    preferences.putULong("epoch", now.unixtime());
    preferences.end();
    Serial.println("[mRTC] Current boot time saved.");
}

int mRTC::delta() {
    if (!has_rtc) return INT_MAX;
    if (dt_previous_unix == 0) {
        Serial.println("[mRTC] delta() - Previous boot time is invalid!");
        return INT_MAX;
    }
    return rtc.now().unixtime() - dt_previous_unix;
}
