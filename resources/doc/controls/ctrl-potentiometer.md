# Case of use

The `ArduinoPotentiometerControl` allow you to use a potentiometer (or any other component wich generate analog values).
This control is designed to be used for sending DREF instruction to XPlane.

For example it can be used to manage parking break, pitch, roll etc..

The value send to Xplane will depend on the output voltage of the potentiometer.

By default, a `ArduinoPotentiometerControl` will send to Xplane a `float` value between 0 & 1 by step of 0.05, but you can change these settings to fit your need.


# Arduino connection

Board view | Sketch view
---------- | -----------
![Rotary switch connection (board view)](../../img/potentiometer_bb.png?raw=true) | ![Rotary switch connection (sketch view)](../../img/potentiometer_sk.png?raw=true)


# Code sample

```cpp
#include "src/RomgereCockpit/Application/CockpitMainApplication.h"
#include "src/RomgereCockpit/CommunicationInterface/EthernetInterface.h"
#include "src/RomgereCockpit/ArduinoControl/ArduinoPotentiometerControl.h"

CockpitMainApplication  *cockpitApp;
EthernetInterface       *ethernetInterface;

void setup() {
  ethernetInterface = new EthernetInterface( ... );
  cockpitApp = new CockpitMainApplication(ethernetInterface);

  //Declare a potentiometer on analog pin n°5 and bind it to a DREF to manage parking brake ratio
  cockpitApp->RegisterInputControl(    
      new ArduinoPotentiometerControl(5),
      new XPlaneDREFCommand("cockpit2/controls/parking_brake_ratio[0]")
  );
}

void loop() {
  cockpitApp->Loop();
}
```

# Options (constructor parameters)

Here is the definition of the `ArduinoPotentiometerControl` constructor :

**Multi arduino board OFF :**
`ArduinoPotentiometerControl( uint8_t pin, float step = 0.05, float min = 0, float max = 1);`

**Multi arduino board ON :**
`ArduinoPotentiometerControl( uint8_t pin, float step = 0.05, float min = 0, float max = 1, int boardAddress = -1 );`

Arguments definition :
1. `uint8_t pin` : PIN n° on which Rotary switch is plug.
2. `float step` (default: 0.05) : Define the "length" of one step. Used to prevent the control to send command to XPlane on "minor" input voltage changes (see [`step` parameter overview](#step-parameter-overview)).
3. `float min` (default: 0) : The minimal value the control will send as DREF/Command value.
4. `float max` (default: 1) : The minimal value the control will send as DREF/Command value.
5. `int boardAddress` (default: -1) : Address of slave board on which xxxx is plug (-1 = plug on main board). *Available only if you previously enable the "multi board mode"*

# `step` parameter overview

Due to analog signal input, a `ArduinoPotentiometerControl` implements a `step`, a minimal value change allowed without considering the control as "changed".

For example, if you declare a `ArduinoPotentiometerControl` with min/max to 0/100 & step to 10 :
- Set the input voltage to 0v : Controll will send DREF with `0`
- Set the input voltage to 5v : Controll will send DREF with `100`
- Set the input voltage to 4.9v : Controll will send NOTHING :
  - The  "internal" value corresponding to 4.9v is `98`
  - But the change is bellow than `step` value (aka 90)
  - Library considers that the control did not change

Here is a table for the example above :

Min voltage (>=) | Max voltage (<) | Output value (DREF value)
---------- | ----------- | -----------
0v (0%) | ~0.5v (10%) | 0
~0.5v (10%) | ~0.95v (19%) | 10
~0.95v (19%) | ~1.4v (28%) | 20
~1.4v (28%) | ~1.85v (37%) | 30
~1.85v (37%) | ~2.3v (46%) | 40
~2.3v (46%) | ~2.75v (55%) | 50
~2.75v (55%) | ~3.2v (64%) | 60
~3.2v (64%) | ~3.65v (73%) | 70
~3.65v (73%) | ~4.1v (82%) | 80
~4.1v (82%) | ~4.6v (92%) | 90
~4.6v (92%) | 5v (100%) | 100
