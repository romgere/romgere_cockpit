# Romgere Cockpit Arduino Lib

Arduino library for building "real" cockpit/panel with X-Plane and Arduino board.

# WIP
*2019/05/04 :  This library was just moved from bitbucket (private repository) to github and need some work for writing doc/example, translate, review project folder architecture.*

**Please be careful if you using this library as is.**

**Please note that the Slave/Master board mode does not seems to work properly.**

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

# Use the library in your own sketches

- Create an `src` folder in your Sketch folder (the folder who contain your main `.ino` file)
- Copy (or create simlink) the `lib` folder of this library and name it "RomgereCockpit"
- Include the library file(s) you need with `#include "src/RomgereCockpit/_file_you_need_.h"`.

# Todos

- [x] Translate comment and other text
- [x] Change all file header
- [x] Modify folder and tree to something like lib / test / example / doc...
- [ ] Add "this->" on some methods calls
- [x] Re-indent all pre-compilation conditional blocks
- [ ] Test and Fix Master/Slave board mode
- [ ] Change example and delete all FSX references
- [ ] Write doc and Readme
- [ ] Try to create some complete example
- [ ] Add some "todos" and/or open issues
- [ ] Implements DREF and DATA command support
- [ ] Implements analog control (input and ouput)
- [ ] X-plane 11 support

# Various link to add to readme/doc

- [X-PLane command list] (https://siminnovations.com/wiki/index.php?title=Xplane_commandrefs) (or see in `Resources/plugins/Commands.txt`)
- [List DREF] http://xplane.anzui.de/dataref-search/
- [List DREF] http://www.xsquawkbox.net/xpsdk/docs/DataRefs.txt
- [List DREF] http://crewchief.aero.calpoly.edu/svn/crewrep/Pheagle/branches/RudderProject/Model/udpdata.txt
