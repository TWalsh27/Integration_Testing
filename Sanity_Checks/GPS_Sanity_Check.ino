#include <SparkFun_u-blox_GNSS_Arduino_Library.h>

SFE_UBLOX_GNSS myGPS;

void setup() {
    Wire.begin();

    if (myGPS.begin() == false) {
        Serial.println("GPS not detected");
    }
}

void loop() {
    long latitude = myGPS.getLatitude();
    long longitude = myGPS.getLongitude();

    Serial.println(latitude);
    Serial.println(longitude);
}