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

#ifdef DEBUG_SERIAL_START
    Serial.begin(DEBUG_SERIAL_SPEED);
#endif

#ifdef DEBUG_SLAVE_APP
    Serial.print("SlaveBoardApplication : I2C Initialisation on adresse ");
    Serial.print( adresse);
    Serial.println("...");
#endif

    //set global refence to our SlaveBoardApplication instance
    slaveAppInstance = this;

#ifdef DEBUG_SLAVE_APP
    Serial.println("SlaveBoardApplication : I2C Initialisation OK");
#endif
}

//Register the board on I2C bus
void SlaveBoardApplication::RegisterI2C(){

#ifdef DEBUG_SLAVE_APP
    Serial.print("SlaveBoardApplication : Register I2C...");
#endif

    Wire.begin(BoardI2CAdresse);
    Wire.onReceive(globalReceivedData);
    Wire.onRequest(globalRequestEvent);

#ifdef DEBUG_SLAVE_APP
    Serial.println("OK.");
#endif
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

#ifdef DEBUG_SLAVE_APP
                Serial.println("SlaveBoardApplication : Init PIN Erreur, bad parameter.");
#endif

            }
            else{

#ifdef DEBUG_SLAVE_APP
                Serial.print("SlaveBoardApplication : pinMode(");
                Serial.print(masterCommand->PinNum);
                Serial.print(",");
                Serial.print(masterCommand->PinMode == MasterToSlaveCommand::PINModeInput ? INPUT : masterCommand->PinMode == MasterToSlaveCommand::PINModeIntputPullUp ? INPUT_PULLUP : OUTPUT);
                Serial.println(").");
#endif

                pinMode( masterCommand->PinNum, masterCommand->PinMode == MasterToSlaveCommand::PINModeInput ? INPUT : masterCommand->PinMode == MasterToSlaveCommand::PINModeIntputPullUp ? INPUT_PULLUP : OUTPUT);

            }
        }
        //Write on analog/digital PIN
        else if( masterCommand->TypeCommande == MasterToSlaveCommand::TypeCommandSetPINValue ){

            //Error !?
            if( masterCommand->RWMode == MasterToSlaveCommand::RWModeNotUsed ){

#ifdef DEBUG_SLAVE_APP
                Serial.println("SlaveBoardApplication : Set PIN Erreur, bad parameter.");
#endif

            }
            else if( masterCommand->RWMode == MasterToSlaveCommand::RWModeAnalog ){

#ifdef DEBUG_SLAVE_APP
                Serial.print("SlaveBoardApplication : analogWrite(");
                Serial.print(masterCommand->PinNum);
                Serial.print(",");
                Serial.print(masterCommand->ValueToSet);
                Serial.println(").");
#endif

                analogWrite( masterCommand->PinNum, masterCommand->ValueToSet);
            }
            else{

#ifdef DEBUG_SLAVE_APP
                Serial.print("SlaveBoardApplication : digitalWrite(");
                Serial.print(masterCommand->PinNum);
                Serial.print(",");
                Serial.print(masterCommand->ValueToSet);
                Serial.println(").");
#endif

                digitalWrite( masterCommand->PinNum, masterCommand->ValueToSet);
            }
        }

        delete masterCommand;
    }
}

//Parse data from master Board
void SlaveBoardApplication::parseDataFromMaster( byte* data){

#ifdef DEBUG_SLAVE_APP
    Serial.println("Parse...");
#endif

    MasterToSlaveCommand *masterCommand = new MasterToSlaveCommand();
    masterCommand->ParseDataFromIC2( data);



    //Master asking for PIN Value
    if( masterCommand->TypeCommande == MasterToSlaveCommand::TypeCommandGetPINValue ){

        if( lastPinForValueSend != NULL )
            delete lastPinForValueSend;

#ifdef DEBUG_SLAVE_APP
        Serial.println("SlaveBoardApplication : get PIN ask.");
#endif

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

#ifdef DEBUG_I2C
    Serial.print("I2C rec ");
    Serial.print(index);
    Serial.print(" : ");
    for (int i=0; i < 4; i++){
        Serial.print((int)data[i]);Serial.print("-");
    }
    Serial.println(".");
#endif // DEBUG_I2C4

    parseDataFromMaster(data);
}

//Master board request a value
void SlaveBoardApplication::requestEvent(){


#ifdef DEBUG_I2C
    Serial.println("I2C REQ");
#endif

    I2CDataRCA conv;
    conv.intVal = 0;

    if( lastPinForValueSend != NULL ){

        if( lastPinForValueSend->TypeCommande == MasterToSlaveCommand::TypeCommandGetPINValue && (lastPinForValueSend->PinMode == MasterToSlaveCommand::PINModeInput || lastPinForValueSend->PinMode == MasterToSlaveCommand::PINModeIntputPullUp) ){

            if( lastPinForValueSend->RWMode == MasterToSlaveCommand::RWModeAnalog ){

                conv.intVal = analogRead(lastPinForValueSend->PinNum);

#ifdef DEBUG_SLAVE_APP
                Serial.print("SlaveBoardApplication : analogRead(");
                Serial.print(lastPinForValueSend->PinNum);
                Serial.println(");");
#endif
            }
            else if( lastPinForValueSend->RWMode == MasterToSlaveCommand::RWModeDigital ){

                conv.intVal = digitalRead(lastPinForValueSend->PinNum);

#ifdef DEBUG_SLAVE_APP
                Serial.print("SlaveBoardApplication : digitalRead(");
                Serial.print(lastPinForValueSend->PinNum);
                Serial.println(");");
#endif
            }
        }
        //Error
        else{

#ifdef DEBUG_I2C
            Serial.println("I2C : REQ LAST DATA ERR");
#endif

            delete lastPinForValueSend;
            lastPinForValueSend = NULL;
            return;
        }

        delete lastPinForValueSend;
        lastPinForValueSend = NULL;
    }

#ifdef DEBUG_I2C
    Serial.print("I2C SEND ");
    Serial.print(conv.intVal);
    Serial.println(".");
#endif

    //Send value on I2C Bus
    Wire.write((uint8_t*)conv.byteVal, 2);
}

#endif
