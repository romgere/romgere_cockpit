# Case of use

The `ArduinoLEDControl` class goal is to be bind with an input data (or group of data) from X-Plane to turn on or off an LED plugged on your Arduino board.

# Arduino connection

Board view | Sketch view
---------- | -----------
![LED connection (board view)](/resources/img/led_bb.png?raw=true) | ![LED connection (sketch view)](/resources/img/led_sk.png?raw=true)


# Code sample

```cpp
#include "src/RomgereCockpit/Application/CockpitMainApplication.h"
#include "src/RomgereCockpit/CommunicationInterface/EthernetInterface.h"
#include "src/RomgereCockpit/ArduinoControl/ArduinoLEDControl.h"

CockpitMainApplication  *cockpitApp;
EthernetInterface       *ethernetInterface;

void setup() {
  ethernetInterface = new EthernetInterface( ... );
  cockpitApp = new CockpitMainApplication(ethernetInterface);

  //Declare and bind control with data
  cockpitApp->RegisterOutputControl(
    new ArduinoLEDControl(8), //Create LED Control on PIN n°8, bind it to
    new XPlaneInputData(67, 0) //Use first data of group n°67 (nose gear indicator)
  );
}

void loop() {
  cockpitApp->Loop();
}
```

In this example, the LED plug on the PIN n°8 will be turn on or off depending on the state of the nose gear send by X-Plane (led on if gear down, led off otherwise).

# Options (constructor parameters)

Here is the definition of the `ArduinoLEDControl` constructor :

**Multi arduino board OFF :**
`ArduinoLEDControl( uint8_t pin, float highLEDVal = 1MM);`

**Multi arduino board ON :**
`ArduinoLEDControl( uint8_t pin, float highLEDVal = 1, int boardAddress = -1);`

Arguments definition :
1. `uint8_t pin` : PIN n° on which LED is plug.
2. `float highLEDVal` (default: 1) : Value expected (from x-Plane) to pull on the LED.
3. `int boardAddress ` (default: -1) : Address of slave board on which LED is plug (-1 = plug on main board). *Available only if you previously enable the "multi board mode"*


# Transformation function

In some case the state of the LED could depends on multiple values of one group (example for "gear in transit" LED).

In this case the `RegisterInputControl` method can take a "transformation function" as third arguments.

The "transformation function" goal is to transform all datas of single group in one float value. This single float value will be pass to the `ArduinoLEDControl` to turn on/off the LED.

For more information please see [transformation function documentation page](/resources/doc/6-transformation-function.md).
