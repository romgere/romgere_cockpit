/**
 * Romgere Cockpit Library - https://github.com/romgere/romgere_cockpit
 * @author jerome@mestres.fr
 */

#ifndef MASTERTOSLAVECLASS_H_INCLUDED

#define MASTERTOSLAVECLASS_H_INCLUDED


#include <Arduino.h>

#include "../Config/MainConfig.h"

 //Pour la conversion entier vers 2 byte à envoyer/recevoir via I2C
typedef union{
    byte byteVal[4];
    int intVal;
}I2CDataRCA;

//Pour envoi des commande entre maitre et esclave
// 1 byte : action + PINMode + Read/Write mode
// 1 byte : PIN
// 2 byte : Value

class MasterToSlaveCommand{

public :

    //Type de commande
    typedef enum{
        TypeCommandUnknow = 0,
        TypeCommandInitialisation,         //Initialisation d'une entrée/sortie
        TypeCommandSetPINValue,            //Définit la valeur d'un PIN (en sortie)
        TypeCommandGetPINValue             //Demande la valeur d'un PIN (en entrée)
    }MSCTypeCommand;

    //PIN Mode
    typedef enum{
        PINModeNotUsed = 0, //Utilisé pour des commande type set/get (pas besoin de connaitre le mode)
        PINModeInput,
        PINModeOutput,
        PINModeIntputPullUp
    }MSCPINMode;

    //Read/Write Mode
    typedef enum{
        RWModeNotUsed = 0, //Utilisé pour des commande type init
        RWModeAnalog,
        RWModeDigital
    }MSCRWMode;


    MSCTypeCommand  TypeCommande;
    MSCPINMode      PinMode;
    MSCRWMode       RWMode;
    uint8_t         PinNum;
    int             ValueToSet;

    MasterToSlaveCommand();
    MasterToSlaveCommand(MSCTypeCommand  type, MSCPINMode pin_mode, MSCRWMode rw_mode, uint8_t pin, int value);
    ~MasterToSlaveCommand();

    //Parse la commande (retrouve les valeurs) à partir de bytes reçues via I2C
    void ParseDataFromIC2( byte* data );

    //Envoi la commande via I2C vers une carte Esclave
    void SendDataToIC2( uint8_t boardAddress );

    //Envoi une requête vers une carte I2C, en envoyant préalablement la commande désirée
    int RequestDataFromIC2( uint8_t boardAddress );


private :

    void CreateBufferForI2C( byte* data);

};

#endif
