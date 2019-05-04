/**
 * Romgere Cockpit Library - https://github.com/romgere/romgere_cockpit
 * @author jerome@mestres.fr
 */

#ifndef ARDUININCREMENTALTOGGLESWICTHCLASS_H_INCLUDED
#define ARDUININCREMENTALTOGGLESWICTHCLASS_H_INCLUDED

#include <Arduino.h>

#include "../Config/MainConfig.h"
#include "ArduinoControl.h"

///////////////////////////////////////////////////////////
//Permet de controler des toggle switchs à 3 positions
//avec commande incrémentiel (UP/DOWN)
///////////////////////////////////////////////////////////
#define INCREMENTAL_TOGGLESWITCH_CUR_STATUS_INDEX 0
#define INCREMENTAL_TOGGLESWITCH_OLD_STATUS_INDEX 1
class ArduinoIncrementalThreePosToggleSwitchControl : public ArduinoInputControl{

  public :

    private :

        uint8_t Pin1;
        uint8_t Pin2;

        uint8_t LastPinStatus[2];

    public :
#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE
        ArduinoIncrementalThreePosToggleSwitchControl( uint8_t pin1, uint8_t pin2, int boardAddress = -1);
#else
        ArduinoIncrementalThreePosToggleSwitchControl( uint8_t pin1, uint8_t pin2);
#endif
        ~ArduinoIncrementalThreePosToggleSwitchControl();
        bool ReadInput();
        float getValue();
};


#endif // ARDUININCREMENTALTOGGLESWICTHCLASS_H_INCLUDED
