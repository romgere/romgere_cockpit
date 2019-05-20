# Romgere Cockpit Arduino Lib

Arduino library for building "real" cockpit/panel with X-Plane and Arduino board.

I use it to build my own "cockpit", an Beechcraft Baron 58 "cockpit" (Still work in progress) :
![My Beechcraft Baron 58 cockpit (WIP)](/resources/img/my_cockpit.jpg?raw=true)
*Instruments display not done with this library*

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

The documentation file are stored in the [`resources/doc`](resources/doc/index.md) directory :

* [Getting started](/resources/doc/index.md#getting-started)
  * [Prerequisites](/resources/doc/index.md#prerequisites)
  * [How it works](/resources/doc/index.md#how-it-works)
  * [Use library on your own sketches](/resources/doc/index.md#use-the-library-in-your-own-sketches)
  * [X-Plane configuration](/resources/doc/index.md#x-plane-configuration)
* [Further documentation](/resources/doc/index.md#further-documentation)
  * [Configuration reference](/resources/doc/1-configuration-reference.md)
  * [Basic example](/resources/doc/2-basic-example.md)
  * [Xplane data/command](/resources/doc/3-command-and-data.md)
  * [Control (hardware)](/resources/doc/4-controls.md)
  * [Register controls with data or command](/resources/doc/5-register-control-data-command.md)
  * [Transformation function](/resources/doc/6-transformation-function.md)
  * [Use multiple Arduino boards](/resources/doc/7-multiple-boards.md)

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
- [ ] Implements DREF and DATA command support
- [ ] Implements analog control (input and ouput)
- [ ] X-plane 11 support

# License

This library is under the MIT license.  
For the whole copyright, please see the [LICENSE](LICENSE) file.
