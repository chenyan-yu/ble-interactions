#include <ArduinoBLE.h>

const int BUTTON_PIN = 3;

void setup()
{
  Serial.begin(9600);
  pinMode( BUTTON_PIN, INPUT );
  BLE.setLocalName("Beacon");
  BLE.begin();
  // start advertising
  BLE.advertise();
}

void loop()
{
  const int DATA_SIZE = 1;
  uint8_t data[DATA_SIZE];
  static int oldButtonState = LOW;

  int buttonState = digitalRead( BUTTON_PIN );

  if ( oldButtonState != buttonState )
  {
    oldButtonState = buttonState;
    Serial.println(buttonState);

    data[0] = ( uint8_t ) buttonState;

    BLE.stopAdvertise();
    BLE.setManufacturerData( data, DATA_SIZE );
    BLE.advertise();
  }
}
