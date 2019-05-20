# Create App and NetworkInterface instance in your app

The first step to use this library (after [importing library](/resources/doc/index.md#use-the-library-in-your-own-sketches)) is to declare and create the "communication interface" (to send/recevied data to/from X-Plane) and the  library itself

## Communication interface

For now, the only functional communication interface is `EthernetInterface`. This class allow the library to send and received datas to and from X-Plane.

The `EthernetInterface` take various arguments like read/write port, Arduino/Xplane address, etc...

You can declare (globally) and create an instance (in `setup()`), like that :

```cpp
#include "src/RomgereCockpit/CommunicationInterface/EthernetInterface.h"

EthernetInterface       *ethernetInterface;

void setup()
{
  byte arduinoMAC[6]  = { 0xDE, 0xAD, 0xBE, 0xEF, 0xEA, 0xED };
  ethernetInterface = new EthernetInterface(
    49001, //Read port (UDP)
    49000, //Write port (UDP)
    { 192, 168, 1, 97 }, //Arduino IP address
    arduinoMAC, //Arduino MAC addresse
    { 192, 168, 1, 21 } //X-Plane IP address
  );
}
```

**Constructor arguments :**
1. `unsigned int readPort` : UDP port used to received data to X-Plane
2. `unsigned int writePort` : UDP port used to send data to X-Plane
3. `IPAddress arduinoIP` : Arduino IP address. If `{0,0,0,0}`, DHCP will be used.
4. `uint8_t arduinoMAC[6]` : Arduino MAC Address.
5. `IPAddress xplaneIP` : X-Plane IP address. If `{0,0,0,0}`, address will be set on first receive datas.
6. `bool waitForXPlane` (default: false, **optional**) : Wait until X-Plane send first data sample on init.


## Cockpit Main application

### Create instance
The library itself use a main class named `CockpitMainApplication`. This class is used to declare (and bind) the X-Plane's datas/commands and the input/output controls to manage.

It's the main class of this library and the previously created "Communication interface" class has to be pass as argument when your create your `CockpitMainApplication` instance.



You can declare (globally) and create an instance (in `setup()`), like that :

```cpp
#include "src/RomgereCockpit/Application/CockpitMainApplication.h"

CockpitMainApplication  *cockpitApp;

void setup()
{
  cockpitApp = new CockpitMainApplication(
    ethernetInterface //BaseCommunicationInterface instance
  );
}
```

**Constructor arguments :**
1. `BaseCommunicationInterface *comInterface` : An instance of "Communication interface". You can use `EthernetInterface` (see above) or `SerialDebugInterface`.

### Call the `loop()` method

To works properly, the libray need to be notice on each Arduino loop. To notice the library when loop is done, just call the `loop()` method on your `CockpitMainApplication` instance :

```cpp
void loop()
{
  cockpitApp->Loop();
}
```

## Full Sample

```cpp
#include <Arduino.h>

#include "src/RomgereCockpit/Application/CockpitMainApplication.h"
#include "src/RomgereCockpit/CommunicationInterface/EthernetInterface.h"

CockpitMainApplication  *cockpitApp;
EthernetInterface       *ethernetInterface;

void setup()
{
  //Create & start Ethernet interface + Create app with our Ethernet interface
  byte arduinoMAC[6]  = { 0xDE, 0xAD, 0xBE, 0xEF, 0xEA, 0xED };
  ethernetInterface = new EthernetInterface(
    49001,
    49000,
    { 192, 168, 1, 97 },
    arduinoMAC,
    { 192, 168, 1, 21 }
  );
  cockpitApp = new CockpitMainApplication( ethernetInterface);

  //You can now register controls/commands
}

void loop()
{
  cockpitApp->Loop();
}
```
