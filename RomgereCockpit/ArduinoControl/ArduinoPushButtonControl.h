/* Romgere Cockpit Application - http://romgere-xplane-baron58-cockpit.mestres.fr/
 * Réalisation Jérôme Mestres
 * Utilisation, modification et distribution autorisé pour une utilisation personnel non commercial.
 * L'auteur se reserve le droit à une utilisation commerciale.
 * L'auteur refuse tout utilisation commercial de tout ou partie de la librairie, en l'état ou modifiée.
 * Contact : http://romgere-xplane-baron58-cockpit.mestres.fr/
 */

#ifndef ARDUINPUSHBUTTONCLASS_H_INCLUDED
#define ARDUINPUSHBUTTONCLASS_H_INCLUDED

#include <Arduino.h>

#include "../Config/MainConfig.h"
#include "ArduinoControl.h"


///////////////////////////////////////////////////////////
//Permet de controler des button (push button)
///////////////////////////////////////////////////////////
class ArduinoPushButtonControl : public ArduinoInputControl{

    private :
        uint8_t Pin1;
        uint8_t DefaultPinStatus; //L'état apr défaut du contrôle (au repos), si l'état est différent la commande est systèmatiquement envoyée, même si l'etat n'a pas changé entre deux loop

    public :
#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE
        ArduinoPushButtonControl( uint8_t pin, uint8_t defautStat = LOW , int boardAddress = -1 );
#else
        ArduinoPushButtonControl( uint8_t pin, uint8_t defautStat = LOW );
#endif
        ~ArduinoPushButtonControl();
        bool ReadInput();
        float getValue();
};

#endif // ARDUINPUSHBUTTONCLASS_H_INCLUDED
