#include <Arduino.h>

#include "src/RomgereCockpit/Application/CockpitMainApplication.h"
#include "src/RomgereCockpit/CommunicationInterface/EthernetInterface.h"
#include "src/RomgereCockpit/ArduinoControl/ArduinoRotaryEncoderControl.h"

//Settings
#include "NetworkConfig.h"


CockpitMainApplication  *cockpitApp;
EthernetInterface       *ethernetInterface;


void setup()
{

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

  //////////////////////
  // INSTRUMENT PANEL //
  //////////////////////
  //Alti
  cockpitApp->RegisterInputControl(    new ArduinoRotaryEncoderControl(23,22, ArduinoRotaryEncoderControl::Type3Encoder, true),
                                      new XPlaneSimpleCommand("sim/instruments/barometer_up"),
                                      new XPlaneSimpleCommand("sim/instruments/barometer_down"));
  //HSI heading (gauche/jaune)
  cockpitApp->RegisterInputControl(    new ArduinoRotaryEncoderControl(25,24, ArduinoRotaryEncoderControl::Type3Encoder, true),
                                      new XPlaneSimpleCommand("sim/radios/obs_HSI_up"),
                                      new XPlaneSimpleCommand("sim/radios/obs_HSI_down"));
  //HSI AP (droite/orange)
  cockpitApp->RegisterInputControl(    new ArduinoRotaryEncoderControl(26,27, ArduinoRotaryEncoderControl::Type3Encoder, true),
                                      new XPlaneSimpleCommand("sim/autopilot/heading_up"),
                                      new XPlaneSimpleCommand("sim/autopilot/heading_down"));
  //ADF 1 (left)
  cockpitApp->RegisterInputControl(    new ArduinoRotaryEncoderControl(29,28, ArduinoRotaryEncoderControl::Type3Encoder, true),
                                      new XPlaneSimpleCommand("sim/radios/adf1_card_up"),
                                      new XPlaneSimpleCommand("sim/radios/adf1_card_down"));
  //ADF 2 (right)
  cockpitApp->RegisterInputControl(    new ArduinoRotaryEncoderControl(31,30, ArduinoRotaryEncoderControl::Type3Encoder, true),
                                      new XPlaneSimpleCommand("sim/radios/obs2_up"),
                                      new XPlaneSimpleCommand("sim/radios/obs2_down"));
}

void loop()
{
  cockpitApp->Loop();
}
