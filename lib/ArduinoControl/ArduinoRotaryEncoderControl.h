/**
 * Romgere Cockpit Library - https://github.com/romgere/romgere_cockpit
 * @author jerome@mestres.fr
 */

#ifndef ARDUINOROTARYENCODECLASS_H_INCLUDED
#define ARDUINOROTARYENCODECLASS_H_INCLUDED

#include <Arduino.h>

#include "../Config/MainConfig.h"
#include "ArduinoControl.h"

///////////////////////////////////////////////////////////
//Permet de controler des encodeur rotatif
///////////////////////////////////////////////////////////
#define ROTARY_ENC_CUR_STATUS_INDEX 0
#define ROTARY_ENC_OLD_STATUS_INDEX 1
class ArduinoRotaryEncoderControl : public ArduinoInputControl{

  public :

        //Type d'encodeur
        typedef enum{
            Type1Encoder = 0,
            Type2Encoder,
            Type3Encoder,
            Type4Encoder
        }RotaryEncoderType;

        //Direction de l'encoder
        typedef enum{
            UnknowDirection = 0,
            UpDirection,
            DownDirection,
        }RotaryEncoderDirection;


    private :

        uint8_t Pin1;
        uint8_t Pin2;

        //On garde un historique des 2 dernières valeurs de l'encodeur
        // ROTARY_ENC_CUR_STATUS_INDEX(0) = Dernier statut lu
        // ROTARY_ENC_OLD_STATUS_INDEX(1) = Ancien statut lu
        uint8_t LastPinStatus[2];

#ifdef USE_SECURE_TIME_STAT_CHANGE
        unsigned long LastStatChangeTime; //Dernier changement d'état
#endif // USE_SECURE_TIME_STAT_CHANGE

#ifdef USE_SECURE_TIME_DIR_CHANGE
        RotaryEncoderDirection LastDirection;
        unsigned long LastDirChangeTime; //Dernier changement de direction
#endif // USE_SECURE_TIME_DIR_CHANGE

        RotaryEncoderType EncoderType;

    public :
#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE
        ArduinoRotaryEncoderControl( uint8_t pin1, uint8_t pin2, RotaryEncoderType type, bool usePullUpPin = false, int boardAddress = -1);
#else
        ArduinoRotaryEncoderControl( uint8_t pin1, uint8_t pin2, RotaryEncoderType type, bool usePullUpPin = false);
#endif
        ~ArduinoRotaryEncoderControl();
        bool ReadInput();
        //RotaryEncoderDirection getValue();
        float getValue();
};

#endif // ARDUINOROTARYENCODECLASS_H_INCLUDED
