# Case of use

The `ArduinoPushButtonControl` class allow you to send **and repeat** an X-Plane command when a push button plugged into Arduino board is pressed.

# Arduino connection

Board view | Sketch view
---------- | -----------
![Push button connection (board view)](/resources/img/push_button_bb.png?raw=true) | ![Push button connection (sketch view)](/resources/img/push_button_sk.png?raw=true)


# Code sample

```cpp
#include "src/RomgereCockpit/Application/CockpitMainApplication.h"
#include "src/RomgereCockpit/CommunicationInterface/EthernetInterface.h"
#include "src/RomgereCockpit/ArduinoControl/ArduinoPushButtonControl.h"

CockpitMainApplication  *cockpitApp;
EthernetInterface       *ethernetInterface;

void setup() {
  ethernetInterface = new EthernetInterface( ... );
  cockpitApp = new CockpitMainApplication(ethernetInterface);

  //Declare and bind control with command
  cockpitApp->RegisterInputControl(    
    new ArduinoPushButtonControl(8), //Create push button on PIN 8
    new XPlaneSimpleCommand("sim/annunciator/test_all_annunciators") //Send "Test all annunciators" command to X-Plane
  );
}

void loop() {
  cockpitApp->Loop();
}
```

# Options (constructor parameters)

Here is the definition of the `ArduinoPushButtonControl` constructor :

**Multi arduino board OFF :**
`ArduinoPushButtonControl( uint8_t pin, uint8_t defaultState = LOW);`

**Multi arduino board ON :**
`ArduinoPushButtonControl( uint8_t pin, uint8_t defaultState = LOW , int boardAddress = -1 );`

Arguments definition :
1. `uint8_t pin` : PIN n° on which Toggle switch is plug.
2. `uint8_t defaultState` (default: LOW) : Default state of the PIN (Idle state). Command will be send when PIN state is different of this value.
3. `int boardAddress` (default: -1) : Address of slave board on which xxxx is plug (-1 = plug on main board). *Available only if you previously enable the "multi board mode"*
