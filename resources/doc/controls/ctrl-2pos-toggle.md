# Case of use

The `ArduinoToggleSwitchControl` class allow you to send 2 commands depending on the state of an digital input PIN. You can use it with a toggle switch or a simple push button.

When you use the `ArduinoToggleSwitchControl` class with an push button, unlike `ArduinoPushButton`, the command will be send one time.

# Arduino connection

Board view | Sketch view
---------- | -----------
![2 positions toggle switch connection (board view)](/resources/img/toggle_switch_bb.png?raw=true) | ![2 positions toggle switch connection (sketch view)](/resources/img/toggle_switch_sk.png?raw=true)


# Code sample

```cpp
#include "src/RomgereCockpit/Application/CockpitMainApplication.h"
#include "src/RomgereCockpit/CommunicationInterface/EthernetInterface.h"
#include "src/RomgereCockpit/ArduinoControl/ArduinoToggleSwitchControl.h"

CockpitMainApplication  *cockpitApp;
EthernetInterface       *ethernetInterface;

void setup() {
  ethernetInterface = new EthernetInterface( ... );
  cockpitApp = new CockpitMainApplication(ethernetInterface);

  //Declare and bind control with command
  cockpitApp->RegisterInputControl(
    new ArduinoToggleSwitchControl(6), //Create Toggle Switch Control on PIN 7
    new XPlaneSimpleCommand("sim/lights/landing_lights_on"), //Send "landing lights ON" command to X-Plane
    new XPlaneSimpleCommand("sim/lights/landing_lights_off") //Send "landing lights OFF" command to X-Plane
  );

  cockpitApp->RegisterInputControl(
    new ArduinoToggleSwitchControl(7), //Create Toggle Switch Control on PIN 7
    new XPlaneSimpleCommand("sim/fuel/fuel_pump_1_on"), //Send "fuel pump1 ON" command to X-Plane
    new XPlaneSimpleCommand("sim/fuel/fuel_pump_1_off") //Send "fuel pump1 ON" command to X-Plane
  );
}

void loop() {
  cockpitApp->Loop();
}
```

# Options (constructor parameters)

Here is the definition of the `ArduinoToggleSwitchControl` constructor :

**Multi arduino board OFF :**
`ArduinoToggleSwitchControl( uint8_t pin);`

**Multi arduino board ON :**
`ArduinoToggleSwitchControl( uint8_t pin, int boardAddress = -1);`

Arguments definition :
1. `uint8_t pin` : PIN n° on which Toggle switch is plug.
2. `int boardAddress` (default: -1) : Address of slave board on which xxxx is plug (-1 = plug on main board). *Available only if you previously enable the "multi board mode"*
