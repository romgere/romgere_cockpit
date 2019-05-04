/* Romgere Cockpit Application - http://romgere-xplane-baron58-cockpit.mestres.fr/
 * Réalisation Jérôme Mestres
 * Utilisation, modification et distribution autorisé pour une utilisation personnel non commercial.
 * L'auteur se reserve le droit à une utilisation commerciale.
 * L'auteur refuse tout utilisation commercial de tout ou partie de la librairie, en l'état ou modifiée.
 * Contact : http://romgere-xplane-baron58-cockpit.mestres.fr/
 */


#include "ArduinoRotarySwitchControl.h"


#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE
ArduinoRotarySwitchControl::ArduinoRotarySwitchControl( uint8_t pin, uint8_t nbpos, int boardAddress) : ArduinoInputControl( DigitalControl, ITypeRotarySwitch, (boardAddress != -1), boardAddress){
#else
ArduinoRotarySwitchControl::ArduinoRotarySwitchControl( uint8_t pin, uint8_t nbpos) : ArduinoInputControl( DigitalControl, ITypeRotarySwitch){
#endif
    this->Pin1 = pin;

    _pinMode(this->Pin1, INPUT);

    this->NbPos = nbpos;

    for( int i= 0; i < MAX_COMMAND_FOR_ONE_CONTROLE; i ++ )
        this->RepeatCmd[i] =  false;

    //Pas entre 2 valeurs
    this->Step = 1024 / (this->NbPos-1);
    //Tolerance (écart min/max possible par rapport à la valeur attendu)
    this->Tolerance = this->Step*float(ROTARY_SWITCH_TOLERANCE);


    //pas de valeur précédente, on li la valeur mais sans envoyé de commande pour avoir l'état actuel
    this->LastVal  = -1;
    this->ReadInput();
}

ArduinoRotarySwitchControl::~ArduinoRotarySwitchControl(){}


ArduinoRotarySwitchControl* ArduinoRotarySwitchControl::setRepeat( uint8_t pos, bool repeat){
    if( pos < MAX_COMMAND_FOR_ONE_CONTROLE ){
        this->RepeatCmd[pos] = repeat;
    }

    //Permet de chainer les setRepeat.
    return this;
}

bool ArduinoRotarySwitchControl::ReadInput(){

    //Mémorise l'ancienne valeuir
    uint8_t oldVal = this->LastVal;


    //On lit l'actuelle
    int currentAnalog = _analogRead( this->Pin1);
    for( uint8_t i = 0; i < this->NbPos; i++ ){

        //Pour chaque position possible, on regarde si la valeur actuelle est comprise entre val pour la pos -/+ tolerance
        if( currentAnalog >= (int)((i*this->Step) - this->Tolerance) &&  currentAnalog <= (int)((i*this->Step) + this->Tolerance) ){
           this->LastVal = i;
           break;
        }
    }

    #ifdef DEBUG_CONTROL_STAT
    Serial.print("Control Debug : Read RotarySwitch[");
    Serial.print(this->Pin1);
    Serial.print("] Stat : ");
    Serial.print(this->LastVal != oldVal ? "CHANGE !" : "NO CHANGE");
    Serial.print(", Value : ");
    Serial.print(this->LastVal);
    Serial.print("(");
    Serial.print(currentAnalog);
    Serial.println(").");
    #endif

    //Si la valeur à changée ou qu'il faut répêter la commande, alors on retourne "vrai" pour déclancher l'envoi de la commande.
    if( this->LastVal != oldVal || this->RepeatCmd[this->LastVal])
        return true;

   return false;
}

float ArduinoRotarySwitchControl::getValue(){
    return this->LastVal;
}
