/* Romgere Cockpit Application - http://romgere-xplane-baron58-cockpit.mestres.fr/
 * Réalisation Jérôme Mestres
 * Utilisation, modification et distribution autorisé pour une utilisation personnel non commercial.
 * L'auteur se reserve le droit à une utilisation commerciale.
 * L'auteur refuse tout utilisation commercial de tout ou partie de la librairie, en l'état ou modifiée.
 * Contact : http://romgere-xplane-baron58-cockpit.mestres.fr/
 */

#ifndef ARDUINROTARYSWITCHCLASS_H_INCLUDED
#define ARDUINROTARYSWITCHCLASS_H_INCLUDED

#include <Arduino.h>

#include "../Config/MainConfig.h"
#include "ArduinoControl.h"


//////////////////////////////////////////////////////////////
//Permet de controler des switch rotatif sur port Analogique
//////////////////////////////////////////////////////////////
class ArduinoRotarySwitchControl : public ArduinoInputControl{

    private :

        uint8_t Pin1;
        uint8_t NbPos;

        uint16_t Step;
        uint16_t Tolerance;

        uint8_t LastVal;

        bool RepeatCmd[MAX_COMMAND_FOR_ONE_CONTROLE]; //PErmet de définir si la commande doit être répété pour une position donnée

    public :
#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE
        ArduinoRotarySwitchControl( uint8_t pin, uint8_t nbpos, int boardAddress = -1);
#else
        ArduinoRotarySwitchControl( uint8_t pin, uint8_t nbpos);
#endif

        ~ArduinoRotarySwitchControl();
        bool ReadInput();
        float getValue();
        ArduinoRotarySwitchControl* setRepeat( uint8_t pos, bool repeat = true);
};

#endif // ARDUINROTARYSWITCHCLASS_H_INCLUDED
