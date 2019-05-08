/**
 * Romgere Cockpit Library - https://github.com/romgere/romgere_cockpit
 * @author jerome@mestres.fr
 */

#ifndef ARDUINOLEDCLASS_H_INCLUDED
#define ARDUINOLEDCLASS_H_INCLUDED

#include <Arduino.h>

#include "../Config/MainConfig.h"
#include "ArduinoControl.h"

//Allows use of LED
class ArduinoLEDControl : public ArduinoOutputControl{

    private :
        uint8_t Pin;
        float Val;
        float HighVal;

    public :

#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE
        ArduinoLEDControl( uint8_t pin, float highLEDVal = 1, int boardAddress = -1);
#else
        ArduinoLEDControl( uint8_t pin, float highLEDVal = 1);
#endif
        ~ArduinoLEDControl();

        void WriteOutput();
        bool setValue(float val);
};


/* TODO: Analogique LED control ? */

#endif // ARDUINOLEDCLASS_H_INCLUDED
