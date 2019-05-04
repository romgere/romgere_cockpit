/* Romgere Cockpit Application - http://romgere-xplane-baron58-cockpit.mestres.fr/
 * Réalisation Jérôme Mestres
 * Utilisation, modification et distribution autorisé pour une utilisation personnel non commercial.
 * L'auteur se reserve le droit à une utilisation commerciale.
 * L'auteur refuse tout utilisation commercial de tout ou partie de la librairie, en l'état ou modifiée.
 * Contact : http://romgere-xplane-baron58-cockpit.mestres.fr/
 */


#include "../Config/MainConfig.h"

#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE

#include <Wire.h>

#include "MasterToSlaveCommand.h"


MasterToSlaveCommand::MasterToSlaveCommand() : TypeCommande(TypeCommandUnknow), PinMode(PINModeNotSet), RWMode(RWModeNotSet), PinNum(0), ValueToSet(0){
}

MasterToSlaveCommand::MasterToSlaveCommand(MSCTypeCommand  type, MSCPINMode pin_mode, MSCRWMode rw_mode, uint8_t pin, int value) : TypeCommande(type), PinMode(pin_mode), RWMode(rw_mode), PinNum(pin), ValueToSet(value){
}

MasterToSlaveCommand::~MasterToSlaveCommand(){
}



void MasterToSlaveCommand::SendDataToIC2( uint8_t boardAddress ){

    byte buffer[4] = {0,0,0,0};

    //On créer le buffer de donnée en fonction de l'état de la classe
    CreateBufferForI2C( buffer);

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

    //On transmet à l'esclave
    Wire.beginTransmission(boardAddress);
    Wire.write((uint8_t*)buffer, 4);
    Wire.endTransmission();

    #ifdef DEBUG_I2C
    Serial.println("I2C : SEND OK.");
    #endif
}

//Envoi une requête vers une carte I2C, en envoyant préalablement la commande désirée
int MasterToSlaveCommand::RequestDataFromIC2( uint8_t boardAddress ){

    //On commence par envoyer la commande vers le slave (contenant l'info attendue)
    //SendDataToIC2( boardAddress); Non ! Car la transacition doit englober le write, le requestFrom et le read

    //On créer le buffer de donnée en fonction de l'état de la classe
    byte buffer[4] = {0,0,0,0};
    CreateBufferForI2C( buffer);

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

    //On transmet à l'esclave
    Wire.beginTransmission(boardAddress);
    Wire.write((uint8_t*)buffer, 4);

    //On attend un peu (vu sur beaucoup d'exemple d code simulaire à celui-ci)
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

    //Fin de transmission
    Wire.endTransmission();

    //Erreur ! Pas assez de donnée reçue
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

    //2 bytes reçues : OK, on retourne la valeur int
    return conv.intVal;
}


void MasterToSlaveCommand::CreateBufferForI2C( byte *buffer){



    //On créé notre flag sur le byte 1
    switch( TypeCommande ){
        case TypeCommandInitialisation :
            buffer[0] |= 0x00000001;        break;
        case TypeCommandSetPINValue :
            buffer[0] |= 0x00000002;        break;
        case TypeCommandGetPINValue :
            buffer[0] |= 0x00000004;        break;
    }
    switch( PinMode ){
        case PINModeInput :
            buffer[0] |= 0x00000008;        break;
        case PINModeOutput :
            buffer[0] |= 0x00000010;        break;
        case PINModeIntputPullUp :
            buffer[0] |= 0x00000020;        break;
    }
    switch( RWMode ){
        case RWModeAnalog :
            buffer[0] |= 0x00000040;        break;
        case RWModeDigital :
            buffer[0] |= 0x00000080;        break;
    }

    //Byte n°2 = PIN Mode
    buffer[1] = PinNum;

    //Byte n°3 & 4 value
    I2CDataRCA conv;
    conv.intVal = ValueToSet;
    buffer[2] = conv.byteVal[0];
    buffer[3] = conv.byteVal[1];
}


void MasterToSlaveCommand::ParseDataFromIC2( byte* data ){

    if( data[0] & 0x00000001)
        TypeCommande = TypeCommandInitialisation;
    else if( data[0] & 0x00000002)
        TypeCommande = TypeCommandSetPINValue;
    else if( data[0] & 0x00000004)
        TypeCommande = TypeCommandGetPINValue;

    if( data[0] & 0x00000080)
        PinMode = PINModeInput;
    else if( data[0] & 0x00000010)
        PinMode = PINModeOutput;
    else if( data[0] & 0x00000020)
        PinMode = PINModeIntputPullUp;

    if( data[0] & 0x00000040)
        RWMode = RWModeAnalog;
    else if( data[0] & 0x00000080)
        RWMode = RWModeDigital;


    //Byte n°2 = PIN Mode
    PinNum = data[1];

    //Byte n°3 & 4 value
    I2CDataRCA conv;

    conv.byteVal[0] = data[2];
    conv.byteVal[1] = data[3];

    ValueToSet = conv.intVal;
}

#endif
