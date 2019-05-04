/* Romgere Cockpit Application - http://romgere-xplane-baron58-cockpit.mestres.fr/
 * Réalisation Jérôme Mestres
 * Utilisation, modification et distribution autorisé pour une utilisation personnel non commercial.
 * L'auteur se reserve le droit à une utilisation commerciale.
 * L'auteur refuse tout utilisation commercial de tout ou partie de la librairie, en l'état ou modifiée.
 * Contact : http://romgere-xplane-baron58-cockpit.mestres.fr/
 */


#include "ArduinoRotaryEncoderControl.h"

/*

Source : http://svglobe.com/arduino/encoders.html


dir. / ret :    <==(-1)==    ==(+1)==>
           |-------------------------------|
    Bit2   | 1 | 1 | 0 | 0 | 1 | 1 | 0 | 0 |
    Bit1   | 0 | 1 | 1 | 0 | 0 | 1 | 1 | 0 |
           |-------------------------------|
    Val    | 2 | 3 | 1 | 0 | 2 | 3 | 1 | 0 |
           |-------------------------------|
Cran :
Enc Type1 : ╔═╗_╔═╗_╔═╗_╔═╗_╔═╗_╔═╗_╔═╗_╔═╗_
Enc Type2 : ╔═╗_____╔═╗_____╔═╗_____╔═╗_____
Enc Type3 : ____╔═╗_____╔═╗_____╔═╗_____╔═╗_
Enc Type4 : ____________╔═╗_____________╔═╗_
                         ▲
                         ║


Pour tous les type d'encoder la direction est toujours déterminer de la même façon mais les possitions possible ne sont pas les mêmes.
Pour éviter d'envoyer des commandes en double il faut vérifier la valeur avant d'envoyer la commande
*/
#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE
ArduinoRotaryEncoderControl::ArduinoRotaryEncoderControl( uint8_t pin1, uint8_t pin2, RotaryEncoderType type, bool usePullUpPin, int boardAddress) : ArduinoInputControl( DigitalControl, ITypeRotaryEncode, (boardAddress != -1), boardAddress){
#else
ArduinoRotaryEncoderControl::ArduinoRotaryEncoderControl( uint8_t pin1, uint8_t pin2, RotaryEncoderType type, bool usePullUpPin) : ArduinoInputControl( DigitalControl, ITypeRotaryEncode){
#endif

    this->Pin1 = pin1;
    this->Pin2 = pin2;

    EncoderType = type;

    _pinMode(pin1, usePullUpPin? INPUT_PULLUP : INPUT);
    _pinMode(pin2, usePullUpPin? INPUT_PULLUP : INPUT);


    this->LastPinStatus[ROTARY_ENC_CUR_STATUS_INDEX] = this->LastPinStatus[ROTARY_ENC_OLD_STATUS_INDEX] = 0;

    bitWrite(this->LastPinStatus[ROTARY_ENC_CUR_STATUS_INDEX], 0, _digitalRead(pin1) == HIGH);
    bitWrite(this->LastPinStatus[ROTARY_ENC_CUR_STATUS_INDEX], 1, _digitalRead(pin2) == HIGH);

    this->LastPinStatus[ROTARY_ENC_OLD_STATUS_INDEX] = this->LastPinStatus[ROTARY_ENC_CUR_STATUS_INDEX];



#ifdef USE_SECURE_TIME_STAT_CHANGE
        this->LastStatChangeTime = micros();
#endif // USE_SECURE_TIME_STAT_CHANGE

#ifdef USE_SECURE_TIME_DIR_CHANGE
        this->LastDirection = UnknowDirection;
        this->LastDirChangeTime = micros();
#endif // USE_SECURE_TIME_DIR_CHANGE

}

ArduinoRotaryEncoderControl::~ArduinoRotaryEncoderControl(){}


bool ArduinoRotaryEncoderControl::ReadInput(){

    //Statut courant = ancien statut
    this->LastPinStatus[ROTARY_ENC_OLD_STATUS_INDEX] = this->LastPinStatus[ROTARY_ENC_CUR_STATUS_INDEX];


    //On lit le nouveau statut
    uint8_t newVal = 0;
    /*
    bitWrite(newVal, 0, _digitalRead(this->Pin1) == HIGH);
    bitWrite(newVal, 1, _digitalRead(this->Pin2) == HIGH);
    /*/
    newVal += _digitalRead(this->Pin1) == HIGH ? 1 : 0;
    newVal += _digitalRead(this->Pin2) == HIGH ? 2 : 0;
    //*/

    bool isStatutsChanged = this->LastPinStatus[ROTARY_ENC_OLD_STATUS_INDEX] != newVal;

    #ifdef DEBUG_CONTROL_STAT
    Serial.print("Control Debug : Read RotaryEncoder[");
    Serial.print(this->Pin1);
    Serial.print(",");
    Serial.print(this->Pin2);
    Serial.print("] Stat : ");
    Serial.print(isStatutsChanged ? "CHANGE !" : "NO CHANGE");
    Serial.print(", Value : ");
    Serial.print(this->LastPinStatus[ROTARY_ENC_OLD_STATUS_INDEX]);
    Serial.print(",");
    Serial.print(newVal);
    Serial.println(".");
    #endif

//Mode "sécurisé" pour les mosue encoder (type3)
#ifdef USE_SECURE_MODE_FOR_MOUSE_ENCODER
    if( EncoderType == Type3Encoder ){

        //0 => passage à 1 ou 2
        if( this->LastPinStatus[ROTARY_ENC_OLD_STATUS_INDEX] == 0 && newVal == 3 ){
            #ifdef DEBUG_CONTROL_STAT
            Serial.print("Control Debug : Check RotaryEncoder, impossible stat 0 to 3 !");
            #endif
            return false;
        }
        //1 => passage à 0 ou 3
        else if( this->LastPinStatus[ROTARY_ENC_OLD_STATUS_INDEX] == 1 && newVal == 2 ){
            #ifdef DEBUG_CONTROL_STAT
            Serial.print("Control Debug : Check RotaryEncoder, impossible stat 1 to 2 !");
            #endif
            return false;
        }
        //2 => passage à 0 ou 3
        else if( this->LastPinStatus[ROTARY_ENC_OLD_STATUS_INDEX] == 2 && newVal == 1 ){
            #ifdef DEBUG_CONTROL_STAT
            Serial.print("Control Debug : Check RotaryEncoder, impossible stat 2 to 1 !");
            #endif
            return false;
        }
        //3 => passage à 1 ou 2
        else if( this->LastPinStatus[ROTARY_ENC_OLD_STATUS_INDEX] == 3 && newVal == 0 ){
            #ifdef DEBUG_CONTROL_STAT
            Serial.print("Control Debug : Check RotaryEncoder, impossible stat 3 to 0 !");
            #endif
            return false;
        }
    }
#endif // USE_SECURE_MODE_FOR_MOUSE_ENCODER


#ifdef USE_SECURE_TIME_STAT_CHANGE
    unsigned long timeDiff = micros() - this->LastStatChangeTime;
    if( timeDiff > 0 && timeDiff < MIN_STAT_CHANGE_TIME )
        return false;
    this->LastStatChangeTime = micros();
#endif // USE_SECURE_TIME_STAT_CHANGE

    //MAJ du membre de la class avec la nouvelle valeur
    this->LastPinStatus[ROTARY_ENC_CUR_STATUS_INDEX] = newVal;


    if( ! isStatutsChanged )
        return false;


    //Selon le type d'encodeur, certaine etat ne doivent pas être considéré comme un changement.
    //Pour les type 2, 3 ou 4, si la valeur courante ne correspond pas à une valeur de cran, on retourne FALSE (pas d'envoi de commande)
    switch( EncoderType){

        //Type 1 : chaque valeur correspond à un valeur possible pour un cran
        case Type1Encoder :
            return isStatutsChanged;

        //Type 2 : seul les valeurs 1 & 2 correspondent à un cran possible
        case Type2Encoder :
            if( this->LastPinStatus[ROTARY_ENC_CUR_STATUS_INDEX] == 1 || this->LastPinStatus[ROTARY_ENC_CUR_STATUS_INDEX] == 2 )
                return isStatutsChanged;
            else
                return false;

        //Type 3 : seul les valeurs 0 & 3 correspondent à un cran possible
        case Type3Encoder :
#ifdef USE_SECURE_MODE_FOR_MOUSE_ENCODER
            if(( this->LastPinStatus[ROTARY_ENC_CUR_STATUS_INDEX] == 0 && this->LastPinStatus[ROTARY_ENC_OLD_STATUS_INDEX] != 3) || ( this->LastPinStatus[ROTARY_ENC_CUR_STATUS_INDEX] == 3 && this->LastPinStatus[ROTARY_ENC_OLD_STATUS_INDEX] != 0) )
                return isStatutsChanged;
#else
            if( this->LastPinStatus[ROTARY_ENC_CUR_STATUS_INDEX] == 0 || this->LastPinStatus[ROTARY_ENC_CUR_STATUS_INDEX] == 3 )
                return isStatutsChanged;
#endif
            else
                return false;

        //Type 4 : seul la valeurs 0 correspond à un cran possible
        case Type4Encoder :
            if( this->LastPinStatus[ROTARY_ENC_CUR_STATUS_INDEX] == 0 )
                return isStatutsChanged;
            else
                return false;
    }

    //Unknow type !?
    return false;
}

float ArduinoRotaryEncoderControl::getValue(){

    RotaryEncoderDirection curDirection = UnknowDirection;

    switch( this->LastPinStatus[ROTARY_ENC_CUR_STATUS_INDEX] ){

        case 0 :
            curDirection = (this->LastPinStatus[ROTARY_ENC_OLD_STATUS_INDEX] == 1 ? UpDirection : DownDirection);break;
        case 1 :
            curDirection = (this->LastPinStatus[ROTARY_ENC_OLD_STATUS_INDEX] == 3 ? UpDirection : DownDirection);break;
        case 2 :
            curDirection = (this->LastPinStatus[ROTARY_ENC_OLD_STATUS_INDEX] == 0 ? UpDirection : DownDirection);break;
        case 3 :
            curDirection = (this->LastPinStatus[ROTARY_ENC_OLD_STATUS_INDEX] == 2 ? UpDirection : DownDirection);break;
    }

#ifdef USE_SECURE_TIME_DIR_CHANGE

    if( curDirection != this->LastDirection ){

        unsigned long timeDiff = micros() - this->LastDirChangeTime;

        //Trop rapide, on garde la direction précédente
        if( timeDiff > 0 && timeDiff < MIN_DIR_CHANGE_TIME )
            curDirection = this->LastDirection;
        //Ok, on peu changer de direction
        else
            this->LastDirection = curDirection;
    }

    this->LastDirChangeTime = micros();

#endif // USE_SECURE_TIME_DIR_CHANGE




    return (curDirection == UpDirection) ? 1 : 0;
}
