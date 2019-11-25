# Case of use

The `ArduinoIncrementalThreePosToggleSwitchControl` class allow you to send *only* 2 different command with a 3 position toggle switch.

It's pretty similar to [ArduinoThreePosToggleSwitchControl](ctrl-3pos-toggle.md) but the "central position" command is defined depending of the last state (same command repeat).

It's useful, for example with the flap switch of the Beechcraft baron 58, who has 3 positions, when X-Plane give us the ability to only send a "flap up/down" command.

# Arduino connection

Board view | Sketch view
---------- | -----------
![3 positions toggle switch connection (board view)](../../img/3pos_toggle_switch_bb.png?raw=true) | ![3 positions toggle switch connection (sketch view)](../../img/3pos_toggle_switch_sk.png?raw=true)


# Code sample

```cpp
#include "src/RomgereCockpit/Application/CockpitMainApplication.h"
#include "src/RomgereCockpit/CommunicationInterface/EthernetInterface.h"
#include "src/RomgereCockpit/ArduinoControl/ArduinoIncrementalThreePosToggleSwitchControl.h"

CockpitMainApplication  *cockpitApp;
EthernetInterface       *ethernetInterface;

void setup() {
  ethernetInterface = new EthernetInterface( ... );
  cockpitApp = new CockpitMainApplication(ethernetInterface);

  //Declare and bind control with command
  cockpitApp->RegisterInputControl(    
    new ArduinoIncrementalThreePosToggleSwitchControl(6, 7), //Create 3 pos Toggle Switch Control on PIN 6 & 7
    new XPlaneSimpleCommand("sim/flight_controls/flaps_down"), //Send "Flap down" command to X-Plane
    new XPlaneSimpleCommand("sim/flight_controls/flaps_up") //Send "Flap up" command to X-Plane
  );
}

void loop() {
  cockpitApp->Loop();
}
```

# Options (constructor parameters)

Here is the definition of the `ArduinoIncrementalThreePosToggleSwitchControl` constructor :

**Multi arduino board OFF :**
`ArduinoIncrementalThreePosToggleSwitchControl( uint8_t pin1, uint8_t pin2);`

**Multi arduino board ON :**
`ArduinoIncrementalThreePosToggleSwitchControl( uint8_t pin1, uint8_t pin2, int boardAddress = -1);`

Arguments definition :
1. `uint8_t pin1` : PIN n°1 on which Toggle switch is plug.
2. `uint8_t pin2` : PIN n°2 on which Toggle switch is plug.
3. `int boardAddress` (default: -1) : Address of slave board on which xxxx is plug (-1 = plug on main board). *Available only if you previously enable the "multi board mode"*
