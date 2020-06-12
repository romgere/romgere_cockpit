/**
 * Romgere Cockpit Library - https://github.com/romgere/romgere_cockpit
 * @author jerome@mestres.fr
 */



#include "SlaveBoardApplication.h"

#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE

#include <Wire.h>
#include "../Misc/MasterToSlaveCommand.h"




//Keep a global reference to SlaveBoardApplication instance to be able to notify it
SlaveBoardApplication* slaveAppInstance = NULL;
void globalReceivedData( int dataSize){
    slaveAppInstance->receivedData( dataSize);
}
void globalRequestEvent(){
   slaveAppInstance->requestEvent();
}




SlaveBoardApplication::SlaveBoardApplication( uint8_t adresse ) : BoardI2CAdresse(adresse), lastPinForValueSend(NULL){
    //set global refence to our SlaveBoardApplication instance
    slaveAppInstance = this;
}

//Register the board on I2C bus
void SlaveBoardApplication::RegisterI2C(){
    Wire.begin(BoardI2CAdresse);
    Wire.onReceive(globalReceivedData);
    Wire.onRequest(globalRequestEvent);
}

SlaveBoardApplication::~SlaveBoardApplication(){
    if( lastPinForValueSend != NULL ){
        delete lastPinForValueSend;
        lastPinForValueSend = NULL;
    }
}

//Main slave board app loop :
// - dequeue command from master board
// - do action (init/read) on pin
void SlaveBoardApplication::loop(){

    //Simply unqueue command
    while ( ! this->commandQueue.isEmpty () ){

        MasterToSlaveCommand *masterCommand =  this->commandQueue.dequeue();

        //Init PIN
        if( masterCommand->TypeCommande == MasterToSlaveCommand::TypeCommandInitialisation ){

            //Error !?
            if( masterCommand->PinMode == MasterToSlaveCommand::PINModeNotUsed ){
                break;
            }
            else{
                pinMode( masterCommand->PinNum, masterCommand->PinMode == MasterToSlaveCommand::PINModeInput ? INPUT : masterCommand->PinMode == MasterToSlaveCommand::PINModeIntputPullUp ? INPUT_PULLUP : OUTPUT);
            }
        }
        //Write on analog/digital PIN
        else if( masterCommand->TypeCommande == MasterToSlaveCommand::TypeCommandSetPINValue ){

            //Error !?
            if( masterCommand->RWMode == MasterToSlaveCommand::RWModeNotUsed ){
                break;
            }
            else if( masterCommand->RWMode == MasterToSlaveCommand::RWModeAnalog ){
                analogWrite( masterCommand->PinNum, masterCommand->ValueToSet);
            }
            else{
                digitalWrite( masterCommand->PinNum, masterCommand->ValueToSet);
            }
        }

        delete masterCommand;
    }
}

//Parse data from master Board
void SlaveBoardApplication::parseDataFromMaster( byte* data){

    MasterToSlaveCommand *masterCommand = new MasterToSlaveCommand();
    masterCommand->ParseDataFromIC2( data);

    //Master asking for PIN Value
    if( masterCommand->TypeCommande == MasterToSlaveCommand::TypeCommandGetPINValue ){

        if( lastPinForValueSend != NULL )
            delete lastPinForValueSend;

        //Keep the command (reference to arduino input PIN) to send it later when master board really ask for it (see "SlaveBoardApplication::requestEvent()")
        lastPinForValueSend = masterCommand;
    }
    //Enqueue write command
    else{
        this->commandQueue.enqueue( masterCommand);
    }
}

//Received data from master
void SlaveBoardApplication::receivedData( int dataSize){

    int index = 0;
    byte data[4];

    while(1 < Wire.available())
    {
        data[index++] = Wire.read();
    }
    data[index++] = Wire.read();

    parseDataFromMaster(data);
}

//Master board request a value
void SlaveBoardApplication::requestEvent(){

    I2CDataRCA conv;
    conv.intVal = 0;

    if( lastPinForValueSend != NULL ){

        if( lastPinForValueSend->TypeCommande == MasterToSlaveCommand::TypeCommandGetPINValue
            && (
                lastPinForValueSend->RWMode == MasterToSlaveCommand::RWModeAnalog
                || lastPinForValueSend->RWMode == MasterToSlaveCommand::RWModeDigital
              )
        ){

            if( lastPinForValueSend->RWMode == MasterToSlaveCommand::RWModeAnalog ){
                conv.intVal = analogRead(lastPinForValueSend->PinNum);
            }
            else if( lastPinForValueSend->RWMode == MasterToSlaveCommand::RWModeDigital ){
                conv.intVal = digitalRead(lastPinForValueSend->PinNum);
            }
        }
        //Error
        else{
            delete lastPinForValueSend;
            lastPinForValueSend = NULL;
            return;
        }

        delete lastPinForValueSend;
        lastPinForValueSend = NULL;
    }

    //Send value on I2C Bus
    Wire.write((uint8_t*)conv.byteVal, 2);
}

#endif
