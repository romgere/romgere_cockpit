# Romgere Cockpit Arduino Lib

Arduino library for building "real" cockpit/panel with X-Plane and Arduino board.

I use it to build my own "cockpit", an Beechcraft Baron 58 "cockpit" (Still work in progress) :
![My Beechcraft Baron 58 cockpit (WIP)](./resources/img/my_cockpit.jpg?raw=true)
*Instruments display not done with this library*

# Documentation

[![Netlify Status](https://api.netlify.com/api/v1/badges/26429911-d246-4dd3-8417-8da933fb8e2d/deploy-status)](https://app.netlify.com/sites/festive-fermi-955d25/deploys)


The library documentation can be found on [romgere-cockpit-arduino-lib.mestres.fr](http://romgere-cockpit-arduino-lib.mestres.fr/)

# Sample

```cpp
#include <Arduino.h>

#include "src/RomgereCockpit/Application/CockpitMainApplication.h"
#include "src/RomgereCockpit/CommunicationInterface/EthernetInterface.h"
#include "src/RomgereCockpit/ArduinoControl/AllControlInclude.h"

CockpitMainApplication  *cockpitApp;
EthernetInterface       *ethernetInterface;

void setup()
{
  //Create & start Ethernet interface + Create app with our Eternet interface
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
*This sample come from [Sample0.ino](/example/Sample0/Sample0.ino) file. Find more example in ["example" folder](/example/)*

For more example, please see "example" folder.


# Todos

- [x] Translate comment and other text
- [x] Change all file header
- [x] Modify folder and tree to something like lib / test / example / doc...
- [ ] Add "this->" on some methods calls
- [x] Re-indent all pre-compilation conditional blocks
- [x] Change example and delete all FSX references
- [x] Write doc and Readme
- [ ] Test and Fix Master/Slave board mode
- [x] Create example for master/slave
- [ ] Add some "todos" and/or open issues
- [x] Implements DREF
- [ ] Implements DATA command support
- [x] Implements input analog control
- [x] Implements output analog control
- [ ] X-plane 11 support

# License

This library is under the MIT license.  
For the whole copyright, please see the [LICENSE](LICENSE) file.
