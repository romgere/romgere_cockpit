# Transformation function

The "transformation function" goal is to bind output control to group's data with custom logic (not only on particular data of a group).

For example, if you want to create a "gear in transit" indicator, you need to use the whole data of "gear status" group to determine the state of indicator. With a "transformation function" you will be able to calculate a single float value, depending on all datas of the group. The single float value will be pass to control to define his state.

## Function definition

Here is the definition of a transformation function :

`typedef float (*inputDataTransformFunction)(float *);`

The function take a table of float as parameter (8 float datas representing an X-Plane group) and must return a single float value depending on input data.

Here is some example of transformation function :

```cpp
float inTransitFlapManageFunction( float *val){
  return ( val[3] == val[4] ) ? 0 : 1; //Current value = target value => not in transit
}
float aphFlapManageFunction( float *val){
  return ( val[4] == 0.5 ) ? 1 : 0; //Flap half down => APH position (Baron 58)
}
float inTransitGearManageFunction( float *val){
  //All gear down => not in transit
  if( val[0] == 1 && val[1] == 1 && val[2] == 1 ){
    return 0;
  }
  //All gear up => not in transit
  else if( val[0] == 0 && val[1] == 0 && val[2] == 0 ){
    return 0;
  }
  //Otherwise => in transit
  return 1;
}
```

# Use transformation function with data and control

To use an transformation function with a data/control set, you can pass the transformation function as third argument of `RegisterOutputControl` method, please read [Register controls with data or command](./5-register-control-data-command.md) page for more informations.

# Complete code sample

```cpp
#include <Arduino.h>

#include "src/RomgereCockpit/Application/CockpitMainApplication.h"
#include "src/RomgereCockpit/CommunicationInterface/EthernetInterface.h"
#include "src/RomgereCockpit/ArduinoControl/ArduinoLEDControl.h"

CockpitMainApplication  *cockpitApp;
EthernetInterface       *ethernetInterface;

void setup()
{
  //Create & start Ethernet interface + Create app with our Ethernet interface
  byte arduinoMAC[6]  = { 0xDE, 0xAD, 0xBE, 0xEF, 0xEA, 0xED };
  ethernetInterface = new EthernetInterface( 49001, 49000, { 192, 168, 1, 97 }, arduinoMAC, { 192, 168, 1, 21 });
  cockpitApp = new CockpitMainApplication( ethernetInterface);

  //Gear in transit indicator
  cockpitApp->RegisterOutputControl(
    new ArduinoLEDControl(38), //LED on PIN n°38
    new XPlaneInputData(67), //Use the entire group datas (no index is specified)
    inTransitGearManageFunction //Transformation function
  );
}

//Transformation function for "Gear in transit" indicator
//All gear down (1) or up (0) => turn off (return 0), turn on (return 1) otherwise
float inTransitGearManageFunction( float *val){
  return (( val[0] == 1 && val[1] == 1 && val[2] == 1 ) || ( val[0] == 0 && val[1] == 0 && val[2] == 0 )) ? 0 : 1;
}

void loop()
{
  cockpitApp->Loop();
}
```
