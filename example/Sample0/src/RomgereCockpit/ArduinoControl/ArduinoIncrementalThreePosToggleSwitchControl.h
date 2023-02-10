/**
 * Romgere Cockpit Library - https://github.com/romgere/romgere_cockpit
 * @author jerome@mestres.fr
 */

#ifndef ARDUININCREMENTALTOGGLESWICTHCLASS_H_INCLUDED
#define ARDUININCREMENTALTOGGLESWICTHCLASS_H_INCLUDED

#include <Arduino.h>

#include "../Config/MainConfig.h"
#include "ArduinoControl.h"

#define INCREMENTAL_TOGGLESWITCH_CUR_STATUS_INDEX 0
#define INCREMENTAL_TOGGLESWITCH_OLD_STATUS_INDEX 1

// Allows use of 3 positions toggle switch with 2 incremental commands
// The command sent when the toggle is switch to middle position is deduce by
// previous position
class ArduinoIncrementalThreePosToggleSwitchControl
    : public ArduinoInputControl {

public:
private:
  uint8_t Pin1;
  uint8_t Pin2;

  uint8_t LastPinStatus[2];

public:
#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE
  ArduinoIncrementalThreePosToggleSwitchControl(uint8_t pin1, uint8_t pin2,
                                                int boardAddress = -1);
#else
  ArduinoIncrementalThreePosToggleSwitchControl(uint8_t pin1, uint8_t pin2);
#endif
  ~ArduinoIncrementalThreePosToggleSwitchControl();
  bool ReadInput();
  float getValue();
};

#endif // ARDUININCREMENTALTOGGLESWICTHCLASS_H_INCLUDED
