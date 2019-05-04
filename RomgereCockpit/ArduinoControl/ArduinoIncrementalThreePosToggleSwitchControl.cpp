/* Romgere Cockpit Application - http://romgere-xplane-baron58-cockpit.mestres.fr/
 * Réalisation Jérôme Mestres
 * Utilisation, modification et distribution autorisé pour une utilisation personnel non commercial.
 * L'auteur se reserve le droit à une utilisation commerciale.
 * L'auteur refuse tout utilisation commercial de tout ou partie de la librairie, en l'état ou modifiée.
 * Contact : http://romgere-xplane-baron58-cockpit.mestres.fr/
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


    //Statut courant = ancien statut
    this->LastPinStatus[INCREMENTAL_TOGGLESWITCH_OLD_STATUS_INDEX] = this->LastPinStatus[INCREMENTAL_TOGGLESWITCH_CUR_STATUS_INDEX];


    //On lit le nouveau statut
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

    return this->LastPinStatus[INCREMENTAL_TOGGLESWITCH_OLD_STATUS_INDEX] != this->LastPinStatus[INCREMENTAL_TOGGLESWITCH_CUR_STATUS_INDEX];
}

//On retourne une valeur de type float en fonction de l'état du toggle switch et du mode demandé
float ArduinoIncrementalThreePosToggleSwitchControl::getValue(){

    //Dernière position possible, forcément commande n°2 (monte)
    if( this->LastPinStatus[INCREMENTAL_TOGGLESWITCH_CUR_STATUS_INDEX] == 1 ){
        return 1;
    }
    //Première position possible, forcément commande n°1 (descent)
    else if( this->LastPinStatus[INCREMENTAL_TOGGLESWITCH_CUR_STATUS_INDEX] == 2 ){
        return 0;
    }
    //Position intermédiaire, la commande dépend de l'état précédent
    else{

        //Ancienne position = premier, donc on "monte" => commande n°2
        if( this->LastPinStatus[INCREMENTAL_TOGGLESWITCH_OLD_STATUS_INDEX] == 2 )
            return 1;
        //Ancienne position = dernière, donc on "descend" => commande n°1
        else
            return 0;
    }

    //Normalement pas possible, donc commande n°3 qui doit de totue façon être null
    return 2;
}
