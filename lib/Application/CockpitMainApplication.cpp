/**
 * Romgere Cockpit Library - https://github.com/romgere/romgere_cockpit
 * @author jerome@mestres.fr
 */


#include "CockpitMainApplication.h"

#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE
    //Include I2C support if needed
    #include <Wire.h>
#endif // ACTIVE_MULTI_ARDUINO_BOARD_MODE



CockpitMainApplication::CockpitMainApplication( BaseCommunicationInterface *comInterface){

#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE
    I2CInit = false;
#endif // ACTIVE_MULTI_ARDUINO_BOARD_MODE


#ifdef DEBUG_SERIAL_START
    Serial.begin(DEBUG_SERIAL_SPEED);
#endif

#ifdef DEBUG_LIBRARY
    Serial.println("CockpitMainApplication : APP STARTING...");
    Serial.println("CockpitMainApplication : Creating network interface...");
#endif

    //Communication interface from/to XPlane (Ethernet/Serial/Debug)
    CommunicationInterface = comInterface;


#ifdef DEBUG_LIBRARY
    Serial.println("CockpitMainApplication : Network interface created.");
    Serial.println("CockpitMainApplication : Init application datas.");
#endif

#if NUMBER_LOOP_SKIP_FOR_READ_ARDUINO_INPUT > 1 || NUMBER_LOOP_SKIP_FOR_READ_DATA_FROM_XPLANE > 1
    LoopNumber = 0;
#endif

    InputControlCount = 0;
    OutputControlCount = 0;

    //NULL Initialisation of control/command association (input)
    for( int i = 0; i < MAX_INPUT_CONTROL_IN_APPLICATION; i++ )
        InputControlList[i] = NULL;

    //NULL Initialisation of control/command association (output)
    for( int i = 0; i < MAX_OUTPUT_CONTROL_IN_APPLICATION; i++ )
        OutputControlList[i] = NULL;


#ifdef DEBUG_LIBRARY
    Serial.println("CockpitMainApplication : Application datas initialized.");
    Serial.println("CockpitMainApplication : APP STARTED.");
#endif
}


CockpitMainApplication::~CockpitMainApplication() {

    for( int i = 0; i < MAX_INPUT_CONTROL_IN_APPLICATION; i++ )
        if( InputControlList[i] != NULL )
            delete InputControlList[i];

    for( int i = 0; i < MAX_OUTPUT_CONTROL_IN_APPLICATION; i++ )
        if( OutputControlList[i] != NULL )
            delete OutputControlList[i];

    delete CommunicationInterface;
}




//Register an input control (switch, encode, ...) and bind it to one or more output command(s)
void CockpitMainApplication::RegisterInputControl(  ArduinoInputControl *ctrl,
                                                    XPlaneOutputCommand *cmd1,
                                                    ... ){

    if( InputControlCount >= MAX_INPUT_CONTROL_IN_APPLICATION)
        return;

#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE
    CheckAndInitI2CIfNeed( ctrl);
#endif

    /*
        TODO: Check args according to ArduinoInputControl (analog/digital)
        Analog : First command = DREF (and other NULL or ignored)
        Digital : One to three (Key, Simple, Data or Internal) command
    */

    InputControlAssociation* assoc = new InputControlAssociation();



    assoc->Control = ctrl;
    assoc->OutputCommand[0] = cmd1;

    va_list args;
    va_start(args, cmd1);

    XPlaneOutputCommand *nextCmd = NULL;
    uint8_t nextIndex = 1;

    //Additional command for multiple position input control (rotary switch, toggle switch ...)
    while ((nextCmd = va_arg(args, XPlaneOutputCommand *)) != NULL) {

        if ( nextIndex < MAX_COMMAND_FOR_ONE_CONTROLE ) {
            assoc->OutputCommand[ nextIndex] = nextCmd;
        }

        ++nextIndex;
    }

    va_end(args);

    InputControlList[InputControlCount] = assoc;

#ifdef DEBUG_CONTROL_COMMAND_REGISTER
    Serial.print("CockpitMainApplication : Register INPUT control/command association, control : [");

    switch( ctrl->getInputType() ){
        case ArduinoInputControl::ITypePushButton :
            Serial.print("Push Button");break;
        case ArduinoInputControl::ITypeToggleSwitch:
            Serial.print("Toggle Switch");break;
        case ArduinoInputControl::ITypeRotarySwitch:
            Serial.print("Rotary Switch");break;
        case ArduinoInputControl::ITypeRotaryEncode:
            Serial.print("Rotary Encoder");break;
        case ArduinoInputControl::ITypePotentiometer:
            Serial.print("Potentiometer");break;
    }

    Serial.print("] with command :");

    for( int i=0; i < MAX_COMMAND_FOR_ONE_CONTROLE; i++){

        if( assoc->OutputCommand[i] != NULL ){

            switch( assoc->OutputCommand[i]->getType() ){

                case XPlaneOutputCommand::TypeKeyCommand:
                    Serial.print(" [Key Command:");
                    Serial.print(assoc->OutputCommand[i]->toString());
                    break;
                case XPlaneOutputCommand::TypeSimpleCommand:
                    Serial.print(" [Simple Commande:");
                    Serial.print(assoc->OutputCommand[i]->toString());
                    break;
                case XPlaneOutputCommand::TypeDATACommand:
                    Serial.print(" [DATA Command:");
                    Serial.print(assoc->OutputCommand[i]->toString());
                    break;
                case XPlaneOutputCommand::TypeDREFCommand:
                    Serial.print(" [DREF command");
                    Serial.print(assoc->OutputCommand[i]->toString());
                    break;
                case XPlaneOutputCommand::TypeInternalCommand:
                    Serial.print(" [Internal Command");
                    Serial.print(assoc->OutputCommand[i]->toString());
                    break;
            }
            Serial.print("]");
        }
    }
    Serial.println(".");
#endif // DEBUG_CONTROL_COMMAND_REGISTER

    InputControlCount++;
}

