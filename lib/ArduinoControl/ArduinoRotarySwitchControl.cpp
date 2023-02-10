/**
 * Romgere Cockpit Library - https://github.com/romgere/romgere_cockpit
 * @author jerome@mestres.fr
 */

#include "ArduinoRotarySwitchControl.h"

#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE
ArduinoRotarySwitchControl::ArduinoRotarySwitchControl(uint8_t pin,
                                                       uint8_t positionCount,
                                                       int boardAddress)
    : ArduinoInputControl(DigitalControl, ITypeRotarySwitch,
                          (boardAddress != -1), boardAddress) {
#else
ArduinoRotarySwitchControl::ArduinoRotarySwitchControl(uint8_t pin,
                                                       uint8_t positionCount)
    : ArduinoInputControl(DigitalControl, ITypeRotarySwitch) {
#endif
  this->Pin1 = pin;

  _pinMode(this->Pin1, INPUT);

  this->positionCount = positionCount;

  // Default : no repeated value
  for (int i = 0; i < MAX_COMMAND_FOR_ONE_CONTROLE; i++)
    this->RepeatCmd[i] = false;

  // Step between 2 values & tolerance (gap allowed between read and expected
  // value)
  this->Step = 1024 / (this->positionCount - 1);
  this->Tolerance = this->Step * float(ROTARY_SWITCH_TOLERANCE);

  this->LastVal = -1;
  this->ReadInput();
}

ArduinoRotarySwitchControl::~ArduinoRotarySwitchControl() {}

ArduinoRotarySwitchControl *ArduinoRotarySwitchControl::setRepeat(uint8_t pos,
                                                                  bool repeat) {
  if (pos < MAX_COMMAND_FOR_ONE_CONTROLE) {
    this->RepeatCmd[pos] = repeat;
  }

  return this;
}

bool ArduinoRotarySwitchControl::ReadInput() {

  // Keep current value as old, and read new one
  uint8_t oldVal = this->LastVal;
  int currentAnalog = _analogRead(this->Pin1);
  for (uint8_t i = 0; i < this->positionCount; i++) {

    // Check each possible position and check if current value is in interval of
    // expected value +/- tolerance
    if (currentAnalog >= (int)((i * this->Step) - this->Tolerance) &&
        currentAnalog <= (int)((i * this->Step) + this->Tolerance)) {
      this->LastVal = i;
      break;
    }
  }

  // Value changed or repeat set for this position => return true to send
  // command
  if (this->LastVal != oldVal || this->RepeatCmd[this->LastVal])
    return true;

  return false;
}

float ArduinoRotarySwitchControl::getValue() { return this->LastVal; }
