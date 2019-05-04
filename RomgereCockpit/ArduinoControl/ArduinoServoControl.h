/* Romgere Cockpit Application - http://romgere-xplane-baron58-cockpit.mestres.fr/
 * Réalisation Jérôme Mestres
 * Utilisation, modification et distribution autorisé pour une utilisation personnel non commercial.
 * L'auteur se reserve le droit à une utilisation commerciale.
 * L'auteur refuse tout utilisation commercial de tout ou partie de la librairie, en l'état ou modifiée.
 * Contact : http://romgere-xplane-baron58-cockpit.mestres.fr/
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
