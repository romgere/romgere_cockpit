#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>

//L'application
#include "RomgereCockpit/Application/CockpitMainApplication.h"
#include "RomgereCockpit/CommunicationInterface/EthernetInterface.h"

//Les contrôles
#include "RomgereCockpit/ArduinoControl/ArduinoRotaryEncoderControl.h"

//Config pour la carte 2
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

    //Crééer et "démarre" l'aplication principale
    cockpitApp = new CockpitMainApplication( ethernetInterface);

    //////////////////////
    // INSTRUMENT PANEL //
    //////////////////////
    //Alti
    cockpitApp->DeclareInputControl(    new ArduinoRotaryEncoderControl(23,22, ArduinoRotaryEncoderControl::Type3Encoder, true),
                                        new XPlaneSimpleCommand("sim/instruments/barometer_up"),
                                        new XPlaneSimpleCommand("sim/instruments/barometer_down"));
    //HSI heading (gauche/jaune)
    cockpitApp->DeclareInputControl(    new ArduinoRotaryEncoderControl(25,24, ArduinoRotaryEncoderControl::Type3Encoder, true),
                                        new XPlaneSimpleCommand("sim/radios/obs_HSI_up"),
                                        new XPlaneSimpleCommand("sim/radios/obs_HSI_down"));
    //HSI AP (droite/orange)
    cockpitApp->DeclareInputControl(    new ArduinoRotaryEncoderControl(26,27, ArduinoRotaryEncoderControl::Type3Encoder, true),
                                        new XPlaneSimpleCommand("sim/autopilot/heading_up"),
                                        new XPlaneSimpleCommand("sim/autopilot/heading_down"));
    //ADF 1 (gauche)
    cockpitApp->DeclareInputControl(    new ArduinoRotaryEncoderControl(29,28, ArduinoRotaryEncoderControl::Type3Encoder, true),
                                        new XPlaneSimpleCommand("sim/radios/adf1_card_up"),
                                        new XPlaneSimpleCommand("sim/radios/adf1_card_down"));
    //ADF 2 (droite)
    cockpitApp->DeclareInputControl(    new ArduinoRotaryEncoderControl(31,30, ArduinoRotaryEncoderControl::Type3Encoder, true),
                                        new XPlaneSimpleCommand("sim/radios/obs2_up"),
                                        new XPlaneSimpleCommand("sim/radios/obs2_down"));


    return;

}

void loop()
{
    cockpitApp->Loop();
}

