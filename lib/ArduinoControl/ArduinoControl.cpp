/**
 * Romgere Cockpit Library - https://github.com/romgere/romgere_cockpit
 * @author jerome@mestres.fr
 */

#include "ArduinoControl.h"
#include "../Config/MainConfig.h"

#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE

#include "../Misc/MasterToSlaveCommand.h"

void ArduinoControl::_pinMode(uint8_t p, uint8_t m) {

  if (this->ControleOnSlaveBoard) {

    // Create and send I2C command to init PIN on slave board
    MasterToSlaveCommand slaveCMD(
        MasterToSlaveCommand::TypeCommandInitialisation,
        (m == INPUT
             ? MasterToSlaveCommand::PINModeInput
             : (m == INPUT_PULLUP ? MasterToSlaveCommand::PINModeIntputPullUp
                                  : MasterToSlaveCommand::PINModeOutput)),
        MasterToSlaveCommand::RWModeNotUsed, p, 0);
    slaveCMD.SendDataToIC2(SlaveBoardAddress);

  } else {
    pinMode(p, m);
  }
}

void ArduinoControl::_digitalWrite(uint8_t p, uint8_t v) {

  if (ControleOnSlaveBoard) {

    // Create and send I2C command to write PIN on slave board
    MasterToSlaveCommand slaveCMD(MasterToSlaveCommand::TypeCommandSetPINValue,
                                  MasterToSlaveCommand::PINModeNotUsed,
                                  MasterToSlaveCommand::RWModeDigital, p, v);
    slaveCMD.SendDataToIC2(SlaveBoardAddress);

  } else {
    digitalWrite(p, v);
  }
}

int ArduinoControl::_digitalRead(uint8_t p) {
  if (ControleOnSlaveBoard) {

    // Create I2C command to request a PIN read on slave board
    MasterToSlaveCommand slaveCMD(MasterToSlaveCommand::TypeCommandGetPINValue,
                                  MasterToSlaveCommand::PINModeNotUsed,
                                  MasterToSlaveCommand::RWModeDigital, p, 0);

    // Send command and wait for the answer
    return slaveCMD.RequestDataFromIC2(SlaveBoardAddress);
  } else {
    return digitalRead(p);
  }
}

int ArduinoControl::_analogRead(uint8_t p) {

  if (ControleOnSlaveBoard) {
    // Create I2C command to request a PIN read on slave board
    MasterToSlaveCommand slaveCMD(MasterToSlaveCommand::TypeCommandGetPINValue,
                                  MasterToSlaveCommand::PINModeNotUsed,
                                  MasterToSlaveCommand::RWModeAnalog, p, 0);

    // Send command and wait for the answer
    return slaveCMD.RequestDataFromIC2(SlaveBoardAddress);

  } else {
    return analogRead(p);
  }
}

void ArduinoControl::_analogWrite(uint8_t p, int v) {

  if (ControleOnSlaveBoard) {
    // Create and send I2C command to write PIN on slave board
    MasterToSlaveCommand slaveCMD(MasterToSlaveCommand::TypeCommandSetPINValue,
                                  MasterToSlaveCommand::PINModeNotUsed,
                                  MasterToSlaveCommand::RWModeAnalog, p, v);
    slaveCMD.SendDataToIC2(SlaveBoardAddress);

  } else {
    analogWrite(p, v);
  }
}

#endif // ACTIVE_MULTI_ARDUINO_BOARD_MODE
