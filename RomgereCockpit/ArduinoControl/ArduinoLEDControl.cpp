/* Romgere Cockpit Application - http://romgere-xplane-baron58-cockpit.mestres.fr/
 * Réalisation Jérôme Mestres
 * Utilisation, modification et distribution autorisé pour une utilisation personnel non commercial.
 * L'auteur se reserve le droit à une utilisation commerciale.
 * L'auteur refuse tout utilisation commercial de tout ou partie de la librairie, en l'état ou modifiée.
 * Contact : http://romgere-xplane-baron58-cockpit.mestres.fr/
 */


#include "ArduinoLEDControl.h"


#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE
ArduinoLEDControl::ArduinoLEDControl( uint8_t pin, float defaultVal, float highLEDVal, int boardAddress): ArduinoOutputControl( DigitalControl, OTypeLed, (boardAddress != -1), boardAddress){
#else
ArduinoLEDControl::ArduinoLEDControl( uint8_t pin, float defaultVal, float highLEDVal): ArduinoOutputControl( DigitalControl, OTypeLed){
#endif

    this->Pin = pin;
    this->Val = (float)defaultVal;
    this->HighVal = highLEDVal;
    _pinMode(this->Pin, OUTPUT);

    WriteOutput();
}

ArduinoLEDControl::~ArduinoLEDControl(){}

void ArduinoLEDControl::WriteOutput(){
    _digitalWrite( this->Pin, this->Val == this->HighVal ?  HIGH : LOW);
}

bool ArduinoLEDControl::setValue(float val){
    float oldVal = this->Val;
    this->Val = val;
    return oldVal != this->Val;
}
