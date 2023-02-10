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

#ifdef DEBUG_ETHERNET
    Serial.println("Ethernet debug : Start Ethernet using DHCP...");
#endif // DEBUG

    res = Ethernet.begin(arduinoMAC);

#ifdef DEBUG_ETHERNET
    if (res == 0) {
      Serial.println(
          "Ethernet debug : Failed to configure Ethernet using DHCP");
    } else {
      Serial.print("Ethernet debug : Ehernet OK, IP address from DHCP : ");
      for (byte thisByte = 0; thisByte < 4; thisByte++) {
        Serial.print(Ethernet.localIP()[thisByte], DEC);
        Serial.print(".");
      }
      Serial.println();
    }
#endif // DEBUG_ETHERNET

    // DHCP error ! Can't do any other util stuff
    if (res == 0) {
      for (;;) {
      }
    }
  }
  // Fixed IP
  else {
#ifdef DEBUG_ETHERNET
    Serial.println("Ethernet debug : Start Ethernet using Static IP Address");
#endif

    Ethernet.begin(arduinoMAC, arduinoIP);

#ifdef DEBUG_ETHERNET
    Serial.println("Ethernet debug : Ethernet Start.");
#endif
  }

  // Start listening on UDP
  res = this->Udp.begin(this->XplaneReadPort);

#ifdef DEBUG_ETHERNET
  if (res == 0) {
    Serial.println("Ethernet debug : Failed to start UDP");
  } else {
    Serial.println("Ethernet debug : UDP start");
  }
#endif // DEBUG_ETHERNET

  // UDP error ! Can't do any other util stuff
  if (res == 0) {
    for (;;) {
    }
  }
  this->IsClassInit = true;

  // Wait for a first exchange with X-Plane
  if (waitForXPlane) {

#ifdef DEBUG_ETHERNET
    Serial.println("Ethernet debug : Start wait for Xplane...");
#endif // DEBUG

    while (!this->IsXPlaneAdressInit) {
      this->ReadAllInput();

#ifdef DEBUG_ETHERNET
      if (this->IsXPlaneAdressInit) {
        Serial.print("Ethernet debug : Ethernet debug : OK, data received from "
                     "Xplane, IP of Xplane is : ");
        for (byte thisByte = 0; thisByte < 4; thisByte++) {
          Serial.print(this->XPlaneAdress[thisByte], DEC);
          Serial.print(".");
        }
        Serial.println();
      } else
        Serial.println(
            "Ethernet debug : Nothing received from Xplane still waiting...");
#endif // DEBUG

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

#ifdef DEBUG_ETHERNET
    Serial.print("Ethernet debug : Received ");
    Serial.print(readSize);
    Serial.println(" byte(s) from Xplane, parsing...");
#endif // DEBUG

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

        XPData *p = new XPData();
        // First byte : Data's group
        p->group = buffer[i];

#ifdef DEBUG_ETHERNET
        Serial.print("Ethernet debug : Receive data for group ");
        Serial.print(p->group);
        Serial.print(" with value [");
#endif

        // Datas : table of 8 float value (8 x 4 bytes)
        for (int j = 0; j < 8; j++) {

          XPGroupData tmpData;

          for (int k = 0; k < 4; k++) {
            tmpData.byteVal[k] = buffer[i + 4 + (j * 4) + k];
          }
          // Convert 4 byte array to float with XPGroupData union
          p->data[j] = tmpData.floatVal;

#ifdef DEBUG_ETHERNET
          Serial.print(p->data[j]);
          Serial.print(",");
#endif
        }

#ifdef DEBUG_ETHERNET
        Serial.println("].");
#endif

        this->LastXPlaneDatas[index] = p;

        index++;
      }

      // Return the number of data group read (if 0 no output will be modified)
      return index;
    }

#ifdef DEBUG_ETHERNET
    Serial.println(
        "Ethernet debug : Data received not start with \"DATA>\" : ");
    Serial.println("---------------START------------------");
    for (int i = 0; i < readSize; i++)
      Serial.print((char)buffer[i]);
    Serial.println("----------------END-------------------");
#endif
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

#ifdef DEBUG_ETHERNET
  int res = this->Udp.endPacket();
  if (res)
    Serial.print("Ethernet debug : Send key \"");
  else
    Serial.print("Ethernet debug : Error sending key \"");

  Serial.print(key);
  Serial.println("\" to Xplane.");
#else
  this->Udp.endPacket();
#endif
}

// Send command to X-Plane
void EthernetInterface::SendCommand(const char *cmd) {

  if (!this->IsClassInit)
    return;

  this->Udp.beginPacket(this->XPlaneAdress, this->XPlaneWritePort);
  this->Udp.write("CMND0");
  this->Udp.write(cmd);

#ifdef DEBUG_ETHERNET
  int res = this->Udp.endPacket();
  if (res)
    Serial.print("Ethernet debug : Send command \"");
  else
    Serial.print("Ethernet debug : Error sending command \"");

  Serial.print(cmd);
  Serial.println("\" to Xplane.");
#else
  this->Udp.endPacket();
#endif
}

// Send DREF command to X-Plane
/*
void EthernetInterface::SendDrefCommand( const char *dref, byte data[]){

    if( ! this->IsClassInit )
        return;

        this->Udp.beginPacket( this->XPlaneAdress, this->XPlaneWritePort);
    this->Udp.write("DREF0");

#ifdef DEBUG_ETHERNET
        Serial.print("Send DREF command [");
#endif
    //Envoi des données
    for (int i=0; i<4; i++){
        this->Udp.write( data[i]);

#ifdef DEBUG_ETHERNET
            Serial.print( data[i]);
            if( i < 3 ) Serial.print(",");
#endif
    }

    //Donnée à envoyer
    char DataOut[500];

    //"sim/"
    DataOut[0] = 's';
    DataOut[1] = 'i';
    DataOut[2] = 'm';
    DataOut[3] = '/';

    int i = 0;
    while( dref[i] != 0 && i < 499 ){
        DataOut[i+4] = dref[i];
        i++;
    }
    for(; i < 498; i++  ){
        DataOut[i] = char(32);
    }
    DataOut[499] = 0;

#ifdef DEBUG_ETHERNET
    Serial.print("] with command \"");
    Serial.print(DataOut);
    Serial.print("\" : ");
#endif

    int res = this->Udp.write(DataOut);

#ifdef DEBUG_ETHERNET
    if( res == 500 )
        Serial.println("succes");
    else
        Serial.println("error");
#endif // DEBUG


    this->Udp.endPacket();
}

*/

// Get a datas received for a given group number
XPData *EthernetInterface::GetData(float group) {

  for (int i = 0; i < MAX_INPUT_DATA_FROM_XPLANE; i++) {
    if (this->LastXPlaneDatas[i] != NULL &&
        this->LastXPlaneDatas[i]->group == group) {
      return this->LastXPlaneDatas[i];
    }
  }

  return NULL;
}
