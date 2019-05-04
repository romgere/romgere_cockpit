/**
 * Romgere Cockpit Library - https://github.com/romgere/romgere_cockpit
 * @author jerome@mestres.fr
 */

#ifndef ARDUINSERVOCONTROLCLASS_H_INCLUDED
#define ARDUINSERVOCONTROLCLASS_H_INCLUDED

#include <Arduino.h>

#include "../Config/MainConfig.h"
#include "ArduinoControl.h"

///////////////////////////////////////////////////////////
//Permet de controler un Servo moteur
///////////////////////////////////////////////////////////
class ArduinoServoControl : public ArduinoOutputControl{

    private :
        uint8_t Pin1;
        uint8_t Pin2;
        float Val;

    public :
#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE
        ArduinoServoControl( uint8_t pin1, uint8_t pin2, float defaultVal, int boardAddress = -1);
#else
        ArduinoServoControl( uint8_t pin1, uint8_t pin2, float defaultVal);
#endif
        ~ArduinoServoControl();

        void WriteOutput();
        bool setValue(float val);
};

#endif // ARDUINSERVOCONTROLCLASS_H_INCLUDED
