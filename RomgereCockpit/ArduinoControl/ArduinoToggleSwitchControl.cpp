/* Romgere Cockpit Application - http://romgere-xplane-baron58-cockpit.mestres.fr/
 * Réalisation Jérôme Mestres
 * Utilisation, modification et distribution autorisé pour une utilisation personnel non commercial.
 * L'auteur se reserve le droit à une utilisation commerciale.
 * L'auteur refuse tout utilisation commercial de tout ou partie de la librairie, en l'état ou modifiée.
 * Contact : http://romgere-xplane-baron58-cockpit.mestres.fr/
 */


#include "ArduinoToggleSwitchControl.h"


#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE
ArduinoToggleSwitchControl::ArduinoToggleSwitchControl( uint8_t pin, int boardAddress) : ArduinoInputControl( DigitalControl, ITypeToggleSwitch, (boardAddress != -1), boardAddress){
#else
ArduinoToggleSwitchControl::ArduinoToggleSwitchControl( uint8_t pin) : ArduinoInputControl( DigitalControl, ITypeToggleSwitch){
#endif

    this->Pin1 = pin;

    _pinMode(this->Pin1, INPUT);

    LastPinStatus = _digitalRead(this->Pin1);
}


ArduinoToggleSwitchControl::~ArduinoToggleSwitchControl() {}

bool ArduinoToggleSwitchControl::ReadInput(){
    //On garde une sauvegarde de l'état avant MAJ
    uint8_t last = this->LastPinStatus;
    //On Met à jour l'état du contrôle
    this->LastPinStatus = _digitalRead( this->Pin1);

    #ifdef DEBUG_CONTROL_STAT
    Serial.print("Control Debug : Read ToggleSwitch[");
    Serial.print(this->Pin1);
    Serial.print("] Stat : ");
    Serial.print(this->LastPinStatus != last ? "CHANGE !" : "NO CHANGE");
    Serial.print(", Value : ");
    Serial.print(this->LastPinStatus);
    Serial.println(".");
	#endif

    //Retourne TRUE si le controle à changé d'état...
    return this->LastPinStatus != last;
}

float ArduinoToggleSwitchControl::getValue(){
    return this->LastPinStatus == HIGH ? 1 : 0;
};
