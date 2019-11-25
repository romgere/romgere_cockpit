# Case of use

The `ArduinoThreePosToggleSwitchControl` class allow you to send 3 commands depending on the state of 2 digital input PIN. You can use it with a 3 positions toggle switch.

# Arduino connection

Board view | Sketch view
---------- | -----------
![3 positions toggle switch connection (board view)](../../img/3pos_toggle_switch_bb.png?raw=true) | ![3 positions toggle switch connection (sketch view)](../../img/3pos_toggle_switch_sk.png?raw=true)


# Code sample

```cpp
#include "src/RomgereCockpit/Application/CockpitMainApplication.h"
#include "src/RomgereCockpit/CommunicationInterface/EthernetInterface.h"
#include "src/RomgereCockpit/ArduinoControl/ArduinoTheePosToggleSwitchControl.h"

CockpitMainApplication  *cockpitApp;
EthernetInterface       *ethernetInterface;

void setup() {
  ethernetInterface = new EthernetInterface( ... );
  cockpitApp = new CockpitMainApplication(ethernetInterface);

  //Declare and bind control with command
  cockpitApp->RegisterInputControl(
    new ArduinoTheePosToggleSwitchControl(6, 7), //Create 3 pos Toggle Switch Control on PIN 6 & 7
    new XPlaneSimpleCommand("sim/autopilot/hsi_select_nav_1"), //Send "HSI shows nav 1" command to X-Plane
    new XPlaneSimpleCommand("sim/autopilot/hsi_select_nav_2"), //Send "HSI shows nav 2" command to X-Plane
    new XPlaneSimpleCommand("sim/autopilot/hsi_select_gps") //Send "HSI shows GPS" command to X-Plane
  );
}

void loop() {
  cockpitApp->Loop();
}
```

# Options (constructor parameters)

Here is the definition of the `ArduinoThreePosToggleSwitchControl` constructor :

**Multi arduino board OFF :**
`ArduinoIncrementalThreePosToggleSwitchControl( uint8_t pin1, uint8_t pin2);`

**Multi arduino board ON :**
`ArduinoIncrementalThreePosToggleSwitchControl( uint8_t pin1, uint8_t pin2, int boardAddress = -1);`

Arguments definition :
1. `uint8_t pin1` : PIN n°1 on which Toggle switch is plug.
2. `uint8_t pin2` : PIN n°2 on which Toggle switch is plug.
3. `int boardAddress` (default: -1) : Address of slave board on which xxxx is plug (-1 = plug on main board). *Available only if you previously enable the "multi board mode"*
