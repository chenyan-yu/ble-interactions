/*
  ArduinoBLE Potentiometer and Button

  This example creates a BLE peripheral with service that contains a
  characteristic to represents the reading of the potentiometer.

  The circuit:
  - Arduino Nano 33 Iot
  - Potentiometer connected to pin A0
  - Button connected to pin 3

  This example code is in the public domain.
*/

#include <ArduinoBLE.h>

const int sensorPin = A0; // set sensorPin to analog pin A0
const int buttonPin = 3; // set buttonPin to digital pin 3

BLEService sensorService("19B10010-E8F2-537E-4F6C-D104768A1214"); // create service
BLEService buttonService("19B10011-E8F2-537E-4F6C-D104768A1214"); // create service

// create button characteristic and allow remote device to read and write
BLEIntCharacteristic sensorCharacteristic("19B10012-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);
BLEBoolCharacteristic buttonCharacteristic("19B10013-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);

void setup() {
  Serial.begin(9600);
  //while (!Serial);

  pinMode(sensorPin, INPUT); // use button pin as an input
  pinMode(buttonPin, INPUT); // use button pin as an input

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }

  // set the local name peripheral advertises
  BLE.setLocalName("Sensor Button");

  // set the UUID for the service this peripheral advertises:
  BLE.setAdvertisedService(sensorService);
  BLE.setAdvertisedService(buttonService);

  // add the characteristics to the service
  sensorService.addCharacteristic(sensorCharacteristic);
  buttonService.addCharacteristic(buttonCharacteristic);


  // add the service
  BLE.addService(sensorService);
  BLE.addService(buttonService);


  //set init value
  sensorCharacteristic.writeValue(0);
  buttonCharacteristic.writeValue(0);


  // start advertising
  BLE.advertise();

  Serial.println("Bluetooth device active, waiting for connections...");
}

void loop() {
  // poll for BLE events
  BLE.poll();

  int sensorValue = analogRead(sensorPin);
  Serial.println(sensorValue / 4);
  sensorCharacteristic.writeValue(sensorValue / 4);
  bool buttonValue = digitalRead(buttonPin);
  buttonCharacteristic.writeValue(buttonValue);

}
