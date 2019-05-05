/**
 * Romgere Cockpit Library - https://github.com/romgere/romgere_cockpit
 * @author jerome@mestres.fr
 */


#include "../Config/MainConfig.h"

#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE

#include <Wire.h>

#include "MasterToSlaveCommand.h"

//Send current command to Slave board on I2C bus
void MasterToSlaveCommand::SendDataToIC2( uint8_t boardAddress ){

    //Only for setPin or Init command
    if( this->TypeCommande !=  TypeCommandSetPINValue && this->TypeCommande != TypeCommandInitialisation )
      return;

    byte buffer[4] = {0,0,0,0};

    this->CreateBufferForI2C( buffer);

#ifdef DEBUG_I2C
    Serial.print("I2C : SEND TO ");
    Serial.print(boardAddress);
    Serial.print(" {");
    Serial.print(buffer[0]);
    Serial.print("-");
    Serial.print(buffer[1]);
    Serial.print("-");
    Serial.print(buffer[2]);
    Serial.print("-");
    Serial.print(buffer[3]);
    Serial.println("}.");
#endif

    //Send to slave board on I2C bus
    Wire.beginTransmission(boardAddress);
    Wire.write((uint8_t*)buffer, 4);
    Wire.endTransmission();

#ifdef DEBUG_I2C
    Serial.println("I2C : SEND OK.");
#endif
}

//Send a "GetPIN" command to slave board and wait return of slave board on I2C bus
int MasterToSlaveCommand::RequestDataFromIC2( uint8_t boardAddress ){

    //Only for getPin
    if( this->TypeCommande !=  TypeCommandGetPINValue )
        return 0;

    //We can't do this because one I2C transaction must include write, requestFrom and read actions
    //SendDataToIC2( boardAddress);

    byte buffer[4] = {0,0,0,0};
    this->CreateBufferForI2C( buffer);

#ifdef DEBUG_I2C
    Serial.print("I2C : SEND TO ");
    Serial.print(this->boardAddress);
    Serial.print(" {");
    Serial.print(buffer[0]);
    Serial.print("-");
    Serial.print(buffer[1]);
    Serial.print("-");
    Serial.print(buffer[2]);
    Serial.print("-");
    Serial.print(buffer[3]);
    Serial.println("}.");
#endif

    //Send to slave
    Wire.beginTransmission(boardAddress);
    Wire.write((uint8_t*)buffer, 4);

    //Wait a little bit (Seen on lot of other source code)
    delay(10);

    I2CDataRCA conv;

#ifdef DEBUG_I2C
    Serial.print("I2C : REQ FROM ");
    Serial.print(boardAddress);
    Serial.println("...");
#endif

    Wire.requestFrom( boardAddress, (uint8_t)2);
    uint8_t index = 0;
    while(Wire.available() && index < 2)
    {
        conv.byteVal[index++]  = Wire.read();
    }

    Wire.endTransmission();

    //Not enough data ! error
    if( index != 2 ){

#ifdef DEBUG_I2C
        Serial.println("I2C : REQ/REC HS.");
#endif

        return 0;
    }

#ifdef DEBUG_I2C
    Serial.print("I2C : REQ/REC int : ");
    Serial.print(conv.intVal);
    Serial.println(".");
#endif

    //2 bytes received : OK, return the received value as an integer
    return conv.intVal;
}

//Fill "data" buffer with bytes defining current command
void MasterToSlaveCommand::CreateBufferForI2C( byte *buffer){

    //First Byte : command type, pin mode and R/W mode
    switch( this->TypeCommande ){
        case TypeCommandInitialisation :
            buffer[0] |= 0x00000001; break;
        case TypeCommandSetPINValue :
            buffer[0] |= 0x00000002; break;
        case TypeCommandGetPINValue :
            buffer[0] |= 0x00000004; break;
    }
    switch( this->PinMode ){
        case PINModeInput :
            buffer[0] |= 0x00000008; break;
        case PINModeOutput :
            buffer[0] |= 0x00000010; break;
        case PINModeIntputPullUp :
            buffer[0] |= 0x00000020; break;
    }
    switch( this->RWMode ){
        case RWModeAnalog :
            buffer[0] |= 0x00000040; break;
        case RWModeDigital :
            buffer[0] |= 0x00000080; break;
    }

    //Second byte : PIN N°
    buffer[1] = PinNum;

    //Third and Fourth Byte : value (setPin command)
    I2CDataRCA conv;
    conv.intVal = this->ValueToSet;
    buffer[2] = conv.byteVal[0];
    buffer[3] = conv.byteVal[1];
}


//Parse datas from "data" buffer (received from I2C bus) and init current command attributes
void MasterToSlaveCommand::ParseDataFromIC2( byte* data ){

    //First Byte : command type, pin mode and R/W mode
    if( data[0] & 0x00000001)
        this->TypeCommande = TypeCommandInitialisation;
    else if( data[0] & 0x00000002)
        this->TypeCommande = TypeCommandSetPINValue;
    else if( data[0] & 0x00000004)
        this->TypeCommande = TypeCommandGetPINValue;

    if( data[0] & 0x00000080)
        this->PinMode = PINModeInput;
    else if( data[0] & 0x00000010)
        this->PinMode = PINModeOutput;
    else if( data[0] & 0x00000020)
        this->PinMode = PINModeIntputPullUp;

    if( data[0] & 0x00000040)
        this->RWMode = RWModeAnalog;
    else if( data[0] & 0x00000080)
        this->RWMode = RWModeDigital;

    //Second byte : PIN N°
    this->PinNum = data[1];

    //Third and Fourth Byte : value (setPin command)
    I2CDataRCA conv;

    conv.byteVal[0] = data[2];
    conv.byteVal[1] = data[3];

    this->ValueToSet = conv.intVal;
}

#endif
