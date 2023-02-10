/**
 * Romgere Cockpit Library - https://github.com/romgere/romgere_cockpit
 * @author jerome@mestres.fr
 */

#include "ArduinoPushButtonControl.h"

#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE
ArduinoPushButtonControl::ArduinoPushButtonControl(uint8_t pin,
                                                   uint8_t defautStat,
                                                   int boardAddress)
    : ArduinoInputControl(DigitalControl, ITypePushButton, (boardAddress != -1),
                          boardAddress) {
#else
ArduinoPushButtonControl::ArduinoPushButtonControl(uint8_t pin,
                                                   uint8_t defautStat)
    : ArduinoInputControl(DigitalControl, ITypePushButton) {
#endif

  this->Pin1 = pin;
  this->InitialPinStatus = defautStat == HIGH ? HIGH : LOW;

  _pinMode(this->Pin1, INPUT);
}

bool ArduinoPushButtonControl::ReadInput() {
#ifdef DEBUG_CONTROL_STAT
  int newVal = _digitalRead(this->Pin1);
  Serial.print("Control Debug : Read PushButton[");
  Serial.print(this->Pin1);
  Serial.print("] Stat : ");
  Serial.print(this->InitialPinStatus != newVal ? "CHANGE !" : "NO CHANGE");
  Serial.print(", Value : ");
  Serial.print(newVal);
  Serial.println(".");

  return this->InitialPinStatus != newVal;
#else
  return this->InitialPinStatus != _digitalRead(this->Pin1);
#endif
}
