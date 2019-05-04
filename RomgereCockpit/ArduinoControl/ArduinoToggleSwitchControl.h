/* Romgere Cockpit Application - http://romgere-xplane-baron58-cockpit.mestres.fr/
 * Réalisation Jérôme Mestres
 * Utilisation, modification et distribution autorisé pour une utilisation personnel non commercial.
 * L'auteur se reserve le droit à une utilisation commerciale.
 * L'auteur refuse tout utilisation commercial de tout ou partie de la librairie, en l'état ou modifiée.
 * Contact : http://romgere-xplane-baron58-cockpit.mestres.fr/
 */

#ifndef ARDUINTOGGLESWITCHCLASS_H_INCLUDED
#define ARDUINTOGGLESWITCHCLASS_H_INCLUDED

#include <Arduino.h>

#include "../Config/MainConfig.h"
#include "ArduinoControl.h"

///////////////////////////////////////////////////////////
//Permet de controler des toggle switchs (2 positions)
// ou des bouttons à deux position (la commande n'est pas répêtée)
///////////////////////////////////////////////////////////
class ArduinoToggleSwitchControl : public ArduinoInputControl{


    private :

        uint8_t Pin1;
        uint8_t LastPinStatus;


    public :
#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE
        ArduinoToggleSwitchControl( uint8_t pin, int boardAddress = -1);
#else
        ArduinoToggleSwitchControl( uint8_t pin);
#endif
        ~ArduinoToggleSwitchControl();
        bool ReadInput();
        float getValue();
};

#endif // ARDUINTOGGLESWITCHCLASS_H_INCLUDED