//Register an output control (LED, servo, ...) and bind it to one intput data(s) (with optional "transformation“ function)
void CockpitMainApplication::RegisterOutputControl( ArduinoOutputControl *ctrl,  XPlaneInputData *data, inputDataTransformFunction fct){

    if( OutputControlCount >= MAX_OUTPUT_CONTROL_IN_APPLICATION)
        return;

#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE
    CheckAndInitI2CIfNeed( ctrl);
#endif

    OutputControlAssociation* assoc = new OutputControlAssociation();

    assoc->Control = ctrl;
    assoc->IntputData = data;
    assoc->TransformationFunction = fct;

    OutputControlList[OutputControlCount] = assoc;


#ifdef DEBUG_CONTROL_COMMAND_REGISTER
    Serial.print("CockpitMainApplication : Register OUTPUT control/data association, control : [");

    switch( ctrl->getOutputType() ){
        case ArduinoOutputControl::OTypeLed:
            Serial.print("LED");break;
        case ArduinoOutputControl::OTypeServo:
            Serial.print("Servo");break;
    }

    Serial.print("] with data : [group:");
    Serial.print(data->getGroup());
    Serial.print(",index:");
    Serial.print(data->getIndex());
    Serial.println("]");

    Serial.print(", ");
    Serial.print( fct != NULL ? "WITH" : "WITHOUT");
    Serial.print(" transform function");

    Serial.print(".");
#endif // DEBUG_CONTROL_COMMAND_REGISTER

    OutputControlCount++;
}

  //Main app loop :
  // - read xplane Datas
  // - update outpput controls
  // - read input controls
  // - send commands to X-plane
