#include <Arduino.h>
//#include <SPI.h>
//#include <Ethernet.h>
//#include <Wire.h>

#include "src/RomgereCockpit/Application/CockpitMainApplication.h"
#include "src/RomgereCockpit/CommunicationInterface/EthernetInterface.h"
#include "src/RomgereCockpit/ArduinoControl/ArduinoToggleSwitchControl.h"
#include "src/RomgereCockpit/ArduinoControl/ArduinoLEDControl.h"
#include "src/RomgereCockpit/ArduinoControl/ArduinoPushButtonControl.h"

CockpitMainApplication  *cockpitApp;
EthernetInterface       *ethernetInterface;

void setup()
{
  //Create & start Ethernet interface + Create app with our Ethernet interface
  byte arduinoMAC[6]  = { 0xDE, 0xAD, 0xBE, 0xEF, 0xEA, 0xED };
  ethernetInterface = new EthernetInterface( 49001, 49000, { 192, 168, 1, 97 }, arduinoMAC, { 192, 168, 1, 21 });
  cockpitApp = new CockpitMainApplication( ethernetInterface);

  //Declare and bind control
  cockpitApp->RegisterInputControl(
    new ArduinoPushButtonControl(8), //Create push button on PIN 8 on Master board
    new XPlaneSimpleCommand("sim/annunciator/test_all_annunciators") //Send "Test all annunciators" command to X-Plane
  );

  //Declare and bind control
  cockpitApp->RegisterOutputControl(
    new ArduinoLEDControl(8, 1), //Create LED Control on PIN n°8 on Slave board n°1
    new XPlaneInputData(67, 0)
  );

  //Declare and bind control
  cockpitApp->RegisterInputControl(
    new ArduinoToggleSwitchControl(9, 1), //Create toggle switch Control on PIN n°9 on Slave board n°1
    new XPlaneSimpleCommand("sim/systems/avionics_on"),
    new XPlaneSimpleCommand("sim/systems/avionics_off")
  );
}

void loop()
{
    cockpitApp->Loop();
}
