/**
 * Romgere Cockpit Library - https://github.com/romgere/romgere_cockpit
 * @author jerome@mestres.fr
 */

#ifndef ROMGERECOCKPITCOMMUNICATIONINTERFACE_H_INCLUDED
#define ROMGERECOCKPITCOMMUNICATIONINTERFACE_H_INCLUDED

#include <Arduino.h>
#include "../Config/MainConfig.h"


//Pour reception des infos XPlane
typedef struct{
    byte group;
    float data[8];
}XPData;

//Classe abstraite permettant la communication en entrée/sortie de l'application
//Classe hérité par l'interface Network et Debug (et à terme Serial)
class BaseCommunicationInterface{


    public :

        BaseCommunicationInterface(){};
        virtual ~BaseCommunicationInterface(){};

        //Lit et parse les données reçues d'Xplane, retourne le nombre de données (paquet Xplane) reçus
        virtual uint8_t ReadAllInput() = 0;

        //Envoi une commande à Xplane
        virtual void SendCommand(const char* cmd) = 0;

        //Envoi une touche à Xplane
        virtual void SendKey( const char* key) = 0;

        //Envoi une commande DREF à Xplane
        virtual void SendDrefCommand( const  char *dref, byte data[]){}; //Non virtuelle pure car non implémenter pour EthernetInterface

        //Retourne les données d'un groupe de donnée reçu d'XPlane
        virtual XPData* GetData( float group ) = 0;
};



#endif // ROMGERECOCKPITCOMMUNICATIONINTERFACE_H_INCLUDED
