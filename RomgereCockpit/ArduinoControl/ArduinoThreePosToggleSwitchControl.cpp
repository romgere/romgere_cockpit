/* Romgere Cockpit Application - http://romgere-xplane-baron58-cockpit.mestres.fr/
 * Réalisation Jérôme Mestres
 * Utilisation, modification et distribution autorisé pour une utilisation personnel non commercial.
 * L'auteur se reserve le droit à une utilisation commerciale.
 * L'auteur refuse tout utilisation commercial de tout ou partie de la librairie, en l'état ou modifiée.
 * Contact : http://romgere-xplane-baron58-cockpit.mestres.fr/
 */


#include "ArduinoThreePosToggleSwitchControl.h"


#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE
ArduinoThreePosToggleSwitchControl::ArduinoThreePosToggleSwitchControl( uint8_t pin1, uint8_t pin2, int boardAddress)  : ArduinoInputControl(DigitalControl, ITypeToggleSwitch, (boardAddress != -1), boardAddress){
#else
ArduinoThreePosToggleSwitchControl::ArduinoThreePosToggleSwitchControl( uint8_t pin1, uint8_t pin2)  : ArduinoInputControl(DigitalControl, ITypeToggleSwitch){
#endif

    this->Pin1 = pin1;
    this->Pin2 = pin2;

    _pinMode(pin1, INPUT);
    _pinMode(pin2, INPUT);

    LastPinStatus[0] = _digitalRead(pin1);
    LastPinStatus[1] = _digitalRead(pin2);
}

ArduinoThreePosToggleSwitchControl::~ArduinoThreePosToggleSwitchControl() {}

bool ArduinoThreePosToggleSwitchControl::ReadInput(){

    uint8_t last[2];

    //On garde une sauvegarde de l'état avant MAJ
    last[0] = this->LastPinStatus[0];
    last[1] = this->LastPinStatus[1];

    //On Met à jour l'état du contrôle
    this->LastPinStatus[0] = _digitalRead( this->Pin1);
    this->LastPinStatus[1] = _digitalRead( this->Pin2);

    #ifdef DEBUG_CONTROL_STAT
    Serial.print("Control Debug : Read TheePosToggleSwitch[");
    Serial.print(this->Pin1);
    Serial.print(",");
    Serial.print(this->Pin2);
    Serial.print("] Stat : ");
    Serial.print((this->LastPinStatus[0] != last[0] || this->LastPinStatus[1] != last[1]) ? "CHANGE !" : "NO CHANGE");
    Serial.print(", Value : ");
    Serial.print(this->LastPinStatus[0]);
    Serial.print(",");
    Serial.print(this->LastPinStatus[1]);
    Serial.println(".");
    #endif

    //Retourne TRUE si le controle à changé d'état...
    return this->LastPinStatus[0] != last[0] || this->LastPinStatus[1] != last[1];
}

//On retourne une valeur de type float en fonction de l'état du toggle switch et du mode demandé
float ArduinoThreePosToggleSwitchControl::getValue(){


    if( this->LastPinStatus[0] == HIGH && this->LastPinStatus[1] == LOW ){
        return 0;
    }
    else if( this->LastPinStatus[0] == LOW && this->LastPinStatus[1] == LOW ){
        return 1;
    }
    else if( this->LastPinStatus[0] == LOW && this->LastPinStatus[1] == HIGH ){
        return 2;
    }

    //Etat inconnu !?
    return 0;
}
