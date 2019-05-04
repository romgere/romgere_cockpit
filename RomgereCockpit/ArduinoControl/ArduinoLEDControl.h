/* Romgere Cockpit Application - http://romgere-xplane-baron58-cockpit.mestres.fr/
 * Réalisation Jérôme Mestres
 * Utilisation, modification et distribution autorisé pour une utilisation personnel non commercial.
 * L'auteur se reserve le droit à une utilisation commerciale.
 * L'auteur refuse tout utilisation commercial de tout ou partie de la librairie, en l'état ou modifiée.
 * Contact : http://romgere-xplane-baron58-cockpit.mestres.fr/
 */

#ifndef ARDUINOLEDCLASS_H_INCLUDED
#define ARDUINOLEDCLASS_H_INCLUDED

#include <Arduino.h>

#include "../Config/MainConfig.h"
#include "ArduinoControl.h"

///////////////////////////////////////////////////////////
//Permet de controler une LED
///////////////////////////////////////////////////////////
class ArduinoLEDControl : public ArduinoOutputControl{

    private :
        uint8_t Pin;
        float Val;
        float HighVal;

    public :

#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE
        ArduinoLEDControl( uint8_t pin, float defaultVal = 0, float highLEDVal = 1, int boardAddress = -1);
#else
        ArduinoLEDControl( uint8_t pin, float defaultVal = 0, float highLEDVal = 1);
#endif
        ~ArduinoLEDControl();

        void WriteOutput();
        bool setValue(float val);
};


/* TODO: class de type LED analogique */

#endif // ARDUINOLEDCLASS_H_INCLUDED
