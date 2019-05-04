/**
 * Romgere Cockpit Library - https://github.com/romgere/romgere_cockpit
 * @author jerome@mestres.fr
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
Detent :
Type1 Enc : ╔═╗_╔═╗_╔═╗_╔═╗_╔═╗_╔═╗_╔═╗_╔═╗_
Type2 Enc : ╔═╗_____╔═╗_____╔═╗_____╔═╗_____
Type3 Enc : ____╔═╗_____╔═╗_____╔═╗_____╔═╗_
Type4 Enc : ____________╔═╗_____________╔═╗_
                         ▲
                         ║

For all types of rotary encoder, the direction logic is the same. Only detents vary.
Command are send only if rotary is stop on a detent.
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

    //Keep current status as old status and read new one
    this->LastPinStatus[ROTARY_ENC_OLD_STATUS_INDEX] = this->LastPinStatus[ROTARY_ENC_CUR_STATUS_INDEX];
    uint8_t newVal = 0;
    /*
    bitWrite(newVal, 0, _digitalRead(this->Pin1) == HIGH);
    bitWrite(newVal, 1, _digitalRead(this->Pin2) == HIGH);
    /*/
    newVal += _digitalRead(this->Pin1) == HIGH ? 1 : 0;
    newVal += _digitalRead(this->Pin2) == HIGH ? 2 : 0;
    //*/

    bool isStatusChanged = this->LastPinStatus[ROTARY_ENC_OLD_STATUS_INDEX] != newVal;

    #ifdef DEBUG_CONTROL_STAT
    Serial.print("Control Debug : Read RotaryEncoder[");
    Serial.print(this->Pin1);
    Serial.print(",");
    Serial.print(this->Pin2);
    Serial.print("] Stat : ");
    Serial.print(isStatusChanged ? "CHANGE !" : "NO CHANGE");
    Serial.print(", Value : ");
    Serial.print(this->LastPinStatus[ROTARY_ENC_OLD_STATUS_INDEX]);
    Serial.print(",");
    Serial.print(newVal);
    Serial.println(".");
    #endif

//"Secure“ mode (for cheap type 3 mouse encoder)
#ifdef USE_SECURE_MODE_FOR_MOUSE_ENCODER
    if( EncoderType == Type3Encoder ){

        //Current 0 => allow only new stat to 1 or 2
        if( this->LastPinStatus[ROTARY_ENC_OLD_STATUS_INDEX] == 0 && newVal == 3 ){
            #ifdef DEBUG_CONTROL_STAT
            Serial.print("Control Debug : Check RotaryEncoder, impossible stat 0 to 3 !");
            #endif
            return false;
        }
        //Current 1 => allow only new stat to 0 or 3
        else if( this->LastPinStatus[ROTARY_ENC_OLD_STATUS_INDEX] == 1 && newVal == 2 ){
            #ifdef DEBUG_CONTROL_STAT
            Serial.print("Control Debug : Check RotaryEncoder, impossible stat 1 to 2 !");
            #endif
            return false;
        }
        //Current 2 => allow only new stat to 0 or 3
        else if( this->LastPinStatus[ROTARY_ENC_OLD_STATUS_INDEX] == 2 && newVal == 1 ){
            #ifdef DEBUG_CONTROL_STAT
            Serial.print("Control Debug : Check RotaryEncoder, impossible stat 2 to 1 !");
            #endif
            return false;
        }
        //Current 3 => allow only new stat to 1 or 2
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
    //Too fast, no change allowed yet
    if( timeDiff > 0 && timeDiff < MIN_STAT_CHANGE_TIME )
        return false;
    this->LastStatChangeTime = micros();
#endif // USE_SECURE_TIME_STAT_CHANGE


    this->LastPinStatus[ROTARY_ENC_CUR_STATUS_INDEX] = newVal;

    if( ! isStatusChanged )
        return false;


    //Selon le type d'encodeur, certaine etat ne doivent pas être considéré comme un changement.
    //Pour les type 2, 3 ou 4, si la valeur courante ne correspond pas à une valeur de cran, on retourne FALSE (pas d'envoi de commande)
    //According to the encoder type, somes states does not corresponding to a detent, return false in these case.
    switch( EncoderType){

        //Type 1 : each value can correspond to a detent
        case Type1Encoder :
            return isStatusChanged;

        //Type 2 : Only 1 & 2 values can correspond to a detent
        case Type2Encoder :
            if( this->LastPinStatus[ROTARY_ENC_CUR_STATUS_INDEX] == 1 || this->LastPinStatus[ROTARY_ENC_CUR_STATUS_INDEX] == 2 )
                return isStatusChanged;
            else
                return false;

        //Type 3 : Only 0 & 3 values can correspond to a detent
        case Type3Encoder :
#ifdef USE_SECURE_MODE_FOR_MOUSE_ENCODER
            if(( this->LastPinStatus[ROTARY_ENC_CUR_STATUS_INDEX] == 0 && this->LastPinStatus[ROTARY_ENC_OLD_STATUS_INDEX] != 3) || ( this->LastPinStatus[ROTARY_ENC_CUR_STATUS_INDEX] == 3 && this->LastPinStatus[ROTARY_ENC_OLD_STATUS_INDEX] != 0) )
                return isStatusChanged;
#else
            if( this->LastPinStatus[ROTARY_ENC_CUR_STATUS_INDEX] == 0 || this->LastPinStatus[ROTARY_ENC_CUR_STATUS_INDEX] == 3 )
                return isStatusChanged;
#endif
            else
                return false;

        //Type 4 : Only 0 value can correspond to a detent
        case Type4Encoder :
            if( this->LastPinStatus[ROTARY_ENC_CUR_STATUS_INDEX] == 0 )
                return isStatusChanged;
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

        //Too fast, keep previous direction
        if( timeDiff > 0 && timeDiff < MIN_DIR_CHANGE_TIME )
            curDirection = this->LastDirection;
        //Timing OK, we can accept a direction change
        else
            this->LastDirection = curDirection;
    }

    this->LastDirChangeTime = micros();

#endif // USE_SECURE_TIME_DIR_CHANGE




    return (curDirection == UpDirection) ? 1 : 0;
}
