/**
 * Romgere Cockpit Library - https://github.com/romgere/romgere_cockpit
 * @author jerome@mestres.fr
 */

#ifndef XPLANENETWORKINGCLASS_H_INCLUDED
#define XPLANENETWORKINGCLASS_H_INCLUDED

#include <Arduino.h>
#include <Ethernet.h>

#include "../Config/MainConfig.h"
#include "BaseCommunicationInterface.h"

// Allows to send and received X-Plane datas on an ethernet network (Need a
// board with an ethernet Shield)
class EthernetInterface : public BaseCommunicationInterface {

private:
  typedef union {
    byte byteVal[4];
    float floatVal;
  } XPNetworkData;

  // Last datas read from X-Plane
  XPGroupDatas *LastXPlaneDatas[MAX_INPUT_DATA_FROM_XPLANE];

  bool IsClassInit;

  bool IsXPlaneAdressInit;
  IPAddress XPlaneAdress;

  unsigned int XPlaneWritePort;
  unsigned int XplaneReadPort;

  EthernetUDP Udp;

public:
  EthernetInterface(unsigned int readPort, unsigned int writePort,
                    IPAddress arduinoIP, uint8_t arduinoMAC[6],
                    IPAddress xplaneIP, bool waitForXPlane = false);
  ~EthernetInterface();

  // Read and parse datas received from X-Plane. Return : Number of packet read
  // from x-Plane ()
  uint8_t ReadAllInput();

  // Send command to X-Plane
  void SendCommand(const char *cmd);

  // Send a key to X-Plane
  void SendKey(const char *key);

  // Send DREF command to X-Plane
  void SendDrefCommand(const char *dref, float value);

  // Get a datas received for a given group number
  XPGroupDatas *GetData(float group);
};

#endif // XPLANENETWORKINGCLASS_H_INCLUDED
