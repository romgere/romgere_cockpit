/* Romgere Cockpit Application - http://romgere-xplane-baron58-cockpit.mestres.fr/
 * Réalisation Jérôme Mestres
 * Utilisation, modification et distribution autorisé pour une utilisation personnel non commercial.
 * L'auteur se reserve le droit à une utilisation commerciale.
 * L'auteur refuse tout utilisation commercial de tout ou partie de la librairie, en l'état ou modifiée.
 * Contact : http://romgere-xplane-baron58-cockpit.mestres.fr/
 */

#include "XPlaneCommand.h"


/***************/
/* KEY COMMAND */
/***************/



XPlaneKeyCommand::XPlaneKeyCommand( const char c ) : XPlaneOutputCommand(TypeKeyCommand), Key(c) {}

XPlaneKeyCommand::~XPlaneKeyCommand(){}

const char* XPlaneKeyCommand::toString(){
    return &Key;
}

/*********************/
/* COMMAND CLASSIQUE */
/*********************/


XPlaneSimpleCommand::XPlaneSimpleCommand( const char* cmd, const char* cmd2 ): XPlaneOutputCommand(TypeSimpleCommand), Command( cmd){
    if( cmd2 != NULL ){
        SecondCommand = new String( cmd2);
        Serial.println("DOUBLE COMAND DECLARE...");
        Serial.println(cmd2);
        doubleCommand = true;
    }
    else{
        SecondCommand = NULL;
        doubleCommand = false;
    }
}

XPlaneSimpleCommand::~XPlaneSimpleCommand(){

}

const char* XPlaneSimpleCommand::toString(){
    return Command.c_str();
}

bool XPlaneSimpleCommand::isDoubleCommand(){
    return doubleCommand;
}

const char* XPlaneSimpleCommand::toStringSecond(){
    return SecondCommand->c_str();
}

/******************/
/* SYSTEM COMMAND */
/******************/

const char* LibrarySpecialCommand::toString(){
    switch( cmd ){
    case SendAll :
        return "Send All";break;
    case ResetArduino :
        return "Reset";break;
    case AllLedOn :
        return "All LED ON";break;
    case AllLedOff :
        return "All LED OFF";break;
    }
    return "";
}

