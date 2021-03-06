#include <Arduino.h>
//#include <SPI.h>
//#include <Ethernet.h>
//#include <Wire.h>

#include "src/RomgereCockpit/Application/CockpitMainApplication.h"
#include "src/RomgereCockpit/CommunicationInterface/SerialDebugInterface.h"
#include "src/RomgereCockpit/CommunicationInterface/EthernetInterface.h"
#include "src/RomgereCockpit/ArduinoControl/AllControlInclude.h"

#include "NetworkConfig.h"

CockpitMainApplication  *cockpitApp;
EthernetInterface       *ethernetInterface;

void setup()
{
    /*
    cockpitApp = new CockpitMainApplication( new SerialDebugInterface());
    /*/
    byte arduinoMAC[6]  = ARDUINO_MAC_ADDRESS;

    //Create & start Ethernet interface
    ethernetInterface = new EthernetInterface(
        XPLANE_READ_PORT,
        XPLANE_WRITE_PORT,
        ARDUINO_IP_ADDRESS,
        arduinoMAC,
        XPLANE_IP_ADDRESS,
        WAIT_FOR_XPLANE_AT_STARTUP
    );
    //Create app with our Eternet interface
    cockpitApp = new CockpitMainApplication( ethernetInterface);
    //*/

    cockpitApp->RegisterInputControl(
        new ArduinoPotentiometerControl(0, 0.05, 0, 1, 1),
        new XPlaneDREFCommand("cockpit2/controls/parking_brake_ratio[0]"  )
    );

    cockpitApp->RegisterInputControl(
        new ArduinoToggleSwitchControl(5, 1),
        new XPlaneSimpleCommand("sim/lights/landing_lights_on"),
        new XPlaneSimpleCommand("sim/lights/landing_lights_off")
    );


    cockpitApp->RegisterOutputControl(
        new ArduinoLEDControl(7, 1, 1), //DWN
        new XPlaneInputData(13, 4)
    );
}

void loop()
{
    cockpitApp->Loop();
}
