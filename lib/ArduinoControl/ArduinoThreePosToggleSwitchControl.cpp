/**
 * Romgere Cockpit Library - https://github.com/romgere/romgere_cockpit
 * @author jerome@mestres.fr
 */

#include "ArduinoThreePosToggleSwitchControl.h"

#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE
ArduinoThreePosToggleSwitchControl::ArduinoThreePosToggleSwitchControl(
    uint8_t pin1, uint8_t pin2, int boardAddress)
    : ArduinoInputControl(DigitalControl, ITypeToggleSwitch,
                          (boardAddress != -1), boardAddress) {
#else
ArduinoThreePosToggleSwitchControl::ArduinoThreePosToggleSwitchControl(
    uint8_t pin1, uint8_t pin2)
    : ArduinoInputControl(DigitalControl, ITypeToggleSwitch) {
#endif

  this->Pin1 = pin1;
  this->Pin2 = pin2;

  _pinMode(pin1, INPUT);
  _pinMode(pin2, INPUT);

  LastPinStatus[0] = _digitalRead(pin1);
  LastPinStatus[1] = _digitalRead(pin2);
}

ArduinoThreePosToggleSwitchControl::~ArduinoThreePosToggleSwitchControl() {}

bool ArduinoThreePosToggleSwitchControl::ReadInput() {

  uint8_t last[2];

  // Keep current value as old, and read new one
  last[0] = this->LastPinStatus[0];
  last[1] = this->LastPinStatus[1];
  this->LastPinStatus[0] = _digitalRead(this->Pin1);
  this->LastPinStatus[1] = _digitalRead(this->Pin2);

  // Value changed ?
  return this->LastPinStatus[0] != last[0] || this->LastPinStatus[1] != last[1];
}

float ArduinoThreePosToggleSwitchControl::getValue() {

  if (this->LastPinStatus[0] == HIGH && this->LastPinStatus[1] == LOW) {
    return 0;
  } else if (this->LastPinStatus[0] == LOW && this->LastPinStatus[1] == LOW) {
    return 1;
  } else if (this->LastPinStatus[0] == LOW && this->LastPinStatus[1] == HIGH) {
    return 2;
  }

  // Unknow state !?
  return 0;
}
