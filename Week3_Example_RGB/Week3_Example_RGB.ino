/*
  RGB BLE Central

  The circuit:
  - Arduino MKR WiFi 1010, Arduino Uno WiFi Rev2 board, Arduino Nano 33 IoT,
    Arduino Nano 33 BLE, or Arduino Nano 33 BLE Sense board.

  This example code is in the public domain.
*/

#define RED 22
#define BLUE 24
#define GREEN 23
#define LED_PWR 25


#include <ArduinoBLE.h>

void setup() {
  // intitialize the digital Pin as an output
  pinMode(RED, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(LED_PWR, OUTPUT);

  Serial.begin(9600);
  while (!Serial);

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }

  Serial.println("BLE Central - SensorTag button");
  Serial.println("Make sure to turn on the device.");

  // start scanning for peripheral
  BLE.scan();
}

void loop() {
  // check if a peripheral has been discovered
  BLEDevice peripheral = BLE.available();

  if (peripheral) {
    // discovered a peripheral, print out address, local name, and advertised service
    Serial.print("Found ");
    Serial.print(peripheral.address());
    Serial.print(" '");
    Serial.print(peripheral.localName());
    Serial.print("' ");
    Serial.print(peripheral.advertisedServiceUuid());
    Serial.println();

    // Check if the peripheral is a SensorTag, the local name will be:
    // "CC2650 SensorTag"
    if (peripheral.localName() == "BLE") {
      // stop scanning
      BLE.stopScan();

      monitorSensorTagButtons(peripheral);

      // peripheral disconnected, start scanning again
      BLE.scan();
    }
  }
}

void monitorSensorTagButtons(BLEDevice peripheral) {
  // connect to the peripheral
  Serial.println("Connecting ...");
  if (peripheral.connect()) {
    Serial.println("Connected");
  } else {
    Serial.println("Failed to connect!");
    return;
  }

  // discover peripheral attributes
  Serial.println("Discovering service 0xffe0 ...");
  if (peripheral.discoverService("19B10000-E8F2-537E-4F6C-D104768A1216")) {
    Serial.println("Service discovered");
  } else {
    Serial.println("Attribute discovery failed.");
    peripheral.disconnect();

    while (1);
    return;
  }

  // retrieve the characteristics
  BLECharacteristic CharacteristicX = peripheral.characteristic("19B10001-E8F2-537E-4F6C-D104768A1216");
  BLECharacteristic CharacteristicY = peripheral.characteristic("19B10002-E8F2-537E-4F6C-D104768A1216");
  BLECharacteristic CharacteristicZ = peripheral.characteristic("19B10003-E8F2-537E-4F6C-D104768A1216");

  // subscribe to the simple key characteristic
  Serial.println("Subscribing to simple key characteristic ...");
  if (!CharacteristicX) {
    Serial.println("no simple key characteristic found!");
    peripheral.disconnect();
    return;
  } else if (!CharacteristicX.canSubscribe()) {
    Serial.println("simple key characteristic is not subscribable!");
    peripheral.disconnect();
    return;
  } else if (!CharacteristicX.subscribe()) {
    Serial.println("subscription failed!");
    peripheral.disconnect();
    return;
  } else {
    Serial.println("Subscribed");
    Serial.println("Press the right and left buttons on your SensorTag.");
  }

  while (peripheral.connected()) {
    // while the peripheral is connected

    // check if the value of the simple key characteristic has been updated
    if (CharacteristicX.valueUpdated()) {
      // yes, get the value, characteristic is 1 byte so use byte value
      byte value = 0;

      CharacteristicX.readValue(value);
      analogWrite(RED, value);
      Serial.print("RED: ");
      Serial.println(value);
    }

    if (CharacteristicY.valueUpdated()) {
      // yes, get the value, characteristic is 1 byte so use byte value
      byte value = 0;

      CharacteristicY.readValue(value);
      analogWrite(GREEN, value);
    }

    if (CharacteristicZ.valueUpdated()) {
      // yes, get the value, characteristic is 1 byte so use byte value
      byte value = 0;

      CharacteristicZ.readValue(value);
      analogWrite(BLUE, value);
    }
  }

  Serial.println("SensorTag disconnected!");
}
