# Basic example

You can find below a basic case of use for this library.

This code manage:
- A toggle switch (2 positions) to send "Gear up/down" command.
- 3 LED bind with the 3 "Gear down" indicators.
- 1 LED to manager "Gear in transit" indicator.

```cpp
#include <Arduino.h>

#include "src/RomgereCockpit/Application/CockpitMainApplication.h"
#include "src/RomgereCockpit/CommunicationInterface/EthernetInterface.h"
#include "src/RomgereCockpit/ArduinoControl/AllControlInclude.h"

CockpitMainApplication  *cockpitApp;
EthernetInterface       *ethernetInterface;

void setup()
{
  //Create & start Ethernet interface + Create app with our Ethernet interface
  byte arduinoMAC[6]  = { 0xDE, 0xAD, 0xBE, 0xEF, 0xEA, 0xED };
  ethernetInterface = new EthernetInterface( 49001, 49000, { 192, 168, 1, 97 }, arduinoMAC, { 192, 168, 1, 21 });
  cockpitApp = new CockpitMainApplication( ethernetInterface);

  //Gear toggle switch on PIN 44
  cockpitApp->RegisterInputControl( new ArduinoToggleSwitchControl(44),
                                    new XPlaneSimpleCommand("sim/flight_controls/landing_gear_down"),
                                    new XPlaneSimpleCommand("sim/flight_controls/landing_gear_up"));

  //Gear Indicator on PIN 32, 34, 36 & 38
  cockpitApp->RegisterOutputControl( new ArduinoLEDControl(32,0),
                                     new XPlaneInputData(67, 0));
  cockpitApp->RegisterOutputControl( new ArduinoLEDControl(34,0),
                                     new XPlaneInputData(67, 1));
  cockpitApp->RegisterOutputControl( new ArduinoLEDControl(36,0),
                                     new XPlaneInputData(67, 2));
  cockpitApp->RegisterOutputControl( new ArduinoLEDControl(38),
                                     new XPlaneInputData(67),
                                     inTransitGearManageFunction);
}

//Transformation function for "Gear in transit" indicator
//All gear down (1) or up (0) => turn off (return 0), turn on (return 1) otherwise
float inTransitGearManageFunction( float *val){
  return (( val[0] == 1 && val[1] == 1 && val[2] == 1 ) || ( val[0] == 0 && val[1] == 0 && val[2] == 0 )) ? 0 : 1;
}

void loop()
{
  cockpitApp->Loop();
}
```
*This sample come from [Sample0.ino](https://github.com/romgere/romgere_cockpit/blob/master/example/Sample0/Sample0.ino) file*

For more example, please see "example" folder.
