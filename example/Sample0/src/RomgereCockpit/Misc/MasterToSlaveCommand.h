/**
 * Romgere Cockpit Library - https://github.com/romgere/romgere_cockpit
 * @author jerome@mestres.fr
 */

#ifndef MASTERTOSLAVECLASS_H_INCLUDED

#define MASTERTOSLAVECLASS_H_INCLUDED

#include <Arduino.h>

#include "../Config/MainConfig.h"

// For convertion between integer and the 2 bytes to send/received via I2C bus
typedef union {
  byte byteVal[4];
  int intVal;
} I2CDataRCA;

// Allows exchange between Master and Slave board on I2C bus.
//  First byte       : action + PINMode + Read/Write mode
//  Second byte      : PIN Number
//  Third and fourth : Value (write action)
class MasterToSlaveCommand {

public:
  // Command Types
  typedef enum {
    TypeCommandUnknow = 0,
    TypeCommandInitialisation, // Init a PIN
    TypeCommandSetPINValue,    // Write PIN on slave board
    TypeCommandGetPINValue     // Read PIN on slave board
  } MSCTypeCommand;

  // PIN Modes
  typedef enum {
    PINModeNotUsed = 0, // For get/set command the PIN mode is unused
    PINModeInput,
    PINModeOutput,
    PINModeIntputPullUp
  } MSCPINMode;

  // Read/Write Modes
  typedef enum {
    RWModeNotUsed = 0, // For init command the R/W mode is unused
    RWModeAnalog,
    RWModeDigital
  } MSCRWMode;

  MSCTypeCommand TypeCommande;
  MSCPINMode PinMode;
  MSCRWMode RWMode;
  uint8_t PinNum;
  int ValueToSet;

  MasterToSlaveCommand()
      : TypeCommande(TypeCommandUnknow), PinMode(PINModeNotUsed),
        RWMode(RWModeNotUsed), PinNum(0), ValueToSet(0){};
  MasterToSlaveCommand(MSCTypeCommand type, MSCPINMode pin_mode,
                       MSCRWMode rw_mode, uint8_t pin, int value)
      : TypeCommande(type), PinMode(pin_mode), RWMode(rw_mode), PinNum(pin),
        ValueToSet(value){};
  ~MasterToSlaveCommand(){};

  // Parse datas from "data" buffer (received from I2C bus) and init current
  // command attributes
  void ParseDataFromIC2(byte *data);

  // Send current command to Slave board on I2C bus (used for "SetPIN" command)
  void SendDataToIC2(uint8_t boardAddress);

  // Send a "GetPIN" command to slave board and wait return of slave board on
  // I2C bus
  int RequestDataFromIC2(uint8_t boardAddress);

private:
  // Fill "data" buffer with bytes defining current command
  void CreateBufferForI2C(byte *data);
};

#endif
