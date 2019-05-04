/**
 * Romgere Cockpit Library - https://github.com/romgere/romgere_cockpit
 * @author jerome@mestres.fr
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
        Serial.print("Twin command registered : ");
        Serial.println(cmd2);
        twinCommand = true;
    }
    else{
        SecondCommand = NULL;
        twinCommand = false;
    }
}

XPlaneSimpleCommand::~XPlaneSimpleCommand(){

}

const char* XPlaneSimpleCommand::toString(){
    return Command.c_str();
}

bool XPlaneSimpleCommand::isTwinCommand(){
    return twinCommand;
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
