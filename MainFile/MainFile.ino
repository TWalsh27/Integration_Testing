// Main file for integration testing
// This should (in theory):
//    Track Barometer data
//    Track IMU data
//    Track GPS data
//    Write to an SD card via CSV file
// Please note that SD integration has not been added as of 6/7/26

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_BMP3XX.h>
#include <TinyGPSPlus.h>

Adafruit_BMP3XX bmp; // pins 3, 18, 19
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire1); // pins 2, 16, 17
TinyGPSPlus gps; // pins 0, 1, 4

#define SEA_LEVEL_PRESSURE_HPA 1013.25

unsigned long lastPrint = 0;
const unsigned long printInterval = 100;

float previousAltitude = 0.0;
unsigned long previousTime = 0;

void setup() {
  Serial.begin(115200);
  Serial1.begin(38400); // GPS baud, may need to alter
  delay(2000);

  Serial.println("Starting sensor test...");

  Wire.begin();   // BMP388 bus
  Wire1.begin();  // BNO055 bus

  if (!bmp.begin_I2C(0x77, &Wire)) {
    Serial.println("Could not find BMP388 at 0x77. Trying 0x76...");

    if (!bmp.begin_I2C(0x76, &Wire)) {
      Serial.println("BMP388 not found.");
      while (1);
    }
  }

  Serial.println("BMP388 found.");

  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_2X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_2X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_1);

  if (!bno.begin()) {
    Serial.println("BNO055 not found.");
    while (1);
  }

  Serial.println("BNO055 found.");
  bno.setExtCrystalUse(true);

  // CSV header file
  Serial.println(
  "time_ms,temp_C,pressure_hPa,altitude_m,velocity_mps,"
  "accel_x,accel_y,accel_z,"
  "heading,roll,pitch,"
  "gps_lat,gps_lon,gps_alt_m,gps_speed_mps");
}

void loop() {
  while (Serial1.available() > 0) {
  gps.encode(Serial1.read());
  } 

  unsigned long now = millis();

  // added a kill after 20 seconds
  if (now - lastPrint >= printInterval && now < 20000) {
    lastPrint = now;

    if (!bmp.performReading()) {
      Serial.println("BMP388 reading failed.");
      bool bmpOK = false;

        for (int i = 0; i < 4; i++) {  // original try + 3 retries
          if (bmp.performReading()) {
            bmpOK = true;
            break;
          }
        delay(5);
      }

      if (!bmpOK) {
        Serial.println("BMP388 reading failed after retries.");
        return;
      }
    }

    float temperature = bmp.temperature;
    float pressure_hPa = bmp.pressure / 100.0;
    float altitude = bmp.readAltitude(SEA_LEVEL_PRESSURE_HPA);

    float velocity = 0.0;
    if (previousTime != 0) {
      float dt = (now - previousTime) / 1000.0;
      velocity = (altitude - previousAltitude) / dt;
    }

    previousAltitude = altitude;
    previousTime = now;

    imu::Vector<3> accel = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
    imu::Vector<3> orient = bno.getVector(Adafruit_BNO055::VECTOR_EULER);

    Serial.print(now);
    Serial.print(",");

    Serial.print(temperature);
    Serial.print(",");

    Serial.print(pressure_hPa);
    Serial.print(",");

    Serial.print(altitude);
    Serial.print(",");

    Serial.print(velocity);
    Serial.print(",");

    Serial.print(accel.x());
    Serial.print(",");
    Serial.print(accel.y());
    Serial.print(",");
    Serial.print(accel.z());
    Serial.print(",");

    Serial.print(orient.x()); // heading
    Serial.print(",");
    Serial.print(orient.y()); // roll
    Serial.print(",");
    Serial.print(orient.z()); // pitch

    Serial.print(",");

    if (gps.location.isValid()) {
      Serial.print(gps.location.lat(), 6);
      Serial.print(",");
      Serial.print(gps.location.lng(), 6);
    } 
    else {
      Serial.print("INVALID,INVALID");
    }

    Serial.print(",");

    if (gps.altitude.isValid()) {
      Serial.print(gps.altitude.meters());
    } 
    else {
      Serial.print("INVALID");
    }

    Serial.print(",");

    if (gps.speed.isValid()) {
      Serial.println(gps.speed.mps());
    } else {
      Serial.println("INVALID");
    }
  }
}