void CockpitMainApplication::Loop(){


#ifdef DEBUG_LIBRARY_LOOP
    Serial.print("CockpitMainApplication : Loop n°");
    Serial.print(LoopNumber);
    Serial.println("...");
#endif

    #if NUMBER_LOOP_SKIP_FOR_READ_DATA_FROM_XPLANE > 1
    if( LoopNumber % NUMBER_LOOP_SKIP_FOR_READ_DATA_FROM_XPLANE == 0){
#endif

#ifdef DEBUG_LIBRARY_LOOP
        Serial.println("CockpitMainApplication : Loop, process Input Data Reading...");
#endif

        //Read data from X-plane
        uint8_t nbDataRead = CommunicationInterface->ReadAllInput();

        //update ouput controls
        if( nbDataRead > 0 ){

#ifdef DEBUG_LIBRARY_LOOP
            Serial.print("CockpitMainApplication : ");
            Serial.print(nbDataRead);
            Serial.println(" data to read...");
#endif


            for( unsigned int i = 0; i < OutputControlCount; i++ ){

#ifdef DEBUG_LIBRARY_LOOP
                Serial.print("CockpitMainApplication : Loop, process for control n°");
                Serial.print(i);
                Serial.println("...");
#endif

                this->updateControlWithCommand( OutputControlList[i]);
            }
        }
        else{
#ifdef DEBUG_LIBRARY_LOOP
            Serial.println("CockpitMainApplication : Loop, No Input Data.");
#endif
        }
#if NUMBER_LOOP_SKIP_FOR_READ_DATA_FROM_XPLANE > 1
    }
#endif

#if NUMBER_LOOP_SKIP_FOR_READ_ARDUINO_INPUT > 1
    if( LoopNumber % NUMBER_LOOP_SKIP_FOR_READ_ARDUINO_INPUT == 0){
#endif

#ifdef DEBUG_LIBRARY_LOOP
        Serial.println("CockpitMainApplication : Loop, process Input Control Reading...");
#endif

        //Read input controls and send datas to X-plane
        for( unsigned int i = 0; i < InputControlCount; i++ ){

#ifdef DEBUG_LIBRARY_LOOP
            Serial.print("CockpitMainApplication : Loop, process for control n°");
            Serial.print(i);
            Serial.println("...");
#endif

            this->doControlCommandProcess( InputControlList[i]);
        }
#if NUMBER_LOOP_SKIP_FOR_READ_ARDUINO_INPUT > 1
    }
    LoopNumber++;
#endif
}

//Update output control if needed with value of binded input command
void CockpitMainApplication::updateControlWithCommand( OutputControlAssociation* oca ){

    XPlaneInputData* inputData  = oca->IntputData;
    ArduinoOutputControl* outputControl = oca->Control;

    //get group data of control
    XPGroupDatas*  dataXplane = CommunicationInterface->GetData( inputData->getGroup() );

    //update control with data received from X-plane
    if( dataXplane != NULL ){

#ifdef DEBUG_LIBRARY_LOOP
        Serial.print("CockpitMainApplication : Loop, data received for control, (new) value :");
#endif

        float val = 0;

        if( oca->TransformationFunction == NULL )
            val =  dataXplane->data[inputData->getIndex()];
        else
            val =  oca->TransformationFunction( dataXplane->data);

#ifdef DEBUG_LIBRARY_LOOP
        Serial.print(val);
#endif

        //Update control only if value changed ( setValue == true )
        if( outputControl->setValue( val) ){

#ifdef DEBUG_LIBRARY_LOOP
            Serial.println(" => value changed, update control.");
#endif

            outputControl->WriteOutput();
        }
#ifdef DEBUG_LIBRARY_LOOP
        else{
            Serial.println(" => value DON'T changed.");
        }
#endif
    }
#ifdef DEBUG_LIBRARY_LOOP
    else{
        Serial.println("CockpitMainApplication : Loop, NO data received for control.");
    }
#endif
}

