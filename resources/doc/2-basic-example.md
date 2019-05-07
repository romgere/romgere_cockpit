
1 basic example code + explain of each line/block

# Basic example

```
//Register an "3 positions Toggle Switch" on PIN 40, 41 to select auto pilot source (Nav1, nav2 or GPS)
cockpitApp->RegisterInputControl(   new ArduinoThreePosToggleSwitchControl(40,41),
                                    new XPlaneSimpleCommand("sim/autopilot/hsi_select_nav_1"),
                                    new XPlaneSimpleCommand("sim/autopilot/hsi_select_nav_2"),
                                    new XPlaneSimpleCommand("sim/autopilot/hsi_select_gps")
                                );

//Register an "Rotary encoder" on PIN 30, 31, to send "Barometer UP/DOWN" command to x-plane
cockpitApp->RegisterInputControl(   new ArduinoRotaryEncoderControl(30,31, ArduinoRotaryEncoderControl::Type3Encoder),
                                    new XPlaneSimpleCommand("sim/instruments/barometer_down"),
                                    new XPlaneSimpleCommand("sim/instruments/barometer_up"));
```

For more example, please see `Carte1` folder.
