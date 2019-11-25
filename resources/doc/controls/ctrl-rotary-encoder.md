# Case of use

The `ArduinoRotaryEncoderControl` allow you to send 2 commands depending on direction of a rotary encoder plug into your Arduino board.

This class is compatible with "incremental encoder" ([see wikipedia Incremental encoder page](https://en.wikipedia.org/wiki/Incremental_encoder) for more information).

# Arduino connection

Board view | Sketch view
---------- | -----------
![Rotary encoder connection (board view)](../../img/rotary_encoder_bb.png?raw=true) | ![Rotary encoder connection (sketch view)](../../img/rotary_encoder_sk.png?raw=true)


# Code sample

```cpp
#include "src/RomgereCockpit/Application/CockpitMainApplication.h"
#include "src/RomgereCockpit/CommunicationInterface/EthernetInterface.h"
#include "src/RomgereCockpit/ArduinoControl/ArduinoRotaryEncoderControl.h"

CockpitMainApplication  *cockpitApp;
EthernetInterface       *ethernetInterface;

void setup() {
  ethernetInterface = new EthernetInterface( ... );
  cockpitApp = new CockpitMainApplication(ethernetInterface);

  //Declare and bind control with command
  cockpitApp->RegisterInputControl(
    new ArduinoRotaryEncoderControl(5,6, ArduinoRotaryEncoderControl::Type3Encoder), //Create type 3 encoder on PIN 5 & 6
    new XPlaneSimpleCommand("sim/instruments/barometer_up"), //Send "Baro selection up" command to X-Plane
    new XPlaneSimpleCommand("sim/instruments/barometer_down") //Send "Baro selection down" command to X-Plane
  );
}

void loop() {
  cockpitApp->Loop();
}
```
# Encoder type

`ArduinoRotaryEncoderControl` class allow 4 configurations of rotary encoder depending on the number of state changes between 2 detents :
![Rotary encoder type](../../img/rotary_encoder_type.png?raw=true)
*More information in [ArduinoRotaryEncoderControl.cpp](https://github.com/romgere/romgere_cockpit/blob/master/lib/ArduinoControl/ArduinoRotaryEncoderControl.cpp) file's header*


An encoder type can be one of the type below :

1. `ArduinoRotaryEncoderControl::Type1Encoder`
2. `ArduinoRotaryEncoderControl::Type2Encoder`
3. `ArduinoRotaryEncoderControl::Type3Encoder`
4. `ArduinoRotaryEncoderControl::Type4Encoder`


# Options (constructor parameters)

Here is the definition of the `ArduinoRotaryEncoderControl` constructor :

**Multi arduino board OFF :**
`ArduinoRotaryEncoderControl( uint8_t pin1, uint8_t pin2, RotaryEncoderType type, bool usePullUpPin = false);`

**Multi arduino board ON :**
`ArduinoRotaryEncoderControl( uint8_t pin1, uint8_t pin2, RotaryEncoderType type, bool usePullUpPin = false, int boardAddress = -1);`

Arguments definition :
1. `uint8_t pin1` : PIN n°1 on which Toggle switch is plug.
2. `uint8_t pin2` : PIN n°2 on which Toggle switch is plug.
3. `RotaryEncoderType type` : Type of encoder (1 to 4 see above "Encoder type").
4. `bool usePullUpPin` (default: false) : If set to true, PIN will be init with `INPUT_PULLUP` mode or with `INPUT` otherwise.
5. `int boardAddress` (default: -1) : Address of slave board on which xxxx is plug (-1 = plug on main board). *Available only if you previously enable the "multi board mode"*
