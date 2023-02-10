/**
 * Romgere Cockpit Library - https://github.com/romgere/romgere_cockpit
 * @author jerome@mestres.fr
 */

#ifndef ARDUINTOGGLESWITCHCLASS_H_INCLUDED
#define ARDUINTOGGLESWITCHCLASS_H_INCLUDED

#include <Arduino.h>

#include "../Config/MainConfig.h"
#include "ArduinoControl.h"

// Allow use of 2 positions toggle switch or push button for command who has not
// to be repeat
class ArduinoToggleSwitchControl : public ArduinoInputControl {

private:
  uint8_t Pin1;
  uint8_t LastPinStatus;

public:
#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE
  ArduinoToggleSwitchControl(uint8_t pin, int boardAddress = -1);
#else
  ArduinoToggleSwitchControl(uint8_t pin);
#endif
  ~ArduinoToggleSwitchControl();
  bool ReadInput();
  float getValue();
};

#endif // ARDUINTOGGLESWITCHCLASS_H_INCLUDED
