/**
 * Romgere Cockpit Library - https://github.com/romgere/romgere_cockpit
 * @author jerome@mestres.fr
 */


#include "ArduinoControl.h"
#include "../Config/MainConfig.h"

/*********************************************************************************************************************/
/*************************************************   CONTROLE OUTPUT *************************************************/
/*********************************************************************************************************************/

#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE

#include "../Misc/MasterToSlaveCommand.h"

void    ArduinoControl::_pinMode(uint8_t p, uint8_t m){

    if( ControleOnSlaveBoard ){

        //On créer la commande I2C
        MasterToSlaveCommand slaveCMD(  MasterToSlaveCommand::TypeCommandInitialisation,
                                        (m == INPUT ? MasterToSlaveCommand::PINModeInput : (m == OUTPUT ? MasterToSlaveCommand::PINModeOutput : MasterToSlaveCommand::PINModeIntputPullUp)),
                                        MasterToSlaveCommand::RWModeNotSet,
                                        p,
                                        0 );
        //Puis on envoi
        slaveCMD.SendDataToIC2( SlaveBoardAddress);

    }
    else{
        pinMode(p, m);
    }
}

void    ArduinoControl::_digitalWrite(uint8_t p, uint8_t v){

    if( ControleOnSlaveBoard ){

        //On créer la commande I2C
        MasterToSlaveCommand slaveCMD(  MasterToSlaveCommand::TypeCommandSetPINValue,
                                        MasterToSlaveCommand::PINModeNotSet,
                                        MasterToSlaveCommand::RWModeDigital,
                                        p,
                                        v);
        //Puis on envoi
        slaveCMD.SendDataToIC2( SlaveBoardAddress);

    }
    else{
        digitalWrite(p, v);
    }
}

int     ArduinoControl::_digitalRead(uint8_t p){
    if( ControleOnSlaveBoard ){

        //On créer la commande I2C
        MasterToSlaveCommand slaveCMD(  MasterToSlaveCommand::TypeCommandGetPINValue,
                                        MasterToSlaveCommand::PINModeNotSet,
                                        MasterToSlaveCommand::RWModeDigital,
                                        p,
                                        0);

        //Puis on envoi la requête et on attend la réponse
        return slaveCMD.RequestDataFromIC2( SlaveBoardAddress);
    }
    else{
        return digitalRead(p);
    }
}

int     ArduinoControl::_analogRead(uint8_t p){

    if( ControleOnSlaveBoard ){
        //On créer la commande I2C
        MasterToSlaveCommand slaveCMD(  MasterToSlaveCommand::TypeCommandGetPINValue,
                                        MasterToSlaveCommand::PINModeNotSet,
                                        MasterToSlaveCommand::RWModeAnalog,
                                        p,
                                        0);

        //Puis on envoi la requête et on attend la réponse
        return slaveCMD.RequestDataFromIC2( SlaveBoardAddress);

    }
    else{
        return analogRead(p);
    }
}

void    ArduinoControl::_analogWrite(uint8_t p, int v){

     if( ControleOnSlaveBoard ){
        //On créer la commande I2C
        MasterToSlaveCommand slaveCMD(  MasterToSlaveCommand::TypeCommandSetPINValue,
                                        MasterToSlaveCommand::PINModeNotSet,
                                        MasterToSlaveCommand::RWModeAnalog,
                                        p,
                                        v);
        //Puis on envoi
        slaveCMD.SendDataToIC2( SlaveBoardAddress);

    }
    else{
        analogWrite( p, v);
    }
}

#endif //ACTIVE_MULTI_ARDUINO_BOARD_MODE
