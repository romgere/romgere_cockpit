#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
//#include <Wire.h>

//L'application
#include "src/RomgereCockpit/Application/CockpitMainApplication.h"
#include "src/RomgereCockpit/CommunicationInterface/EthernetInterface.h"
//Les contrôles
#include "src/RomgereCockpit/ArduinoControl/AllControlInclude.h"


//Config pour la carte 1
#include "NetworkConfig.h"
//Les commande XPlane ou FSX
#include "CommandeRefXplaneFSX.h"
//Les fonction de transformation pour Xplane ou FSX
#include "TransformationFunction.h"


void setupControlOnBoard1();

//Mode XPLANE ou FSX ?
bool arduinoRunForFSX = false;


CockpitMainApplication  *cockpitApp;
EthernetInterface       *ethernetInterface;


void setup()
{

    byte arduinoMAC[6]              = ARDUINO_MAC_ADDRESS;
    byte arduinoIP[4]               = ARDUINO_IP_ADDRESS;
    byte xplaneIP[4]                = XPLANE_IP_ADDRESS;

    //Démarre l'interface Ethernet pour XPlane
    ethernetInterface = new EthernetInterface(XPLANE_READ_PORT, XPLANE_WRITE_PORT, arduinoIP, arduinoMAC, xplaneIP, WAIT_FOR_XPLANE_AT_STARTUP);

    //Crééer et "démarre" l'application principale
    cockpitApp = new CockpitMainApplication( ethernetInterface);

    ///////////////////////
    // ANNUN TEST BUTTON //
    ///////////////////////
    cockpitApp->RegisterInputControl(    new ArduinoPushButtonControl(40),
                                        //new XPlaneSimpleCommand("sim/annunciator/test_all_annunciators")
                                        new LibrarySpecialCommand(LibrarySpecialCommand::SendAll));

    /////////////
    // STARTER //
    /////////////
    ArduinoRotarySwitchControl *a;
    ArduinoRotarySwitchControl *b;
    cockpitApp->RegisterInputControl(    (a = new ArduinoRotarySwitchControl(9, 5)),
                                        new XPlaneSimpleCommand(commandTab[magnetos_off_1][arduinoRunForFSX]),
                                        new XPlaneSimpleCommand(commandTab[magnetos_right_1][arduinoRunForFSX]),
                                        new XPlaneSimpleCommand(commandTab[magnetos_left_1][arduinoRunForFSX]),
                                        new XPlaneSimpleCommand(commandTab[magnetos_both_1][arduinoRunForFSX]),
                                        new XPlaneSimpleCommand(commandTab[engage_starter_1][arduinoRunForFSX]));

    cockpitApp->RegisterInputControl(    (b = new ArduinoRotarySwitchControl(8, 5)),
                                        new XPlaneSimpleCommand(commandTab[magnetos_off_2][arduinoRunForFSX]),
                                        new XPlaneSimpleCommand(commandTab[magnetos_right_2][arduinoRunForFSX]),
                                        new XPlaneSimpleCommand(commandTab[magnetos_left_2][arduinoRunForFSX]),
                                        new XPlaneSimpleCommand(commandTab[magnetos_both_2][arduinoRunForFSX]),
                                        new XPlaneSimpleCommand(commandTab[engage_starter_2][arduinoRunForFSX]));

    //Pour XPlane on répête la commande de démarrage tant que le Rotary switch reste dans la position 4
    if( ! arduinoRunForFSX ){
        a->setRepeat(4);
        b->setRepeat(4);
    }


    //////////////////
    // LIGHT SWITCH //
    //////////////////
    cockpitApp->RegisterInputControl(    new ArduinoToggleSwitchControl(27),
                                        new XPlaneSimpleCommand(commandTab[landing_lights_on][arduinoRunForFSX]),
                                        new XPlaneSimpleCommand(commandTab[landing_lights_off][arduinoRunForFSX]));
    cockpitApp->RegisterInputControl(    new ArduinoToggleSwitchControl(25),
                                        new XPlaneSimpleCommand(commandTab[taxi_lights_on][arduinoRunForFSX]),
                                        new XPlaneSimpleCommand(commandTab[taxi_lights_off][arduinoRunForFSX]));
    cockpitApp->RegisterInputControl(    new ArduinoToggleSwitchControl(31),
                                        new XPlaneSimpleCommand(commandTab[strobe_lights_on][arduinoRunForFSX]),
                                        new XPlaneSimpleCommand(commandTab[strobe_lights_off][arduinoRunForFSX]));
    cockpitApp->RegisterInputControl(    new ArduinoToggleSwitchControl(37),
                                        new XPlaneSimpleCommand(commandTab[nav_lights_on][arduinoRunForFSX]),
                                        new XPlaneSimpleCommand(commandTab[nav_lights_off][arduinoRunForFSX]));
    cockpitApp->RegisterInputControl(    new ArduinoToggleSwitchControl(29),
                                        new XPlaneSimpleCommand(commandTab[beacon_lights_on][arduinoRunForFSX]),
                                        new XPlaneSimpleCommand(commandTab[beacon_lights_off][arduinoRunForFSX]));
    if( arduinoRunForFSX ){

        cockpitApp->RegisterInputControl(    new ArduinoToggleSwitchControl(33),
                                            new XPlaneSimpleCommand("PANEL_LIGHTS_ON"),
                                            new XPlaneSimpleCommand("PANEL_LIGHTS_OFF"));

    }
    //Manque  ICE, FLOOD et (PANEL) => Pas trouvé les commande !! :(

    ///////////////////
    // MASTER SWITCH //
    ///////////////////
    cockpitApp->RegisterInputControl(    new ArduinoToggleSwitchControl(39),
                                        new XPlaneSimpleCommand(commandTab[avionics_on][arduinoRunForFSX]),
                                        new XPlaneSimpleCommand(commandTab[avionics_off][arduinoRunForFSX]));
    cockpitApp->RegisterInputControl(    new ArduinoToggleSwitchControl(47),
                                        new XPlaneSimpleCommand(commandTab[battery_1_on][arduinoRunForFSX]),
                                        new XPlaneSimpleCommand(commandTab[battery_1_off][arduinoRunForFSX]));
    cockpitApp->RegisterInputControl(    new ArduinoToggleSwitchControl(45),
                                        new XPlaneSimpleCommand(commandTab[generator_1_on][arduinoRunForFSX]),
                                        new XPlaneSimpleCommand(commandTab[generator_1_off][arduinoRunForFSX]));
    cockpitApp->RegisterInputControl(    new ArduinoToggleSwitchControl(41),
                                        new XPlaneSimpleCommand(commandTab[generator_2_on][arduinoRunForFSX]),
                                        new XPlaneSimpleCommand(commandTab[generator_2_off][arduinoRunForFSX]));
    cockpitApp->RegisterInputControl(    new ArduinoToggleSwitchControl(43),
                                        new XPlaneSimpleCommand(commandTab[prop_sync_on][arduinoRunForFSX]),
                                        new XPlaneSimpleCommand(commandTab[prop_sync_off][arduinoRunForFSX]));


    ////////////////
    // ICE SWITCH //
    ////////////////
    cockpitApp->RegisterInputControl(    new ArduinoToggleSwitchControl(26),
                                        new XPlaneSimpleCommand(commandTab[pitot_heat0_on][arduinoRunForFSX], commandTab[pitot_heat1_on][arduinoRunForFSX]),
                                        new XPlaneSimpleCommand(commandTab[pitot_heat0_off][arduinoRunForFSX], commandTab[pitot_heat1_off][arduinoRunForFSX]));
    cockpitApp->RegisterInputControl(    new ArduinoToggleSwitchControl(28),
                                        new XPlaneSimpleCommand(commandTab[AOA_heat0_on][arduinoRunForFSX]),
                                        new XPlaneSimpleCommand(commandTab[AOA_heat0_off][arduinoRunForFSX]));
    cockpitApp->RegisterInputControl(    new ArduinoToggleSwitchControl(22),
                                        new XPlaneSimpleCommand(commandTab[prop_heat0_on][arduinoRunForFSX]),
                                        new XPlaneSimpleCommand(commandTab[prop_heat0_off][arduinoRunForFSX]));
    cockpitApp->RegisterInputControl(    new ArduinoToggleSwitchControl(30),
                                        new XPlaneSimpleCommand(commandTab[window_heat_on][arduinoRunForFSX]),
                                        new XPlaneSimpleCommand(commandTab[window_heat_off][arduinoRunForFSX]));
    cockpitApp->RegisterInputControl(    new ArduinoToggleSwitchControl(24),
                                        new XPlaneSimpleCommand(commandTab[wing_heat0_on][arduinoRunForFSX]),
                                        new XPlaneSimpleCommand(commandTab[wing_heat0_off][arduinoRunForFSX]));

    ///////////////////////
    // FUEL BOOST SWITCH //
    ///////////////////////
    cockpitApp->RegisterInputControl(    new ArduinoToggleSwitchControl(44),
                                        new XPlaneSimpleCommand(commandTab[fuel_pump_1_on][arduinoRunForFSX]),
                                        new XPlaneSimpleCommand(commandTab[fuel_pump_1_off][arduinoRunForFSX]));
    cockpitApp->RegisterInputControl(    new ArduinoToggleSwitchControl(46),
                                        new XPlaneSimpleCommand(commandTab[fuel_pump_2_on][arduinoRunForFSX]),
                                        new XPlaneSimpleCommand(commandTab[fuel_pump_2_off][arduinoRunForFSX]));


    ////////////////////
    // Gear Indicator //
    ////////////////////
    cockpitApp->RegisterOutputControl(   new ArduinoLEDControl(32, arduinoRunForFSX ? 100 : 1),
                                        new XPlaneInputData(67, 0));
    cockpitApp->RegisterOutputControl(   new ArduinoLEDControl(34, arduinoRunForFSX ? 100 : 1),
                                        new XPlaneInputData(67, 1));
    cockpitApp->RegisterOutputControl(   new ArduinoLEDControl(36, arduinoRunForFSX ? 100 : 1),
                                        new XPlaneInputData(67, 2));
    cockpitApp->RegisterOutputControl(   new ArduinoLEDControl(38),
                                        new XPlaneInputData(67),
                                        arduinoRunForFSX ? inTransitGearManageFunctionFSX : inTransitGearManageFunction);


    ////////////////
    //Flap switch //
    ////////////////
    if( arduinoRunForFSX ){
        cockpitApp->RegisterInputControl(    new ArduinoThreePosToggleSwitchControl(19,18),
                                            new XPlaneSimpleCommand("FLAPS_UP"),
                                            new XPlaneSimpleCommand("FLAPS_1"),
                                            new XPlaneSimpleCommand("FLAPS_DOWN"));
    }
    else{

        cockpitApp->RegisterInputControl(    new ArduinoIncrementalThreePosToggleSwitchControl(19,18),
                                            new XPlaneSimpleCommand("sim/flight_controls/flaps_down"),
                                            new XPlaneSimpleCommand("sim/flight_controls/flaps_up"));
    }


    ////////////////////
    // Flap indicator //
    ////////////////////
    cockpitApp->RegisterOutputControl(   new ArduinoLEDControl(15), //APH
                                        new XPlaneInputData(13),
                                        arduinoRunForFSX ? aphFlapManageFunctionFSX : aphFlapManageFunction);

    cockpitApp->RegisterOutputControl(   new ArduinoLEDControl(16), //IN TRANSIT
                                        new XPlaneInputData(13),
                                        arduinoRunForFSX ? inTransitFlapManageFunctionFSX : inTransitFlapManageFunction);

    if( arduinoRunForFSX ){
      cockpitApp->RegisterOutputControl(   new ArduinoLEDControl(17), //DWN
                                          new XPlaneInputData(13),
                                          downFlapManageFunctionFSX);


    }
    else{
      cockpitApp->RegisterOutputControl(   new ArduinoLEDControl(17), //DWN
                                          new XPlaneInputData(13, 4));
    }


}



void loop()
{
    cockpitApp->Loop();
}
