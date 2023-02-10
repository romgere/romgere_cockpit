/**
 * Romgere Cockpit Library - https://github.com/romgere/romgere_cockpit
 * @author jerome@mestres.fr
 */

#ifndef ARDUINOROTARYENCODECLASS_H_INCLUDED
#define ARDUINOROTARYENCODECLASS_H_INCLUDED

#include <Arduino.h>

#include "../Config/MainConfig.h"
#include "ArduinoControl.h"

#define ROTARY_ENC_CUR_STATUS_INDEX 0
#define ROTARY_ENC_OLD_STATUS_INDEX 1

// Allows use of rotary encoder
class ArduinoRotaryEncoderControl : public ArduinoInputControl {

public:
  // Encoder type
  typedef enum {
    Type1Encoder = 0,
    Type2Encoder,
    Type3Encoder,
    Type4Encoder
  } RotaryEncoderType;

  // Directions
  typedef enum {
    UnknowDirection = 0,
    UpDirection,
    DownDirection,
  } RotaryEncoderDirection;

private:
  uint8_t Pin1;
  uint8_t Pin2;

  // Keep the last 2 status
  uint8_t LastPinStatus[2];

#ifdef USE_SECURE_TIME_STATE_CHANGE
  unsigned long LastStatChangeTime; //(Soft debounce)
#endif                              // USE_SECURE_TIME_STATE_CHANGE

#ifdef USE_SECURE_TIME_DIR_CHANGE
  RotaryEncoderDirection LastDirection;
  unsigned long LastDirChangeTime; //(Soft debounce)
#endif                             // USE_SECURE_TIME_DIR_CHANGE

  RotaryEncoderType EncoderType;

public:
#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE
  ArduinoRotaryEncoderControl(uint8_t pin1, uint8_t pin2,
                              RotaryEncoderType type, bool usePullUpPin = false,
                              int boardAddress = -1);
#else
  ArduinoRotaryEncoderControl(uint8_t pin1, uint8_t pin2,
                              RotaryEncoderType type,
                              bool usePullUpPin = false);
#endif
  ~ArduinoRotaryEncoderControl();
  bool ReadInput();
  float getValue();
};

#endif // ARDUINOROTARYENCODECLASS_H_INCLUDED
