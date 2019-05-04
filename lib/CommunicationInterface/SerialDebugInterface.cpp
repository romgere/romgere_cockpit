
/**
 * Romgere Cockpit Library - https://github.com/romgere/romgere_cockpit
 * @author jerome@mestres.fr
 */


#include "SerialDebugInterface.h"


SerialDebugInterface::SerialDebugInterface( int serialSpeed){
    FakeData = new XPData();

    FakeData->data[0] = 0;
    FakeData->data[1] = 0;
    FakeData->data[2] = 0;
    FakeData->data[3] = 0;

    Serial.begin( serialSpeed);

}
SerialDebugInterface::~SerialDebugInterface(){
    delete FakeData;
}


uint8_t SerialDebugInterface::ReadAllInput(){
    return 0;
}


void SerialDebugInterface::SendCommand(const char* cmd){
    Serial.print("DebugInterface, SEND COMMAND : ");
    Serial.println(cmd);
}


void SerialDebugInterface::SendKey( const char* key){
    Serial.print("DebugInterface, SEND KEY : ");
    Serial.println(key);
}

void SerialDebugInterface::SendDrefCommand( const  char *dref, byte data[]){

    //Donnée à envoyer
    char DataOut[500];

    //"sim/"
    DataOut[0] = 's';
    DataOut[1] = 'i';
    DataOut[2] = 'm';
    DataOut[3] = '/';

    int i = 0;
    while( dref[i] != 0 && i < 499 ){
        DataOut[i+4] = dref[i];
        i++;
    }
    for(; i < 498; i++  ){
        DataOut[i] = char(32);
    }
    DataOut[499] = 0;


    Serial.print("DebugInterface, SEND DREF : ");
    Serial.print(DataOut);
    Serial.print(" with data : {");

    for (int i=0; i<4; i++){
        Serial.print( data[i]);
        if( i < 3 ) Serial.print("-");
    }
    Serial.println("}.");
}

//Retourne la donnée fictive avec le bon groupe
XPData* SerialDebugInterface::GetData( float group ){
    FakeData->group = group;
    return FakeData;
}
