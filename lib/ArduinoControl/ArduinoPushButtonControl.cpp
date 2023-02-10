/**
 * Romgere Cockpit Library - https://github.com/romgere/romgere_cockpit
 * @author jerome@mestres.fr
 */

#include "ArduinoPushButtonControl.h"

#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE
ArduinoPushButtonControl::ArduinoPushButtonControl(uint8_t pin,
                                                   uint8_t defaultState,
                                                   int boardAddress)
    : ArduinoInputControl(DigitalControl, ITypePushButton, (boardAddress != -1),
                          boardAddress) {
#else
ArduinoPushButtonControl::ArduinoPushButtonControl(uint8_t pin,
                                                   uint8_t defaultState)
    : ArduinoInputControl(DigitalControl, ITypePushButton) {
#endif

  this->Pin1 = pin;
  this->InitialPinStatus = defaultState == HIGH ? HIGH : LOW;

  _pinMode(this->Pin1, INPUT);
}

bool ArduinoPushButtonControl::ReadInput() {
  return this->InitialPinStatus != _digitalRead(this->Pin1);
}
