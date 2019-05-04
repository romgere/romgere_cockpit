/**
 * Romgere Cockpit Library - https://github.com/romgere/romgere_cockpit
 * @author jerome@mestres.fr
 */

#ifndef ARDUINTHREEPOSTOGGLESWITCHCLASS_H_INCLUDED
#define ARDUINTHREEPOSTOGGLESWITCHCLASS_H_INCLUDED

#include <Arduino.h>

#include "../Config/MainConfig.h"
#include "ArduinoControl.h"

///////////////////////////////////////////////////////////
//Permet de controler des toggle switchs à 3 positions
///////////////////////////////////////////////////////////
class ArduinoThreePosToggleSwitchControl : public ArduinoInputControl{

  public :

    private :

        uint8_t Pin1;
        uint8_t Pin2;

        uint8_t LastPinStatus[2];

    public :

#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE
        ArduinoThreePosToggleSwitchControl( uint8_t pin1, uint8_t pin2, int boardAddress = -1);
#else
        ArduinoThreePosToggleSwitchControl( uint8_t pin1, uint8_t pin2);
#endif

        ~ArduinoThreePosToggleSwitchControl();
        bool ReadInput();
        float getValue();
};


#endif // ARDUINTHREEPOSTOGGLESWITCHCLASS_H_INCLUDED
