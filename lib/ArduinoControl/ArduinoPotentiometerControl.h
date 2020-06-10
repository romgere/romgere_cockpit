/**
 * Romgere Cockpit Library - https://github.com/romgere/romgere_cockpit
 * @author jerome@mestres.fr
 */

#ifndef ARDUINPOTENTIOMETERCLASS_H_INCLUDED
#define ARDUINPOTENTIOMETERCLASS_H_INCLUDED

#include <Arduino.h>

#include "../Config/MainConfig.h"
#include "ArduinoControl.h"


//Allows use of push button
class ArduinoPotentiometerControl : public ArduinoInputControl{

    private :
        uint8_t Pin1;
        float step;
        int nbStep;
        float min;
        float max;
        // use to detect changes
        float currentValue;
        float ReadAndConvertInput();

    public :
#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE
        ArduinoPotentiometerControl( uint8_t pin, float step = 0.05, float min = 0, float max = 1, int boardAddress = -1 );
#else
        ArduinoPotentiometerControl( uint8_t pin, float step = 0.05, float min = 0, float max = 1);
#endif
        ~ArduinoPotentiometerControl(){};
        bool ReadInput();
        float getValue(); //This control always send his first (and only) command
};

#endif // ARDUINPOTENTIOMETERCLASS_H_INCLUDED
