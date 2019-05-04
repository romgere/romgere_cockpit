/* Romgere Cockpit Application - http://romgere-xplane-baron58-cockpit.mestres.fr/
 * Réalisation Jérôme Mestres
 * Utilisation, modification et distribution autorisé pour une utilisation personnel non commercial.
 * L'auteur se reserve le droit à une utilisation commerciale.
 * L'auteur refuse tout utilisation commercial de tout ou partie de la librairie, en l'état ou modifiée.
 * Contact : http://romgere-xplane-baron58-cockpit.mestres.fr/
 */


#include "CockpitMainApplication.h"

#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE
    //Pour l'I2C (utilisation de carte maitre/esclave)
    #include <Wire.h>
#endif // ACTIVE_MULTI_ARDUINO_BOARD_MODE



CockpitMainApplication::CockpitMainApplication( BaseCommunicationInterface *comInterface){

    #ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE
    I2CActive = false;
    #endif // ACTIVE_MULTI_ARDUINO_BOARD_MODE


    #ifdef DEBUG_SERIAL_START
    Serial.begin(DEBUG_SERIAL_SPEED);
    #endif

    #ifdef DEBUG_LIBRARY
    Serial.println("CockpitMainApplication : APP STARTING...");
    Serial.println("CockpitMainApplication : Creating network interface...");
    #endif

    //Interface de communication vers/depuis XPlane (Ethernet/Serial/Debug)
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

    //Mise à vide des association controle/commande
    for( int i = 0; i < MAX_INPUT_CONTROL_IN_APPLICATION; i++ )
        InputControlList[i] = NULL;

    //Mise à vide des association controle/commande
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




//Déclare un controle de type input (switch, encoder, etc...) et la/les commande(s) output associée(s)
void CockpitMainApplication::DeclareInputControl( ArduinoInputControl *ctrl,    XPlaneOutputCommand *cmd,  XPlaneOutputCommand *cmd1,  XPlaneOutputCommand *cmd2,
                                                            XPlaneOutputCommand *cmd3, XPlaneOutputCommand *cmd4, XPlaneOutputCommand *cmd5,
                                                            XPlaneOutputCommand *cmd6, XPlaneOutputCommand *cmd7, XPlaneOutputCommand *cmd8,
                                                            XPlaneOutputCommand *cmd9, XPlaneOutputCommand *cmd10){

    if( InputControlCount >= MAX_INPUT_CONTROL_IN_APPLICATION)
        return;

    #ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE
    CheckAndInitI2CIfNeed( ctrl);
    #endif

    /* TODO controle en fonction du typre de control arduino (analog/digital)
        si type analogique alors commande 1 = Dref et autre NULL
        si type numerique, alors 1 à 3 comamnde de type char ou data
    */

    InputControlAssociation* assoc = new InputControlAssociation();



    assoc->Control = ctrl;
    assoc->OutputCommand[0] = cmd;
    assoc->OutputCommand[1] = cmd1;
    assoc->OutputCommand[2] = cmd2;

    //TODO: gérer via fonction "Variadic". possible ?
    //Commande "supplémentaire" selon config pour les rotary switch
    if( MAX_COMMAND_FOR_ONE_CONTROLE >= 4 )
        assoc->OutputCommand[3] = cmd3;
    if( MAX_COMMAND_FOR_ONE_CONTROLE >= 5 )
        assoc->OutputCommand[4] = cmd4;
    if( MAX_COMMAND_FOR_ONE_CONTROLE >= 6 )
        assoc->OutputCommand[5] = cmd5;
    if( MAX_COMMAND_FOR_ONE_CONTROLE >= 7 )
        assoc->OutputCommand[6] = cmd6;
    if( MAX_COMMAND_FOR_ONE_CONTROLE >= 8 )
        assoc->OutputCommand[7] = cmd7;
    if( MAX_COMMAND_FOR_ONE_CONTROLE >= 9 )
        assoc->OutputCommand[8] = cmd8;
    if( MAX_COMMAND_FOR_ONE_CONTROLE == 10 )
        assoc->OutputCommand[9] = cmd9;

    InputControlList[InputControlCount] = assoc;


    #ifdef DEBUG_CONTROL_COMMAND_ASSOC
    Serial.print("CockpitMainApplication : Declare INPUT control/command association, control : [");

    switch( ctrl->getInputType() ){
        case ArduinoInputControl::ITypePushButton :
            Serial.print("Push Button");break;
        case ArduinoInputControl::ITypeToggleSwitch:
            Serial.print("Toggle Switch");break;
        case ArduinoInputControl::ITypeRotarySwitch:
            Serial.print("Rotary Switch");break;
        case ArduinoInputControl::ITypeRotaryEncode:
            Serial.print("Rotary Encoder");break;
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
    #endif // DEBUG_CONTROL_COMMAND_ASSOC

    InputControlCount++;
}

//Déclare un controle de type output (LED, servo, etc...) et la commande input associée
void CockpitMainApplication::DeclareOutputControl( ArduinoOutputControl *ctrl,  XPlaneInputData *data, inputDataTransformFunction fct){

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


    #ifdef DEBUG_CONTROL_COMMAND_ASSOC
    Serial.print("CockpitMainApplication : Declare OUTPUT control/data association, control : [");

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
    #endif // DEBUG_CONTROL_COMMAND_ASSOC

    OutputControlCount++;
}

//Lance l'échange de donnèes entre Xplane et l'arduino (lecture des info xplane, MAJ des contrôles, intérogation des contrôles et envoi des données à XPlane)
void CockpitMainApplication::Loop(){


    #ifdef DEBUG_LIBRARY_LOOP
    Serial.print("CockpitMainApplication : Loop n°");
    Serial.print(LoopNumber);
    Serial.println("...");
    #endif

    //Read data from Xplane
    #if NUMBER_LOOP_SKIP_FOR_READ_DATA_FROM_XPLANE > 1
    if( LoopNumber % NUMBER_LOOP_SKIP_FOR_READ_DATA_FROM_XPLANE == 0){
    #endif

        #ifdef DEBUG_LIBRARY_LOOP
        Serial.println("CockpitMainApplication : Loop, process Input Data Reading...");
        #endif

        uint8_t nbDataRead = CommunicationInterface->ReadAllInput();

        //On met à jour les controles (ouput)
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

    //Read input Controle
    #if NUMBER_LOOP_SKIP_FOR_READ_ARDUINO_INPUT > 1
    if( LoopNumber % NUMBER_LOOP_SKIP_FOR_READ_ARDUINO_INPUT == 0){
    #endif

        #ifdef DEBUG_LIBRARY_LOOP
        Serial.println("CockpitMainApplication : Loop, process Input Control Reading...");
        #endif

        //Send data to Xplane
        for( unsigned int i = 0; i < InputControlCount; i++ ){

            #ifdef DEBUG_LIBRARY_LOOP
            Serial.print("CockpitMainApplication : Loop, process for control n°");
            Serial.print(i);
            Serial.println("...");
            #endif

            //Lit le control et envoi la commande si besoin
            this->doControlCommandProcess( InputControlList[i]);
        }
    #if NUMBER_LOOP_SKIP_FOR_READ_ARDUINO_INPUT > 1
    }

    LoopNumber++;

    #endif
}

//Met à jour le contrôle en fonction de la commande.
void CockpitMainApplication::updateControlWithCommand( OutputControlAssociation* oca ){

    XPlaneInputData* inputData  = oca->IntputData;
    ArduinoOutputControl* outputControl = oca->Control;

    //Récupère les datas du groupe associé au controle
    XPData*  dataXplane = CommunicationInterface->GetData( inputData->getGroup() );

    //Ok mise à jour du controle avec la data
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

        //Si changement de valeur, on met à jour le contrôle.
        if( outputControl->setValue( val) ){

            #ifdef DEBUG_LIBRARY_LOOP
            Serial.println(" => value as changed, update control.");
            #endif

            outputControl->WriteOutput();
        }
        #ifdef DEBUG_LIBRARY_LOOP
        else{
            Serial.println(" => value as NOT changed.");
        }
        #endif
    }
    #ifdef DEBUG_LIBRARY_LOOP
    else{
        Serial.println("CockpitMainApplication : Loop, NO data received for control.");
    }
    #endif
}

//Lit le controle et envoi la commande si besoin
void CockpitMainApplication::doControlCommandProcess( InputControlAssociation* ica, bool forceSendingCommand ){

    XPlaneOutputCommand* outputCmd = NULL;
    ArduinoControl* inputControl = ica->Control;

	//On lit l'état du control, si modifié, alors on traite la commande
	//Ou envoi la commande quel que soit l'état du contrôle si demandé (forceSendingCommand==true)
    if( forceSendingCommand || inputControl->ReadInput() ){

		//Récupère la valeur du controle sous forme float
		float val = inputControl->getValue();

        #ifdef DEBUG_LIBRARY_LOOP
        Serial.print("CockpitMainApplication : Loop, control was modified, new value:");
        Serial.print(val);
        Serial.println("...");
        #endif

		//Controle analogique
		if( inputControl->getType() == ArduinoControl::DigitalControl ){
			//Selon la valeur de retour du control, on prend une des commandes du tableau
			if( val < MAX_COMMAND_FOR_ONE_CONTROLE)
				outputCmd = ica->OutputCommand[(uint8_t)val];
		}
		else{
			// TODO controle analogique
            #ifdef DEBUG_LIBRARY_LOOP
            Serial.println("CockpitMainApplication : Loop, control type Analogic, not implemented!");
            #endif
		}

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

				//Commande classique
				case XPlaneOutputCommand::TypeSimpleCommand :

                    #ifdef DEBUG_LIBRARY_LOOP
                    Serial.print("CockpitMainApplication : Loop, Process Command Simple [");
                    Serial.print(outputCmd->toString());
                    Serial.println("] for control.");
                    #endif

					CommunicationInterface->SendCommand( outputCmd->toString());

                    //Deuxième commande à envoyer en meme temps ?
					if( ((XPlaneSimpleCommand*)outputCmd)->isDoubleCommand() ){

                        #ifdef DEBUG_LIBRARY_LOOP
                        Serial.print("CockpitMainApplication : Loop, Process SECOND Command Simple [");
                        Serial.print(((XPlaneSimpleCommand*)outputCmd)->toStringSecond());
                        Serial.println("] for control.");
                        #endif

                        CommunicationInterface->SendCommand( ((XPlaneSimpleCommand*)outputCmd)->toStringSecond());
					}
				break;

				//DREF / DATA : Envoi de DREF
				case XPlaneOutputCommand::TypeDREFCommand :
				case XPlaneOutputCommand::TypeDATACommand :
                    #ifdef DEBUG_LIBRARY_LOOP
                    Serial.println("CockpitMainApplication : Loop, Command type DATA or DREF for control, not implemented!");
                    #endif
				break;

				//Commande "system" : interne à la librairie
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
        else{
            Serial.println("CockpitMainApplication : Loop, no command for control value.");
        }
        #endif
	}
    #ifdef DEBUG_LIBRARY_LOOP
    else{
        Serial.println("CockpitMainApplication : Loop, control was not modified.");
    }
    #endif
}



void(* resetArduinoFunction) (void) = 0; //declare function reset à l'address 0

void CockpitMainApplication::doInternalCommande( LibrarySpecialCommand *cmd){


    switch( cmd->getCommand() ){

        //Envoi de toutes les commande quel que soit l'état du controle
        case LibrarySpecialCommand::SendAll :
            //Send data to Xplane
            for( unsigned int i = 0; i < InputControlCount; i++ ){

                if( InputControlList[i] != NULL ){

                    //Uniquement pour les toggle et rotary switch
                    if( InputControlList[i]->Control->getInputType() == ArduinoInputControl::ITypeRotarySwitch || InputControlList[i]->Control->getInputType() == ArduinoInputControl::ITypeToggleSwitch ){

                        //Lit le control et force lenvoi la commande si besoin
                        this->doControlCommandProcess( InputControlList[i], true);
                    }
                }
            }
        break;

        //RESET ARDUINO
        case LibrarySpecialCommand::ResetArduino :
            resetArduinoFunction();
        break;

        //Allumer/Eteindre toutes les LED
        case LibrarySpecialCommand::AllLedOn :
        case LibrarySpecialCommand::AllLedOff :

            for( unsigned int i = 0; i < OutputControlCount; i++ ){

                if( OutputControlList[i] != NULL ){

                    //Le controle en output
                    ArduinoOutputControl* outputControl = OutputControlList[i]->Control;

                    if( outputControl->getOutputType() == ArduinoOutputControl::OTypeLed ){
                        //Force l'allumage
                        outputControl->setValue( cmd->getCommand() == LibrarySpecialCommand::AllLedOn ? 1 : 0);
                        outputControl->WriteOutput();
                    }
                }
            }
        break;
    }

}

#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE

//Chaque contrôle déclaré (in ou out) à l'application est passé par cette fonction.
//Si le contrôle est déclaré comme situé sur une carte esclave, alors on active le bus I2C pour cette carte (comme maitre)
void CockpitMainApplication::CheckAndInitI2CIfNeed( ArduinoControl *ctrl){

    if( ctrl->isOnSlaveBoard() && ! I2CActive ){
        Wire.begin();        //joindre le bus i2c comme maître
    }
}

#endif
