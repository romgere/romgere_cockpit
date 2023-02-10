/**
 * Romgere Cockpit Library - https://github.com/romgere/romgere_cockpit
 * @author jerome@mestres.fr
 */

#include "ArduinoToggleSwitchControl.h"

#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE
ArduinoToggleSwitchControl::ArduinoToggleSwitchControl(uint8_t pin,
                                                       int boardAddress)
    : ArduinoInputControl(DigitalControl, ITypeToggleSwitch,
                          (boardAddress != -1), boardAddress) {
#else
ArduinoToggleSwitchControl::ArduinoToggleSwitchControl(uint8_t pin)
    : ArduinoInputControl(DigitalControl, ITypeToggleSwitch) {
#endif

  this->Pin1 = pin;

  _pinMode(this->Pin1, INPUT);

  LastPinStatus = _digitalRead(this->Pin1);
}

ArduinoToggleSwitchControl::~ArduinoToggleSwitchControl() {}

bool ArduinoToggleSwitchControl::ReadInput() {

  // Keep current value as old, and read new one
  uint8_t last = this->LastPinStatus;
  this->LastPinStatus = _digitalRead(this->Pin1);

#ifdef DEBUG_CONTROL_STAT
  Serial.print("Control Debug : Read ToggleSwitch[");
  Serial.print(this->Pin1);
  Serial.print("] Stat : ");
  Serial.print(this->LastPinStatus != last ? "CHANGE !" : "NO CHANGE");
  Serial.print(", Value : ");
  Serial.print(this->LastPinStatus);
  Serial.println(".");
#endif

  // Value changed ?
  return this->LastPinStatus != last;
}

float ArduinoToggleSwitchControl::getValue() {
  return this->LastPinStatus == HIGH ? 1 : 0;
};
