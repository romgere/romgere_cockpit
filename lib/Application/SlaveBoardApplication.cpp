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

    // Serial.print("SlaveBoardApplication : I2C Initialisation on adresse ");
    // Serial.print( adresse);
    // Serial.println("...");

    //set global refence to our SlaveBoardApplication instance
    slaveAppInstance = this;

    // Serial.println("SlaveBoardApplication : I2C Initialisation OK");
}

//Register the board on I2C bus
void SlaveBoardApplication::RegisterI2C(){

    // Serial.print("SlaveBoardApplication : Register I2C...");

    Wire.begin(BoardI2CAdresse);
    Wire.onReceive(globalReceivedData);
    Wire.onRequest(globalRequestEvent);

    // Serial.println("OK.");
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

    //On se contente de dépiler les commandes "en attente"
    while ( ! this->commandQueue.isEmpty () ){

        MasterToSlaveCommand *masterCommand =  this->commandQueue.dequeue();

        //Init PIN
        if( masterCommand->TypeCommande == MasterToSlaveCommand::TypeCommandInitialisation ){

            //Error !?
            if( masterCommand->PinMode == MasterToSlaveCommand::PINModeNotUsed ){
                // Serial.println("SlaveBoardApplication : Init PIN Erreur, bad parameter.");
            }
            else{
                // Serial.print("SlaveBoardApplication : pinMode(");
                // Serial.print(masterCommand->PinNum);
                // Serial.print(",");
                // Serial.print(masterCommand->PinMode == MasterToSlaveCommand::PINModeInput ? INPUT : masterCommand->PinMode == MasterToSlaveCommand::PINModeIntputPullUp ? INPUT_PULLUP : OUTPUT);
                // Serial.println(").");

                pinMode( masterCommand->PinNum, masterCommand->PinMode == MasterToSlaveCommand::PINModeInput ? INPUT : masterCommand->PinMode == MasterToSlaveCommand::PINModeIntputPullUp ? INPUT_PULLUP : OUTPUT);

            }
        }
        //Write on analog/digital PIN
        else if( masterCommand->TypeCommande == MasterToSlaveCommand::TypeCommandSetPINValue ){

            //Error !?
            if( masterCommand->RWMode == MasterToSlaveCommand::RWModeNotUsed ){
                // Serial.println("SlaveBoardApplication : Set PIN Erreur, bad parameter.");
            }
            else if( masterCommand->RWMode == MasterToSlaveCommand::RWModeAnalog ){

                // Serial.print("SlaveBoardApplication : analogWrite(");
                // Serial.print(masterCommand->PinNum);
                // Serial.print(",");
                // Serial.print(masterCommand->ValueToSet);
                // Serial.println(").");

                analogWrite( masterCommand->PinNum, masterCommand->ValueToSet);
            }
            else{

                // Serial.print("SlaveBoardApplication : digitalWrite(");
                // Serial.print(masterCommand->PinNum);
                // Serial.print(",");
                // Serial.print(masterCommand->ValueToSet);
                // Serial.println(").");

                digitalWrite( masterCommand->PinNum, masterCommand->ValueToSet);
            }
        }

        delete masterCommand;
    }
}

//Parse data from master Board
void SlaveBoardApplication::parseDataFromMaster( byte* data){

    // Serial.println("Parse...");

    MasterToSlaveCommand *masterCommand = new MasterToSlaveCommand();
    masterCommand->ParseDataFromIC2( data);


    // Serial.print("type");
    // Serial.println(masterCommand->TypeCommande);
    //     Serial.print("mode");
    //     Serial.println(masterCommand->PinMode);
    //         Serial.print("rwmode");
    //         Serial.println(masterCommand->RWMode);
    //             Serial.print("PinNum");
    //             Serial.println(masterCommand->PinNum);

    //Master asking for PIN Value
    if( masterCommand->TypeCommande == MasterToSlaveCommand::TypeCommandGetPINValue ){

        if( lastPinForValueSend != NULL )
            delete lastPinForValueSend;

        // Serial.println("SlaveBoardApplication : get PIN ask.");

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

    // Serial.print("I2C rec ");
    // Serial.print(index);
    // Serial.print(" : ");
    // for (int i=0; i < 4; i++){
    //     Serial.print((int)data[i]);Serial.print("-");
    // }
    // Serial.println(".");

    parseDataFromMaster(data);
}

//Master board request a value
void SlaveBoardApplication::requestEvent(){

    // Serial.println("I2C REQ");

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
                // Serial.print("SlaveBoardApplication : analogRead(");
                // Serial.print(lastPinForValueSend->PinNum);
                // Serial.println(");");
            }
            else if( lastPinForValueSend->RWMode == MasterToSlaveCommand::RWModeDigital ){

                conv.intVal = digitalRead(lastPinForValueSend->PinNum);

                // Serial.print("SlaveBoardApplication : digitalRead(");
                // Serial.print(lastPinForValueSend->PinNum);
                // Serial.println(");");
            }
        }
        //Error
        else{

            // Serial.println("I2C : REQ LAST DATA ERR");
            delete lastPinForValueSend;
            lastPinForValueSend = NULL;
            return;
        }

        delete lastPinForValueSend;
        lastPinForValueSend = NULL;
    }

    // Serial.print("I2C SEND ");
    // Serial.print(conv.intVal);
    // Serial.println(".");

    //Send value on I2C Bus
    Wire.write((uint8_t*)conv.byteVal, 2);
}

#endif
