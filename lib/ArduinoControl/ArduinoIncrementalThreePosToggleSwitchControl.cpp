/**
 * Romgere Cockpit Library - https://github.com/romgere/romgere_cockpit
 * @author jerome@mestres.fr
 */


#include "ArduinoIncrementalThreePosToggleSwitchControl.h"

#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE
ArduinoIncrementalThreePosToggleSwitchControl::ArduinoIncrementalThreePosToggleSwitchControl( uint8_t pin1, uint8_t pin2, int boardAddress)  : ArduinoInputControl(DigitalControl, ITypeToggleSwitch, (boardAddress != -1), boardAddress){
#else
ArduinoIncrementalThreePosToggleSwitchControl::ArduinoIncrementalThreePosToggleSwitchControl( uint8_t pin1, uint8_t pin2)  : ArduinoInputControl(DigitalControl, ITypeToggleSwitch){
#endif

    this->Pin1 = pin1;
    this->Pin2 = pin2;

    _pinMode(pin1, INPUT);
    _pinMode(pin2, INPUT);

    this->LastPinStatus[INCREMENTAL_TOGGLESWITCH_CUR_STATUS_INDEX] = this->LastPinStatus[INCREMENTAL_TOGGLESWITCH_OLD_STATUS_INDEX] = 0;

    bitWrite(this->LastPinStatus[INCREMENTAL_TOGGLESWITCH_CUR_STATUS_INDEX], 0, _digitalRead(this->Pin1) == HIGH);
    bitWrite(this->LastPinStatus[INCREMENTAL_TOGGLESWITCH_CUR_STATUS_INDEX], 1, _digitalRead(this->Pin2) == HIGH);

    this->LastPinStatus[INCREMENTAL_TOGGLESWITCH_OLD_STATUS_INDEX] = this->LastPinStatus[INCREMENTAL_TOGGLESWITCH_CUR_STATUS_INDEX];
}

ArduinoIncrementalThreePosToggleSwitchControl::~ArduinoIncrementalThreePosToggleSwitchControl() {}

bool ArduinoIncrementalThreePosToggleSwitchControl::ReadInput(){


    //Keep current status as old status and read new one
    this->LastPinStatus[INCREMENTAL_TOGGLESWITCH_OLD_STATUS_INDEX] = this->LastPinStatus[INCREMENTAL_TOGGLESWITCH_CUR_STATUS_INDEX];
    bitWrite(this->LastPinStatus[INCREMENTAL_TOGGLESWITCH_CUR_STATUS_INDEX], 0, _digitalRead(this->Pin1) == HIGH);
    bitWrite(this->LastPinStatus[INCREMENTAL_TOGGLESWITCH_CUR_STATUS_INDEX], 1, _digitalRead(this->Pin2) == HIGH);


    #ifdef DEBUG_CONTROL_STAT
        Serial.print("Control Debug : Read IncrementalThreePosToggleSwitch[");
        Serial.print(this->Pin1);
        Serial.print(",");
        Serial.print(this->Pin2);
        Serial.print("] Stat : ");
        Serial.print((this->LastPinStatus[INCREMENTAL_TOGGLESWITCH_OLD_STATUS_INDEX] != this->LastPinStatus[INCREMENTAL_TOGGLESWITCH_CUR_STATUS_INDEX]) ? "CHANGE !" : "NO CHANGE");
        Serial.print(", Value : ");
        Serial.print(this->LastPinStatus[INCREMENTAL_TOGGLESWITCH_OLD_STATUS_INDEX]);
        Serial.print(",");
        Serial.print(this->LastPinStatus[INCREMENTAL_TOGGLESWITCH_CUR_STATUS_INDEX]);
        Serial.println(".");
    #endif

    //Status changed ?
    return this->LastPinStatus[INCREMENTAL_TOGGLESWITCH_OLD_STATUS_INDEX] != this->LastPinStatus[INCREMENTAL_TOGGLESWITCH_CUR_STATUS_INDEX];
}

//On retourne une valeur de type float en fonction de l'état du toggle switch et du mode demandé
float ArduinoIncrementalThreePosToggleSwitchControl::getValue(){

    //Last position
    if( this->LastPinStatus[INCREMENTAL_TOGGLESWITCH_CUR_STATUS_INDEX] == 1 ){
        return 1;
    }
    //First position
    else if( this->LastPinStatus[INCREMENTAL_TOGGLESWITCH_CUR_STATUS_INDEX] == 2 ){
        return 0;
    }
    //Middle position, send same value as old status
    else{

        if( this->LastPinStatus[INCREMENTAL_TOGGLESWITCH_OLD_STATUS_INDEX] == 2 )
            return 1;
        else
            return 0;
    }

    //Not possible ! Command n°3 (Should be null)
    return 2;
}
