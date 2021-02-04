/*
  Arduino LSM6DS3 - Simple Accelerometer

  This example reads the accelerometer values from the LSM6DS3
  sensor and continuously prints them to the Serial Monitor and BLE

  The circuit:
  - Arduino Nano 33 IoT

  created 10 Jul 2019
  by Riccardo Rizzo

  edited 1 Jan 2021
  by Jingwen Zhu

  This example code is in the public domain.
*/

#include <ArduinoBLE.h>
#include <Arduino_LSM6DS3.h>

BLEService customService("19B10000-E8F2-537E-4F6C-D104768A1216");

BLEBoolCharacteristic accelCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1216", BLERead | BLENotify);

float prevX, prevY, prevZ = 0;

void setup() {
  Serial.begin(9600);
  //while (!Serial);
  Serial.println("Started");
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Acceleration in G's");
  Serial.println("X\tY\tZ");

  // begin initialization
  BLE.begin();

  // set advertised local name and service UUID:
  BLE.setLocalName("BLE");
  BLE.setAdvertisedService(customService);

  // add the characteristic to the service
  customService.addCharacteristic(accelCharacteristic);
  // add service
  BLE.addService(customService);

  // set the initial value for the characeristic:
  accelCharacteristic.writeValue(0);
  // start advertising
  BLE.advertise();

  Serial.println("BLE LED Peripheral");

}

void loop() {
  // poll for BLE events
  BLE.poll();

  float x, y, z;

  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);

    float absoluteDiff = abs(x - prevX) + abs(y - prevY) + abs(z - prevZ); //compare acceleration difference
    Serial.println(absoluteDiff);

    if (absoluteDiff > 2) {
      accelCharacteristic.writeValue(true);
    } else {
      accelCharacteristic.writeValue(false);
    }


  }




}
