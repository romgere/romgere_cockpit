/**
 * Romgere Cockpit Library - https://github.com/romgere/romgere_cockpit
 * @author jerome@mestres.fr
 */

#ifndef ROMGERECOCKPITCOMMUNICATIONINTERFACE_H_INCLUDED
#define ROMGERECOCKPITCOMMUNICATIONINTERFACE_H_INCLUDED

#include "../Config/MainConfig.h"
#include <Arduino.h>

// An X-Plane group data
typedef struct {
  byte group;
  float data[8];
} XPGroupDatas;

// Parent class to deal with communication (input/output) between Arduino Board
// and X-Plane Used by "EthernetInterface" and "SerialDebugInterface"
class BaseCommunicationInterface {

public:
  BaseCommunicationInterface(){};
  virtual ~BaseCommunicationInterface(){};

  // Read and parse datas received from X-Plane. Return : Number of packet read
  // from x-Plane ()
  virtual uint8_t ReadAllInput() = 0;

  // Send command to X-Plane
  virtual void SendCommand(const char *cmd) = 0;

  // Send a key to X-Plane
  virtual void SendKey(const char *key) = 0;

  // Send DREF command to X-Plane
  virtual void SendDrefCommand(const char *dref,
                               float value); // Not pure virtual because not
                                             // implemented by EthernetInterface

  // Get a datas received for a given group number
  virtual XPGroupDatas *GetData(float group) = 0;
};

#endif // ROMGERECOCKPITCOMMUNICATIONINTERFACE_H_INCLUDED
