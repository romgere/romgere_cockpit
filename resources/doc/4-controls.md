# Libary control

The library provide 2 types of control :
- Output controls : Use to manage hardware component (eg. LED) who will be received data from X-Plane and change their state depending on the received value.
- Input controls : Use to manage hardware component (button, switch, ...) who will send data/command to X-Plane when their state changed.


## Output control

Currently, LED is the only available output control :

* [LED "ArduinoLEDControl"](./controls/ctrl-led.md)

Other planned control (no yet implemented) :

* Servo motor
* LCD Display

*These 2 types are not implemented. Any help is appreciate. If you want to contribute to the library don't hesitate to create a PR on github.*


## Input control

Here is the list of supported controls (see related page for more informations) :

* [2 positions toggle switch "ArduinoToggleSwitchControl"](./controls/ctrl-2pos-toggle.md)
* [3 positions toggle switch "ArduinoThreePosToggleSwitchControl"](./controls/ctrl-3pos-toggle.md)
* [3 positions incremental toggle switch "ArduinoIncrementalThreePosToggleSwitchControl"](./controls/ctrl-incremental-3pos-toggle.md)
* [Push button (repeated) "ArduinoPushButtonControl"](./controls/ctrl-push-button.md)
* [Rotary encoder "ArduinoRotaryEncoderControl](./controls/ctrl-rotary-encoder.md)
* [Rotary swich "ArduinoRotarySwitchControl"](./controls/ctrl-rotary-switch.md)


Other planned control (no yet implemented) :

* Potentiometer (Analog input)

*Potentiometer type is not implemented. It's depend on the DREF command implementation. Any help is appreciate. If you want to contribute to the library don't hesitate to create a PR on github.*
