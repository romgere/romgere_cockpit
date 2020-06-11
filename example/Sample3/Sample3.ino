#include <Arduino.h>

#include "src/RomgereCockpit/Application/CockpitMainApplication.h"
#include "src/RomgereCockpit/CommunicationInterface/EthernetInterface.h"
#include "src/RomgereCockpit/CommunicationInterface/SerialDebugInterface.h"
#include "src/RomgereCockpit/ArduinoControl/ArduinoPotentiometerControl.h"

//Settings
#include "NetworkConfig.h"


CockpitMainApplication  *cockpitApp;
EthernetInterface       *ethernetInterface;


void setup()
{
/*
  byte arduinoMAC[6]  = ARDUINO_MAC_ADDRESS;

  //Create & start Ethernet interface
  ethernetInterface = new EthernetInterface(
    XPLANE_READ_PORT,
    XPLANE_WRITE_PORT,
    ARDUINO_IP_ADDRESS,
    arduinoMAC,
    XPLANE_IP_ADDRESS,
    WAIT_FOR_XPLANE_AT_STARTUP
  );
  
  //Create app with our Eternet interface
  cockpitApp = new CockpitMainApplication( ethernetInterface);
*/
  
  cockpitApp = new CockpitMainApplication( new SerialDebugInterface());

  
  
  cockpitApp->RegisterInputControl(    new ArduinoPotentiometerControl(5, 0.05, 0, 1),
                                      new XPlaneDREFCommand("parking_brake_ratio[0]"),
                                      new XPlaneDREFCommand("parking_brake_ratio[1]"),
                                      new XPlaneDREFCommand("parking_brake_ratio[2]"));
}

void loop()
{
  cockpitApp->Loop();
}
