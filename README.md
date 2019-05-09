# Romgere Cockpit Arduino Lib

Arduino library for building "real" cockpit/panel with X-Plane and Arduino board.

# WIP
*2019/05/04 :  This library was just moved from bitbucket (private repository) to github and need some work for writing doc/example, translate, review project folder architecture.*

**Please be careful if you using this library as is.**

**Please note that the Slave/Master board mode does not seems to work properly.**

# Basic example

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
*This sample come from [Sample0.ino](/example/Sample0/Sample0.ino) file*

For more example, please see "example" folder.

# Use the library in your own sketches

- Create an `src` folder in your Sketch folder (the folder who contain your main `.ino` file)
- Copy (or create simlink) the `lib` folder of this library and name it "RomgereCockpit"
- Include the library file(s) you need with `#include "src/RomgereCockpit/_file_you_need_.h"`.

# Todos

- [x] Translate comment and other text
- [x] Change all file header
- [x] Modify folder and tree to something like lib / test / example / doc...
- [ ] Add "this->" on some methods calls
- [x] Re-indent all pre-compilation conditional blocks
- [x] Change example and delete all FSX references
- [ ] Write doc and Readme
- [ ] Test and Fix Master/Slave board mode
- [ ] Create example for master/slave
- [ ] Add some "todos" and/or open issues
- [ ] Implements DREF and DATA command support
- [ ] Implements analog control (input and ouput)
- [ ] X-plane 11 support

# Various link to add to readme/doc

- [X-PLane command list] (https://siminnovations.com/wiki/index.php?title=Xplane_commandrefs) (or see in `Resources/plugins/Commands.txt`)
- [List DREF] http://xplane.anzui.de/dataref-search/
- [List DREF] http://www.xsquawkbox.net/xpsdk/docs/DataRefs.txt
- [List DREF] http://crewchief.aero.calpoly.edu/svn/crewrep/Pheagle/branches/RudderProject/Model/udpdata.txt
