/**
 * Romgere Cockpit Library - https://github.com/romgere/romgere_cockpit
 * @author jerome@mestres.fr
 */

#ifndef ROMGERECOCKPITDEBUGINTERFACE_H_INCLUDED
#define ROMGERECOCKPITDEBUGINTERFACE_H_INCLUDED

#include <Arduino.h>

#include "../Config/MainConfig.h"
#include "BaseCommunicationInterface.h"

// Allows debuging (sent keys, commands or DREF commands) on the serial output
// of Arduino board
class SerialDebugInterface : public BaseCommunicationInterface {

private:
  XPData *FakeData;

public:
  SerialDebugInterface(int serialSpeed = 9600);
  ~SerialDebugInterface();

  uint8_t ReadAllInput() { return 0; }

  // Print command on arduino serial output
  void SendCommand(const char *cmd);

  // Print key on arduino serial output
  void SendKey(const char *key);

  // Print DREF command and his datas on arduino serial output
  void SendDrefCommand(const char *dref, byte data[]);

  // Get an empty group datas (4 x 0 float value)
  XPData *GetData(float group);
};

#endif // ROMGERECOCKPITDEBUGINTERFACE_H_INCLUDED
