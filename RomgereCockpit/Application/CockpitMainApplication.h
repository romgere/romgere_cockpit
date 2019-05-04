/**
 * Romgere Cockpit Library - https://github.com/romgere/romgere_cockpit
 * @author jerome@mestres.fr
 */

#ifndef ROMGERECOCKPITCLASS_H_INCLUDED
#define ROMGERECOCKPITCLASS_H_INCLUDED

#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>

#include "../Config/MainConfig.h"

#include "../CommunicationInterface/BaseCommunicationInterface.h"

#include "../XPlaneCommand/XPlaneCommand.h"
#include "../ArduinoControl/ArduinoControl.h"

//Type pointer de fonction permettant de recevoir un tableau de float en paramètre (les données reçues des DREF Xplane)
//et retournant un float à envoyé au controle
typedef float (*inputDataTransformFunction)(float *);


//Gestion de l'application
class CockpitMainApplication{

private :

    //Permet de gérer les actions à effecuer lors d'un appel à loop()
    #if NUMBER_LOOP_SKIP_FOR_READ_ARDUINO_INPUT > 1 || NUMBER_LOOP_SKIP_FOR_READ_DATA_FROM_XPLANE > 1
    uint8_t LoopNumber;
    #endif

    //Class permettant la communication entre l'arduino et XPlane (Serial, NetWork, DEBUG)...
    BaseCommunicationInterface* CommunicationInterface;

    //Permet l'association des contrôle d'entrée avec les commande en sortie d'XPlane
    typedef struct{
            ArduinoInputControl*         Control; //Contrôle à associer
            XPlaneOutputCommand*    OutputCommand[MAX_COMMAND_FOR_ONE_CONTROLE]; //Commande (output) xplane associée
    }InputControlAssociation;

    InputControlAssociation* InputControlList[MAX_INPUT_CONTROL_IN_APPLICATION];
    unsigned int InputControlCount;

    //Permet l'association des contrôle de sortie avec les données en entrée d'XPlane
    typedef struct{
            ArduinoOutputControl*           Control; //Contrôle à associer
            XPlaneInputData*                IntputData; //Donnée (group de l'input) xplane associé
            inputDataTransformFunction      TransformationFunction; //Fonction permettant de "transformer" les données avant envoi vers le contrôle
    }OutputControlAssociation;
    unsigned int OutputControlCount;
    OutputControlAssociation* OutputControlList[MAX_OUTPUT_CONTROL_IN_APPLICATION];

    void updateControlWithCommand( OutputControlAssociation* oca); //Met à jour le contrôle en fonction de la commande.
    void doControlCommandProcess( InputControlAssociation* ica, bool forceSendingCommand = false ); //Lit le controle et envoi la commande si besoin

    void doInternalCommande( LibrarySpecialCommand *cmd); //Traite les commandes internes ("system")

#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE
    //Le "Wire" (bus I2C) a t-il été activé
    bool I2CActive;
    //Active le BUS I2C pour l'utilisation de carte Esclave
    void CheckAndInitI2CIfNeed( ArduinoControl *ctrl);
#endif

public :

    CockpitMainApplication( BaseCommunicationInterface *comInterface);
    ~CockpitMainApplication();

    //Déclare un controle de type input (switch, encoder, etc...) et la/les commande(s) output associée(s)
    void DeclareInputControl( ArduinoInputControl *ctrl,    XPlaneOutputCommand *cmd,  XPlaneOutputCommand *cmd1 = NULL,  XPlaneOutputCommand *cmd2 = NULL,
                                                            XPlaneOutputCommand *cmd3 = NULL, XPlaneOutputCommand *cmd4 = NULL, XPlaneOutputCommand *cmd5 = NULL,
                                                            XPlaneOutputCommand *cmd6 = NULL, XPlaneOutputCommand *cmd7 = NULL, XPlaneOutputCommand *cmd8 = NULL,
                                                            XPlaneOutputCommand *cmd9 = NULL, XPlaneOutputCommand *cmd10 = NULL);

    //Déclare un controle de type output (LED, servo, etc...) et la commande input associée
    void DeclareOutputControl( ArduinoOutputControl *ctrl,  XPlaneInputData *data, inputDataTransformFunction fct = NULL);



    //Lance l'échange de données entre Xplane et l'arduino (lecture des info xplane, MAJ des contrôles, intérogation des contrôles et envoi des données à XPlane)
    void Loop();

};



#endif