//Read input control and send command to X-plane if needed
void CockpitMainApplication::doControlCommandProcess( InputControlAssociation* ica, bool forceSendingCommand ){

    XPlaneOutputCommand* outputCmd = NULL;
    ArduinoControl* inputControl = ica->Control;

    //Read input stat and send command if value changed (ReadInput == true) or if force sending ask (forceSendingCommand == true)
    if( forceSendingCommand || inputControl->ReadInput() ){

		    float val = inputControl->getValue();

#ifdef DEBUG_LIBRARY_LOOP
        Serial.print("CockpitMainApplication : Loop, control was modified, new value:");
        Serial.print(val);
        Serial.println("...");
#endif

		    if( inputControl->getType() == ArduinoControl::DigitalControl ){

            //Command is defined from control value and InputControlAssociation's command tab
			      if( val < MAX_COMMAND_FOR_ONE_CONTROLE)
				        outputCmd = ica->OutputCommand[(uint8_t)val];
		    }
		    else{
            // Always use first command for analog inputs
            outputCmd = ica->OutputCommand[0];

#ifdef DEBUG_LIBRARY_LOOP
            Serial.println("CockpitMainApplication : Loop, control type Analogic, not implemented!");
#endif
        }

        //We got a command to send
    		if( outputCmd != NULL ){

    		    switch( outputCmd->getType() ){

        				//Key
        				case XPlaneOutputCommand::TypeKeyCommand :
#ifdef DEBUG_LIBRARY_LOOP
                    Serial.print("CockpitMainApplication : Loop, Process Key Command [");
                    Serial.print(outputCmd->toString());
                    Serial.println("] for control.");
#endif
  	                CommunicationInterface->SendKey( outputCmd->toString());
        				break;

  				      //Simple command
  				      case XPlaneOutputCommand::TypeSimpleCommand :

#ifdef DEBUG_LIBRARY_LOOP
                    Serial.print("CockpitMainApplication : Loop, Process Command Simple [");
                    Serial.print(outputCmd->toString());
                    Serial.println("] for control.");
#endif

  					        CommunicationInterface->SendCommand( outputCmd->toString());

                    //Twin command (send 2 commands for one input)
  					        if( ((XPlaneSimpleCommand*)outputCmd)->isTwinCommand() ){

#ifdef DEBUG_LIBRARY_LOOP
                        Serial.print("CockpitMainApplication : Loop, Process SECOND Command Simple [");
                        Serial.print(((XPlaneSimpleCommand*)outputCmd)->toStringSecond());
                        Serial.println("] for control.");
#endif

                        CommunicationInterface->SendCommand( ((XPlaneSimpleCommand*)outputCmd)->toStringSecond());
  				          }
  			        break;

  				      //DREF / DATA command
  				      case XPlaneOutputCommand::TypeDREFCommand :

  #ifdef DEBUG_LIBRARY_LOOP
                    Serial.print("CockpitMainApplication : Loop, Process Dref Command [");
                    Serial.print(outputCmd->toString());
                    Serial.println("] for control.");
  #endif

                    CommunicationInterface->SendDrefCommand( outputCmd->toString(), val);
                break;

  				      case XPlaneOutputCommand::TypeDATACommand :
#ifdef DEBUG_LIBRARY_LOOP
                    Serial.println("CockpitMainApplication : Loop, Command type DATA for control, not implemented!");
#endif
  		          break;

  				      //System command : internal to library
  				      case XPlaneOutputCommand::TypeInternalCommand :

#ifdef DEBUG_LIBRARY_LOOP
                    Serial.print("CockpitMainApplication : Loop, Process Command System [");
                    Serial.print(outputCmd->toString());
                    Serial.println("] for control.");
#endif

  				          this->doInternalCommande((LibrarySpecialCommand*)outputCmd);
  		          break;
            }
        }
#ifdef DEBUG_LIBRARY_LOOP
        //No command
        else{
            Serial.println("CockpitMainApplication : Loop, no command for control value.");
        }
#endif
    }
#ifdef DEBUG_LIBRARY_LOOP
    //No change in control value
    else{
        Serial.println("CockpitMainApplication : Loop, control was not modified.");
    }
#endif
}


//Reset arduino (function at 0 address = cause arduino reset)
void(* resetArduinoFunction) (void) = 0;

//Deal with library internal command
void CockpitMainApplication::doInternalCommande( LibrarySpecialCommand *cmd){


    switch( cmd->getCommand() ){

        //Send all command of all (toggle switch and rotary switch) input control even if control was not modified
        case LibrarySpecialCommand::SendAll :
            for( unsigned int i = 0; i < InputControlCount; i++ ){

                if( InputControlList[i] != NULL ){

                    if( InputControlList[i]->Control->getInputType() == ArduinoInputControl::ITypeRotarySwitch || InputControlList[i]->Control->getInputType() == ArduinoInputControl::ITypeToggleSwitch ){
                        this->doControlCommandProcess( InputControlList[i], true);
                    }
                }
            }
        break;

        //Reset Arduino board
        case LibrarySpecialCommand::ResetArduino :
            resetArduinoFunction();
        break;

        //Turn on/off all LED output controls (Example : testing panel)
        case LibrarySpecialCommand::AllLedOn :
        case LibrarySpecialCommand::AllLedOff :

            for( unsigned int i = 0; i < OutputControlCount; i++ ){

                if( OutputControlList[i] != NULL ){

                    ArduinoOutputControl* outputControl = OutputControlList[i]->Control;

                    if( outputControl->getOutputType() == ArduinoOutputControl::OTypeLed && outputControl->setValue( cmd->getCommand() == LibrarySpecialCommand::AllLedOn ? 1 : 0) ){
                        outputControl->WriteOutput();
                    }
                }
            }
        break;
    }
}


#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE

//Init I2C interface if control is defined on slave arduno board (All registered control are passed to this methode)
void CockpitMainApplication::CheckAndInitI2CIfNeed( ArduinoControl *ctrl){

    if( ctrl->isOnSlaveBoard() && ! I2CInit ){
        Wire.begin();        //join i2c bus as master
    }
}

#endif
