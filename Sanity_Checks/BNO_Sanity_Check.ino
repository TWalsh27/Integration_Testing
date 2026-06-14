// PLEASE NOTE:
// This file is not for anything related to telemetry.
// This file is simply to ensure stable connection to the BNO055 component

#include <Adafruit_BNO055.h>
#include <Wire.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire1);

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Attempting to connect to BNO055...");
  Wire1.begin();

  if (!bno.begin()) {
    Serial.println("Houston, we have a problem.");
    Serial.println("BNO055 not found...");
    while(1); // Never reach loop
  }

  Serial.println("BNO055 found!");
}

void loop() {

  imu::Vector<3> accel =
      bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);

  imu::Vector<3> orient =
      bno.getVector(Adafruit_BNO055::VECTOR_EULER);

  Serial.println("----- BNO055 -----");

  Serial.print("Heading: ");
  Serial.println(orient.x());

  Serial.print("Roll: ");
  Serial.println(orient.y());

  Serial.print("Pitch: ");
  Serial.println(orient.z());

  Serial.print("Accel X: ");
  Serial.println(accel.x());

  Serial.print("Accel Y: ");
  Serial.println(accel.y());

  Serial.print("Accel Z: ");
  Serial.println(accel.z());

  Serial.println();

  delay(1000);
}
