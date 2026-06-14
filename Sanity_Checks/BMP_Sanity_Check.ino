// PLEASE NOTE:
// Not for anything related to actual telemetry data.
// This file is part of a sequence of files meant for ensuring key components 

#include <Adafruit_BMP3XX.h>
#include <Wire.h>

#define SEA_LEVEL_PRESSURE_HPA 1013.25

Adafruit_BMP3XX bmp;

void setup() {
  Serial.begin(115200);
  delay(2000);

  Serial.println("Attempting to initialize BMP.");

  Wire.begin();

  if (!bmp.begin_I2C(0x77, &Wire)) {
    Serial.println("Couldn't find BMP388 at 0x77, now attempting 0x76...");

    if (!bmp.begin_I2C(0x76, &Wire)) {
      Serial.println("Houston, we have a problem.");
      Serial.println("BMP not found...");
      while(1); // Never go to loop if BMP not found
    }
  }

  Serial.println("BMP found!");
}

void loop() {

  if (!bmp.performReading()) {
    Serial.println("BMP reading failed...");
    return;
  }

  float temp = bmp.temperature;
  float pressure = bmp.pressure / 100.0;
  float altitude = bmp.readAltitude(SEA_LEVEL_PRESSURE_HPA);
  Serial.print("Pressure: ");
  Serial.println(pressure);
  Serial.print("Temperature: ");
  Serial.println(temp);
  Serial.print("Altitude: ");
  Serial.println(altitude);

  delay(1000); // 1 second delay
}
