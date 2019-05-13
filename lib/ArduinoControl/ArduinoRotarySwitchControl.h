/**
 * Romgere Cockpit Library - https://github.com/romgere/romgere_cockpit
 * @author jerome@mestres.fr
 */

#ifndef ARDUINROTARYSWITCHCLASS_H_INCLUDED
#define ARDUINROTARYSWITCHCLASS_H_INCLUDED

#include <Arduino.h>

#include "../Config/MainConfig.h"
#include "ArduinoControl.h"


//Allows use of rotary switch on analog PIN
class ArduinoRotarySwitchControl : public ArduinoInputControl{

    private :

        uint8_t Pin1;
        uint8_t positionCount;

        uint16_t Step;
        uint16_t Tolerance;

        uint8_t LastVal;

        //Allow one or more command to be repeat even if position of rotary switch don't change
        bool RepeatCmd[MAX_COMMAND_FOR_ONE_CONTROLE];

    public :
#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE
        ArduinoRotarySwitchControl( uint8_t pin, uint8_t positionCount, int boardAddress = -1);
#else
        ArduinoRotarySwitchControl( uint8_t pin, uint8_t positionCount);
#endif

        ~ArduinoRotarySwitchControl();
        bool ReadInput();
        float getValue();
        ArduinoRotarySwitchControl* setRepeat( uint8_t pos, bool repeat = true);
};

#endif // ARDUINROTARYSWITCHCLASS_H_INCLUDED
