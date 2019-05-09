#include <Arduino.h>

#include "src/RomgereCockpit/Application/CockpitMainApplication.h"
#include "src/RomgereCockpit/CommunicationInterface/EthernetInterface.h"
#include "src/RomgereCockpit/ArduinoControl/AllControlInclude.h"

//Settings
#include "NetworkConfig.h"
//Transformation function for LED management
#include "TransformationFunction.h"


CockpitMainApplication  *cockpitApp;
EthernetInterface       *ethernetInterface;


void setup()
{

  byte arduinoMAC[6]  = ARDUINO_MAC_ADDRESS;

  //Create & start Ethernet interface
  ethernetInterface = new EthernetInterface(
    { 0xDE, 0xAD, 0xBE, 0xEF, 0xEA, 0xED },
    XPLANE_WRITE_PORT,
    ARDUINO_IP_ADDRESS,
    arduinoMAC,
    XPLANE_IP_ADDRESS,
    WAIT_FOR_XPLANE_AT_STARTUP
  );
  //Create app with our Eternet interface
  cockpitApp = new CockpitMainApplication( ethernetInterface);

  ///////////////////////
  // ANNUN TEST BUTTON //
  ///////////////////////
  cockpitApp->RegisterInputControl( new ArduinoPushButtonControl(40),
                                    //new XPlaneSimpleCommand("sim/annunciator/test_all_annunciators")
                                    new LibrarySpecialCommand(LibrarySpecialCommand::SendAll));

  /////////////
  // STARTER //
  /////////////

  cockpitApp->RegisterInputControl( (new ArduinoRotarySwitchControl(9, 5))->setRepeat(4), //Command for position n°4 will be repeat until position changed
                                    new XPlaneSimpleCommand("sim/magnetos/magnetos_off_1"),
                                    new XPlaneSimpleCommand("sim/magnetos/magnetos_right_1"),
                                    new XPlaneSimpleCommand("sim/magnetos/magnetos_left_1"),
                                    new XPlaneSimpleCommand("sim/magnetos/magnetos_both_1"),
                                    new XPlaneSimpleCommand("sim/starters/engage_starter_1"));

  cockpitApp->RegisterInputControl( (new ArduinoRotarySwitchControl(8, 5))->setRepeat(4), //Command for position n°4 will be repeat until position changed
                                    new XPlaneSimpleCommand("sim/magnetos/magnetos_off_2"),
                                    new XPlaneSimpleCommand("sim/magnetos/magnetos_right_2"),
                                    new XPlaneSimpleCommand("sim/magnetos/magnetos_left_2"),
                                    new XPlaneSimpleCommand("sim/magnetos/magnetos_both_2"),
                                    new XPlaneSimpleCommand("sim/starters/engage_starter_2"));


  //////////////////
  // LIGHT SWITCH //
  //////////////////
  cockpitApp->RegisterInputControl( new ArduinoToggleSwitchControl(27),
                                    new XPlaneSimpleCommand("sim/lights/landing_lights_on"),
                                    new XPlaneSimpleCommand("sim/lights/landing_lights_off"));
  cockpitApp->RegisterInputControl( new ArduinoToggleSwitchControl(25),
                                    new XPlaneSimpleCommand("sim/lights/taxi_lights_on"),
                                    new XPlaneSimpleCommand("sim/lights/taxi_lights_off"));
  cockpitApp->RegisterInputControl( new ArduinoToggleSwitchControl(31),
                                    new XPlaneSimpleCommand("sim/lights/strobe_lights_on"),
                                    new XPlaneSimpleCommand("sim/lights/strobe_lights_off"));
  cockpitApp->RegisterInputControl( new ArduinoToggleSwitchControl(37),
                                    new XPlaneSimpleCommand("sim/lights/nav_lights_on"),
                                    new XPlaneSimpleCommand("sim/lights/nav_lights_off"));
  cockpitApp->RegisterInputControl( new ArduinoToggleSwitchControl(29),
                                    new XPlaneSimpleCommand("sim/lights/beacon_lights_on"),
                                    new XPlaneSimpleCommand("sim/lights/beacon_lights_off"));

  ///////////////////
  // MASTER SWITCH //
  ///////////////////
  cockpitApp->RegisterInputControl( new ArduinoToggleSwitchControl(39),
                                    new XPlaneSimpleCommand("sim/systems/avionics_on"),
                                    new XPlaneSimpleCommand("sim/systems/avionics_off"));
  cockpitApp->RegisterInputControl( new ArduinoToggleSwitchControl(47),
                                    new XPlaneSimpleCommand("sim/electrical/battery_1_on"),
                                    new XPlaneSimpleCommand("sim/electrical/battery_1_off"));
  cockpitApp->RegisterInputControl( new ArduinoToggleSwitchControl(45),
                                    new XPlaneSimpleCommand("sim/electrical/generator_1_on"),
                                    new XPlaneSimpleCommand("sim/electrical/generator_1_off"));
  cockpitApp->RegisterInputControl( new ArduinoToggleSwitchControl(41),
                                    new XPlaneSimpleCommand("sim/electrical/generator_2_on"),
                                    new XPlaneSimpleCommand("sim/electrical/generator_2_off"));
  cockpitApp->RegisterInputControl( new ArduinoToggleSwitchControl(43),
                                    new XPlaneSimpleCommand("sim/systems/prop_sync_on"),
                                    new XPlaneSimpleCommand("sim/systems/prop_sync_off"));


  ////////////////
  // ICE SWITCH //
  ////////////////
  cockpitApp->RegisterInputControl( new ArduinoToggleSwitchControl(26),
                                    new XPlaneSimpleCommand("sim/ice/pitot_heat0_on", "sim/ice/pitot_heat1_on"),
                                    new XPlaneSimpleCommand("sim/ice/pitot_heat0_off", "sim/ice/pitot_heat1_off"));
  cockpitApp->RegisterInputControl( new ArduinoToggleSwitchControl(28),
                                    new XPlaneSimpleCommand("sim/ice/AOA_heat0_on"),
                                    new XPlaneSimpleCommand("sim/ice/AOA_heat0_off"));
  cockpitApp->RegisterInputControl( new ArduinoToggleSwitchControl(22),
                                    new XPlaneSimpleCommand("sim/ice/prop_heat0_on"),
                                    new XPlaneSimpleCommand("sim/ice/prop_heat0_off"));
  cockpitApp->RegisterInputControl( new ArduinoToggleSwitchControl(30),
                                    new XPlaneSimpleCommand("sim/ice/window_heat_on"),
                                    new XPlaneSimpleCommand("sim/ice/window_heat_off"));
  cockpitApp->RegisterInputControl( new ArduinoToggleSwitchControl(24),
                                    new XPlaneSimpleCommand("sim/ice/wing_heat0_on"),
                                    new XPlaneSimpleCommand("sim/ice/wing_heat0_off"));

  ///////////////////////
  // FUEL BOOST SWITCH //
  ///////////////////////
  cockpitApp->RegisterInputControl( new ArduinoToggleSwitchControl(44),
                                    new XPlaneSimpleCommand("sim/fuel/fuel_pump_1_on"),
                                    new XPlaneSimpleCommand("sim/fuel/fuel_pump_1_off"));
  cockpitApp->RegisterInputControl( new ArduinoToggleSwitchControl(46),
                                    new XPlaneSimpleCommand("sim/fuel/fuel_pump_2_on"),
                                    new XPlaneSimpleCommand("sim/fuel/fuel_pump_2_off"));


  ////////////////////
  // Gear Indicator //
  ////////////////////
  cockpitApp->RegisterOutputControl( new ArduinoLEDControl(32,0),
                                     new XPlaneInputData(67, 0));
  cockpitApp->RegisterOutputControl( new ArduinoLEDControl(34,0),
                                     new XPlaneInputData(67, 1));
  cockpitApp->RegisterOutputControl( new ArduinoLEDControl(36,0),
                                     new XPlaneInputData(67, 2));
  cockpitApp->RegisterOutputControl( new ArduinoLEDControl(38),
                                     new XPlaneInputData(67),
                                     inTransitGearManageFunction);


  ////////////////
  //Flap switch //
  ////////////////
  cockpitApp->RegisterInputControl( new ArduinoIncrementalThreePosToggleSwitchControl(19,18),
                                    new XPlaneSimpleCommand("sim/flight_controls/flaps_down"),
                                    new XPlaneSimpleCommand("sim/flight_controls/flaps_up"));


  ////////////////////
  // Flap indicator //
  ////////////////////
  cockpitApp->RegisterOutputControl(  new ArduinoLEDControl(15), //APH
                                      new XPlaneInputData(13),
                                      aphFlapManageFunction);

  cockpitApp->RegisterOutputControl(  new ArduinoLEDControl(16), //IN TRANSIT
                                      new XPlaneInputData(13),
                                      inTransitFlapManageFunction);


  cockpitApp->RegisterOutputControl(  new ArduinoLEDControl(17), //DWN
                                      new XPlaneInputData(13, 4));


}


void loop()
{
  cockpitApp->Loop();
}
