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

BLEIntCharacteristic CharacteristicX("19B10001-E8F2-537E-4F6C-D104768A1216", BLERead | BLENotify);
BLEIntCharacteristic CharacteristicY("19B10002-E8F2-537E-4F6C-D104768A1216", BLERead | BLENotify);
BLEIntCharacteristic CharacteristicZ("19B10003-E8F2-537E-4F6C-D104768A1216", BLERead | BLENotify);


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
  customService.addCharacteristic(CharacteristicX);
  customService.addCharacteristic(CharacteristicY);
  customService.addCharacteristic(CharacteristicZ);

  // add service
  BLE.addService(customService);

  // set the initial value for the characeristic:
  CharacteristicX.writeValue(0);
  CharacteristicY.writeValue(0);
  CharacteristicZ.writeValue(0);

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

    Serial.print(x);
    Serial.print('\t');
    Serial.print(y);
    Serial.print('\t');
    Serial.println(z);

    CharacteristicX.writeValue(x*100);
    CharacteristicY.writeValue(y*100);
    CharacteristicZ.writeValue(z*100);
  }
  



}
