**WIP**

# Case of use

The `ArduinoRotarySwitchControl` allow you to use multiple position rotary switch (max position number depends on the value of [MAX_COMMAND_FOR_ONE_CONTROLE](/resources/doc/1-configuration-reference.md#MAX_COMMAND_FOR_ONE_CONTROLE) parameter). Each position send can send a different command and one or more command can be repeat as long as the position remains.

# Arduino connection

Board view | Sketch view
---------- | -----------
![Rotary switch connection (board view)](/resources/img/rotary_switch_bb.png?raw=true) | ![Rotary switch connection (sketch view)](/resources/img/rotary_switch_sk.png?raw=true)


# Code sample

```cpp
#include "src/RomgereCockpit/Application/CockpitMainApplication.h"
#include "src/RomgereCockpit/CommunicationInterface/EthernetInterface.h"
#include "src/RomgereCockpit/ArduinoControl/ArduinoRotarySwitchControl.h"

CockpitMainApplication  *cockpitApp;
EthernetInterface       *ethernetInterface;

void setup() {
  ethernetInterface = new EthernetInterface( ... );
  cockpitApp = new CockpitMainApplication(ethernetInterface);

  ArduinoRotarySwitchControl a*;

  //Declare and bind control with command
  cockpitApp->RegisterInputControl(
    (a = new ArduinoRotarySwitchControl(0, 5)), //Create a Rotary switch with 5 position on PIN n°0
    new XPlaneSimpleCommand("sim/magnetos/magnetos_off_1"), //Send "Magnetos off for engine #1" to X-Plane
    new XPlaneSimpleCommand("sim/magnetos/magnetos_right_1"), //Send "Magnetos right for engine #1" to X-Plane
    new XPlaneSimpleCommand("sim/magnetos/magnetos_left_1"), //Send "Magnetos left for engine #1" to X-Plane
    new XPlaneSimpleCommand("sim/magnetos/magnetos_both_1"), //Send "Magnetos both for engine #1" to X-Plane
    new XPlaneSimpleCommand("sim/starters/engage_starter_1") //Send "Engage starter #1" to X-Plane
  );

  a->setRepeat(4); //Set last command repeat mode on ("Engage starter #1" command)
}

void loop() {
  cockpitApp->Loop();
}
```

# Options (constructor parameters)

Here is the definition of the `ArduinoRotarySwitchControl` constructor :

**Multi arduino board OFF :**
`ArduinoRotarySwitchControl( uint8_t pin, uint8_t positionCount);`

**Multi arduino board ON :**
`ArduinoRotarySwitchControl( uint8_t pin, uint8_t positionCount, int boardAddress = -1);`

Arguments definition :
1. `uint8_t pin` : PIN n° on which Rotary switch is plug.
2. `uint8_t positionCount` : Number of available position on Rotary switch (Maximum value depends on the value of [MAX_COMMAND_FOR_ONE_CONTROLE](/resources/doc/1-configuration-reference.md#MAX_COMMAND_FOR_ONE_CONTROLE) parameter).
3. `int boardAddress` (default: -1) : Address of slave board on which xxxx is plug (-1 = plug on main board). *Available only if you previously enable the "multi board mode"*

# Repeat mode

Each position of an `ArduinoRotarySwitchControl` can be set in "repeat mode". That mean the command bind to the position is send as long as the position remains.

To set "repeat mode" on a position you can use the `setRepeat()` method :

`setRepeat( uint8_t pos, bool repeat = true);`

Arguments definition :
1. `uint8_t pos` : Position for which the "repeat mode" is set (First position : 0).
2. `bool repeat` (default : true) : True : active "repeat mode", false to disabled it.

As the `setRepeat()` return a reference to itself you can chain multiple calls to this method :
`a->setRepeat(4)->setRepeat(3)->setRepeat(0);`
