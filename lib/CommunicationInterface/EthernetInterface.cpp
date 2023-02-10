/**
 * Romgere Cockpit Library - https://github.com/romgere/romgere_cockpit
 * @author jerome@mestres.fr
 */

#include "EthernetInterface.h"

EthernetInterface::EthernetInterface(unsigned int readPort,
                                     unsigned int writePort,
                                     IPAddress arduinoIP, uint8_t arduinoMAC[6],
                                     IPAddress xplaneIP, bool waitForXPlane) {

  this->IsClassInit = false;

  for (int i = 0; i < MAX_INPUT_DATA_FROM_XPLANE; i++) {
    this->LastXPlaneDatas[i] = NULL;
  }

  this->XPlaneWritePort = writePort;
  this->XplaneReadPort = readPort;
  this->IsXPlaneAdressInit = false;

  // DHCP : Unknow adress on board initialisation
  if (xplaneIP[0] == 0)
    XPlaneAdress = INADDR_NONE;
  else {
    XPlaneAdress = xplaneIP;
    IsXPlaneAdressInit = true;
  }

  int res = 0;

  // DHCP
  if (arduinoIP[0] == 0) {

    res = Ethernet.begin(arduinoMAC);

    // DHCP error ! Can't do any other util stuff
    if (res == 0) {
      for (;;) {
      }
    }
  }
  // Fixed IP
  else {
    Ethernet.begin(arduinoMAC, arduinoIP);
  }

  // Start listening on UDP
  res = this->Udp.begin(this->XplaneReadPort);

  // UDP error ! Can't do any other util stuff
  if (res == 0) {
    for (;;) {
    }
  }
  this->IsClassInit = true;

  // Wait for a first exchange with X-Plane
  if (waitForXPlane) {

    while (!this->IsXPlaneAdressInit) {
      this->ReadAllInput();

      if (!this->IsXPlaneAdressInit)
        delay(250);
    }
  }
}

EthernetInterface::~EthernetInterface() {}

// Read and parse datas received from X-Plane. Return : Number of packet read
// from x-Plane ()
uint8_t EthernetInterface::ReadAllInput() {

  // Pas initialisée
  if (!this->IsClassInit)
    return 0;

  int readSize = this->Udp.parsePacket();
  if (readSize) {

    // First reception : keep X-Plane remote address
    if (!this->IsXPlaneAdressInit) {
      this->XPlaneAdress = this->Udp.remoteIP();
      this->IsXPlaneAdressInit = true;
    }

    byte buffer[readSize];
    this->Udp.read(buffer, readSize);

    // Dats from X-Plane start with "DATA>"
    // ref : http://svglobe.com/arduino/udpdata.html
    if (buffer[0] == 'D' && buffer[1] == 'A' && buffer[2] == 'T' &&
        buffer[3] == 'A') {

      // Reset previous datas
      for (int i = 0; i < MAX_INPUT_DATA_FROM_XPLANE; i++) {
        if (this->LastXPlaneDatas[i] != NULL) {
          delete this->LastXPlaneDatas[i];
        }

        this->LastXPlaneDatas[i] = NULL;
      }

      uint8_t index = 0;
      // Parse received data (By 36 Bytes packet, length of X-Plane data )
      for (int i = 5; i < readSize; i += 36) {

        XPGroupDatas *p = new XPGroupDatas();
        // First byte : Data's group
        p->group = buffer[i];

        // Datas : table of 8 float value (8 x 4 bytes)
        for (int j = 0; j < 8; j++) {

          XPNetworkData tmpData;

          for (int k = 0; k < 4; k++) {
            tmpData.byteVal[k] = buffer[i + 4 + (j * 4) + k];
          }
          // Convert 4 byte array to float with XPNetworkData union
          p->data[j] = tmpData.floatVal;
        }

        this->LastXPlaneDatas[index] = p;

        index++;
      }

      // Return the number of data group read (if 0 no output will be modified)
      return index;
    }
  }

  return 0;
}

// Send a key to X-Plane
void EthernetInterface::SendKey(const char *key) {

  if (!this->IsClassInit)
    return;

  this->Udp.beginPacket(this->XPlaneAdress, this->XPlaneWritePort);
  this->Udp.write("CHAR0");
  this->Udp.write((char)*key);
  this->Udp.write("");
  this->Udp.endPacket();
}

// Send command to X-Plane
void EthernetInterface::SendCommand(const char *cmd) {

  if (!this->IsClassInit)
    return;

  this->Udp.beginPacket(this->XPlaneAdress, this->XPlaneWritePort);
  this->Udp.write("CMND0");
  this->Udp.write(cmd);
  this->Udp.endPacket();
}

// Send DREF command to X-Plane
void EthernetInterface::SendDrefCommand(const char *dref, float value) {

  if (!this->IsClassInit)
    return;

  // Convert DREF value
  XPNetworkData tmpData;
  tmpData.floatVal = value;

  // build DREF name
  char DataOut[497];
  int i = 0;
  while (dref[i] != 0 && i < 496) {
    DataOut[i] = dref[i];
    i++;
  }
  for (; i < 496; i++) {
    DataOut[i] = char(32);
  }
  DataOut[i] = 0;

  // Send datas
  this->Udp.beginPacket(this->XPlaneAdress, this->XPlaneWritePort);
  this->Udp.write("DREF0");
  for (int i = 0; i < 4; i++) {
    this->Udp.write(tmpData.byteVal[i]);
  }
  this->Udp.write("sim/");
  this->Udp.write(DataOut);
  this->Udp.endPacket();
}

// Get a datas received for a given group number
XPGroupDatas *EthernetInterface::GetData(float group) {

  for (int i = 0; i < MAX_INPUT_DATA_FROM_XPLANE; i++) {
    if (this->LastXPlaneDatas[i] != NULL &&
        this->LastXPlaneDatas[i]->group == group) {
      return this->LastXPlaneDatas[i];
    }
  }

  return NULL;
}
