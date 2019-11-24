# Register (aka "bind") input/output control with data/control

To work properly, all controls need to be associate ("register") with data or command :
- An [output control](./4-controls.md#ouput_control) (LED for example) need to be register with an [X-Plane input data](./3-command-and-data.md#input_data).
- An [input control](./4-controls.md#input_control) (Push button for example) need to be register with an [X-Plane command](./3-command-and-data.md#output_command).


Each input control registered with command(s) will send the associated command when its state change.

Each registered output control with data will change when associated data change.

## Register input control with X-Plane command

To register input control and X-Plane command together you need to use the `RegisterInputControl` method on the application instance :

```cpp
cockpitApp->RegisterInputControl(
  new ArduinoPushButtonControl(40),
  new XPlaneSimpleCommand("sim/annunciator/test_all_annunciators")
);
```

`void RegisterInputControl( ArduinoInputControl *ctrl, XPlaneOutputCommand *cmd1 = NULL, ...);`

Arguments definition :
1. `ArduinoInputControl *ctrl` : Pointer to the input control to register.
2. `XPlaneOutputCommand *cmd1` (default: NULL) : Pointer to output command.
3. `...` : Other output command (for multiple positions control). *Max number of argumen depending on the library configuration*


## Register output control with X-Plane data

To register output control and X-Plane data together you need to use the `RegisterOutputControl` method on the application instance :

```cpp
cockpitApp->RegisterOutputControl(
  new ArduinoLEDControl(32),
  new XPlaneInputData(67, 0)
);
```

`void RegisterOutputControl( ArduinoOutputControl *ctrl,  XPlaneInputData *data, inputDataTransformFunction fct = NULL);`

Arguments definition :
1. `ArduinoOutputControl *ctrl` : Pointer to the output control to register.
2. `XPlaneInputData *data` : Pointer to input data.
3. `inputDataTransformFunction fct` (default: NULL) : Transformation function (Use whole group's data to determine control state). See [Transformation function](./6-transformation-function.md) for more information.
