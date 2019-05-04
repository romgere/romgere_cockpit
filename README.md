# Romgere Cockpit Arduino Lib

Arduino library for building "real" cockpit/panel with X-Plane and arduino board.

# WIP
*2019/05/04 :  This library was just moved from bitbucket (private repository) to github and need some work for writing doc/example, translate, review project organisation.*

**Please be careful if you using this library as is.**

# Basic example

```
//Declare an "3 positions Toggle Switch" on PIN 40, 41 to select auto pilot source (Nav1, nav2 or GPS)
cockpitApp->DeclareInputControl(     new ArduinoThreePosToggleSwitchControl(40,41),
                                    new XPlaneSimpleCommand("sim/autopilot/hsi_select_nav_1"),
                                    new XPlaneSimpleCommand("sim/autopilot/hsi_select_nav_2"),
                                    new XPlaneSimpleCommand("sim/autopilot/hsi_select_gps")
                                );

//Declare an "Rotary encoder" on PIN 30, 31, to send "Barometer UP/DOWN" command to x-plane
cockpitApp->DeclareInputControl(     new ArduinoRotaryEncoderControl(30,31, ArduinoRotaryEncoderControl::Type3Encoder),
                                    new XPlaneSimpleCommand("sim/instruments/barometer_down"),
                                    new XPlaneSimpleCommand("sim/instruments/barometer_up"));
```

For more example, please see `Carte1` folder.

# Todos

- [ ] Translate comment and other text
- [ ] Change all file header
- [x] Modify folder and tree to something like lib / test / example / doc...
- [ ] Change example and delete all FSX references
- [ ] Write doc and Readme
- [ ] Try to create some complete example
- [ ] Add some "todos" and/or open issues
- [ ] X-plane 11 support

# Various link to add to readme/doc

infos divers à mettre dans la doc

- [Arduino dans VS] (http://www.visualmicro.com/)
- [Liste des commande] (https://siminnovations.com/wiki/index.php?title=Xplane_commandrefs) (or see in `Resources/plugins/Commands.txt`)
- [Liste DREF] http://xplane.anzui.de/dataref-search/
- [Liste DREF] http://www.xsquawkbox.net/xpsdk/docs/DataRefs.txt
- [Liste DREF] http://crewchief.aero.calpoly.edu/svn/crewrep/Pheagle/branches/RudderProject/Model/udpdata.txt
