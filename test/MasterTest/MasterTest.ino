#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include <Wire.h>

//L'application + Interface Ehternet
#include "src/RomgereCockpit/Application/CockpitMainApplication.h"
#include "src/RomgereCockpit/CommunicationInterface/EthernetInterface.h"
//Les contrôles
#include "src/RomgereCockpit/ArduinoControl/ArduinoToggleSwitchControl.h"
#include "src/RomgereCockpit/ArduinoControl/ArduinoRotaryEncoderControl.h"



//Config Réseau de la carte
#include "NetworkConfig.h"



CockpitMainApplication  *cockpitApp;
EthernetInterface       *ethernetInterface;


void setup()
{ 

    byte arduinoMAC[6]              = ARDUINO_MAC_ADDRESS;
    byte arduinoIP[4]               = ARDUINO_IP_ADDRESS;
    byte xplaneIP[4]                = XPLANE_IP_ADDRESS;

    //Démarre l'interface Ethernet pour XPlane
    ethernetInterface = new EthernetInterface(XPLANE_READ_PORT, XPLANE_WRITE_PORT, arduinoIP, arduinoMAC, xplaneIP, WAIT_FOR_XPLANE_AT_STARTUP);

    //Créé et "démarre" l'aplication principale
    cockpitApp = new CockpitMainApplication( ethernetInterface);



    //Sur carte esclave #1, PIN 39 ==> Avionic ON/OFF

    cockpitApp->RegisterInputControl(    new ArduinoToggleSwitchControl(40, 1),
                                        new XPlaneSimpleCommand("sim/systems/avionics_on"),
                                        new XPlaneSimpleCommand("sim/systems/avionics_off"));

    cockpitApp->RegisterInputControl(    new ArduinoRotaryEncoderControl(22,23, ArduinoRotaryEncoderControl::Type3Encoder, true, 1),
                                        new XPlaneSimpleCommand("sim/instruments/barometer_up"),
                                        new XPlaneSimpleCommand("sim/instruments/barometer_down"));
    //HSI heading (gauche/jaune)
    cockpitApp->RegisterInputControl(    new ArduinoRotaryEncoderControl(24,25, ArduinoRotaryEncoderControl::Type3Encoder, true, 1),
                                        new XPlaneSimpleCommand("sim/radios/obs_HSI_up"),
                                        new XPlaneSimpleCommand("sim/radios/obs_HSI_down"));
    //HSI AP (droite/orange)
    cockpitApp->RegisterInputControl(    new ArduinoRotaryEncoderControl(26,27, ArduinoRotaryEncoderControl::Type3Encoder, true, 1),
                                        new XPlaneSimpleCommand("sim/autopilot/heading_up"),
                                        new XPlaneSimpleCommand("sim/autopilot/heading_down"));
    //ADF 1 (gauche)
    cockpitApp->RegisterInputControl(    new ArduinoRotaryEncoderControl(28,29, ArduinoRotaryEncoderControl::Type3Encoder, true, 1),
                                        new XPlaneSimpleCommand("sim/radios/adf1_card_up"),
                                        new XPlaneSimpleCommand("sim/radios/adf1_card_down"));
    //ADF 2 (droite)
    cockpitApp->RegisterInputControl(    new ArduinoRotaryEncoderControl(30,31, ArduinoRotaryEncoderControl::Type3Encoder, true, 1),
                                        new XPlaneSimpleCommand("sim/radios/obs2_up"),
                                        new XPlaneSimpleCommand("sim/radios/obs2_down"));



}

void loop()
{
    cockpitApp->Loop();
}
