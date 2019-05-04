/* Romgere Cockpit Application - http://romgere-xplane-baron58-cockpit.mestres.fr/
 * Réalisation Jérôme Mestres
 * Utilisation, modification et distribution autorisé pour une utilisation personnel non commercial.
 * L'auteur se reserve le droit à une utilisation commerciale.
 * L'auteur refuse tout utilisation commercial de tout ou partie de la librairie, en l'état ou modifiée.
 * Contact : http://romgere-xplane-baron58-cockpit.mestres.fr/
 */

#ifndef XPLANECOMMANDCLASS_H_INCLUDED
#define XPLANECOMMANDCLASS_H_INCLUDED

#include <Arduino.h>


/*****************/
/* INPUT COMMAND */
/*****************/


//Donnée Xplane (Donnée type DREF reçue )
class XPlaneInputData{

    private :

        byte DataGroup; //Group de Données
        uint8_t DataIndex; //Index de la Donnée du groupe à utiliser

    public :

        XPlaneInputData(byte group, uint8_t index = 0) : DataGroup(group), DataIndex(index){};
        ~XPlaneInputData(){};

        byte getGroup(){ return DataGroup; };
        uint8_t getIndex(){ return DataIndex; };
};


/******************/
/* OUTPUT COMMAND */
/******************/

//Commande Xplane (Commande envoyée)
//classe virtuelle
class XPlaneOutputCommand{



    public :

        typedef enum{
            TypeKeyCommand = 0,
            TypeSimpleCommand,
            TypeDATACommand,
            TypeDREFCommand,
            TypeInternalCommand,
        }XPlaneCommandType;

    private :

        XPlaneCommandType CommandType;
        //float DataGroup;

    public :

        XPlaneOutputCommand(XPlaneCommandType type) : CommandType(type){};
        ~XPlaneOutputCommand(){};

        XPlaneCommandType getType(){ return CommandType; };

        virtual const char* toString() = 0;
        //virtual float getGroup(){return DataGroup; };
};



//Commande Xplane Type touche
class XPlaneKeyCommand : public XPlaneOutputCommand{

    private :

        char Key;

    public :


        XPlaneKeyCommand( const char c );
        ~XPlaneKeyCommand();

        const char* toString();
};


//Commande Xplane Classique type "sim/radios/com1_standy_flip"
class XPlaneSimpleCommand : public XPlaneOutputCommand{

    private :
        String Command;
        String *SecondCommand;
        bool doubleCommand;

    public :
        XPlaneSimpleCommand( const char* cmd, const char* cmd2 = NULL );
        ~XPlaneSimpleCommand();

        const char* toString();
        const char* toStringSecond();
        bool isDoubleCommand();
};


/******************/
/* NON IMPLEMENTE */
/******************/

//Commande Xplane type DATA
class XPlaneDATACommand : public XPlaneOutputCommand{

    public :
        XPlaneDATACommand() : XPlaneOutputCommand(TypeDATACommand){};
        ~XPlaneDATACommand(){};

        const char* toString(){ return "Not implemented";};
};



//Commande Xplane type DREF
class XPlaneDREFCommand : public XPlaneOutputCommand{

    public :
        XPlaneDREFCommand() : XPlaneOutputCommand(TypeDREFCommand){};
        ~XPlaneDREFCommand(){};

        const char* toString(){ return "Not implemented";};
};



/**********************/
/* COMMANDE "SYSTEME" */
/**********************/



//Commande "spécial" "System"
class LibrarySpecialCommand : public XPlaneOutputCommand{


    public :

     typedef enum{
            SendAll = 0, //Envoi toutes les commandes en fonction de l'état des contrôlee, même si les contrôle n'ont pas été modifié
            ResetArduino, //Redémarrage de l'arduino
            AllLedOn, //Allumer toutes les LED (pour test)
            AllLedOff, //Eteindre toutes les LED (pour test)
        }SpecialCommand;

    private :

        SpecialCommand cmd;

    public :


        LibrarySpecialCommand( SpecialCommand c ) : XPlaneOutputCommand(TypeInternalCommand){cmd = c;};
        ~LibrarySpecialCommand(){};

        const char* toString();
        SpecialCommand getCommand(){ return cmd; };
};


#endif
