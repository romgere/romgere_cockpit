
/**
 * Romgere Cockpit Library - https://github.com/romgere/romgere_cockpit
 * @author jerome@mestres.fr
 */


#include "SerialDebugInterface.h"


SerialDebugInterface::SerialDebugInterface( int serialSpeed){
    FakeData = new XPGroupDatas();

    FakeData->data[0] = 0;
    FakeData->data[1] = 0;
    FakeData->data[2] = 0;
    FakeData->data[3] = 0;

    Serial.begin( serialSpeed);

}
SerialDebugInterface::~SerialDebugInterface(){
    delete FakeData;
}


void SerialDebugInterface::SendCommand(const char* cmd){
    Serial.print("DebugInterface, SEND COMMAND : ");
    Serial.println(cmd);
}


void SerialDebugInterface::SendKey( const char* key){
    Serial.print("DebugInterface, SEND KEY : ");
    Serial.println(key);
}

void SerialDebugInterface::SendDrefCommand( const  char *dref, float value){
    Serial.print("DebugInterface, SEND DREF : ");
    Serial.print("sim/");
    Serial.print(dref);
    Serial.print(" with data : {");
    Serial.print(value);
    Serial.println("}.");
}

//Get an empty group datas (4 x 0 float value)
XPGroupDatas* SerialDebugInterface::GetData( float group ){
    FakeData->group = group;
    return FakeData;
}
