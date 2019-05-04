/**
 * Romgere Cockpit Library - https://github.com/romgere/romgere_cockpit
 * @author jerome@mestres.fr
 */


#include "ArduinoPushButtonControl.h"

#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE
ArduinoPushButtonControl::ArduinoPushButtonControl( uint8_t pin, uint8_t defautStat, int boardAddress) : ArduinoInputControl( DigitalControl, ITypePushButton, (boardAddress != -1), boardAddress ){
#else
ArduinoPushButtonControl::ArduinoPushButtonControl( uint8_t pin, uint8_t defautStat) : ArduinoInputControl( DigitalControl, ITypePushButton){
#endif

    this->Pin1 = pin;
    //Position par Défaut
    this->DefaultPinStatus = defautStat == HIGH ? HIGH : LOW;

    _pinMode(this->Pin1, INPUT);
}

ArduinoPushButtonControl::~ArduinoPushButtonControl(){

}

//Retourne TRUE si le controle n'est pas dans son état par défaut
bool ArduinoPushButtonControl::ReadInput(){
#ifdef DEBUG_CONTROL_STAT
    int newVal = _digitalRead( this->Pin1);
    Serial.print("Control Debug : Read PushButton[");
    Serial.print(this->Pin1);
    Serial.print("] Stat : ");
    Serial.print(this->DefaultPinStatus != newVal ? "CHANGE !" : "NO CHANGE");
    Serial.print(", Value : ");
    Serial.print(newVal);
    Serial.println(".");

	return this->DefaultPinStatus != newVal;
#else
    return this->DefaultPinStatus != _digitalRead( this->Pin1);
#endif
}

float ArduinoPushButtonControl::getValue(){
    //le "Push Button" utilise toujours la première commande passée lors de la déclarion du controle
    return 0;
}
