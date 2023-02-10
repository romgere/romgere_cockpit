/**
 * Romgere Cockpit Library - https://github.com/romgere/romgere_cockpit
 * @author jerome@mestres.fr
 */

#ifndef ARDUINPUSHBUTTONCLASS_H_INCLUDED
#define ARDUINPUSHBUTTONCLASS_H_INCLUDED

#include <Arduino.h>

#include "../Config/MainConfig.h"
#include "ArduinoControl.h"

// Allows use of push button
class ArduinoPushButtonControl : public ArduinoInputControl {

private:
  uint8_t Pin1;

  // Default/initial PIN Status.
  // For this control, "ReadInput" return true (send command) on each loop
  // when his current status has been changed from initial status
  uint8_t InitialPinStatus;

public:
#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE
  ArduinoPushButtonControl(uint8_t pin, uint8_t defautStat = LOW,
                           int boardAddress = -1);
#else
  ArduinoPushButtonControl(uint8_t pin, uint8_t defautStat = LOW);
#endif
  ~ArduinoPushButtonControl(){};
  bool ReadInput();
  float getValue() {
    return 0;
  } // This control always send his first (and only) command
};

#endif // ARDUINPUSHBUTTONCLASS_H_INCLUDED
