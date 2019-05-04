/**
 * Romgere Cockpit Library - https://github.com/romgere/romgere_cockpit
 * @author jerome@mestres.fr
 */

#ifndef XPLANENETWORKINGCLASS_H_INCLUDED
#define XPLANENETWORKINGCLASS_H_INCLUDED

#include <Arduino.h>
#include <Ethernet.h>

#include "../Config/MainConfig.h"
#include "BaseCommunicationInterface.h"


//Classe permettant la réception et l'envoi des données à partir/vers Xplane sur un SHIELD ETHERNET
class EthernetInterface : public BaseCommunicationInterface{


    private :

         typedef union{
            byte byteVal[4];
            float floatVal;
        }XPGroupData;

        //Dernières Données lues d'XPlane
        XPData* LastXPlaneDatas[MAX_INPUT_DATA_FROM_XPLANE];

        bool        IsClassInit; //Adresse Xplane définit ?

        bool        IsXPlaneAdressInit; //Adresse Xplane définit ?
        IPAddress   XPlaneAdress;       //Adresse d'XPlane

        unsigned int XPlaneWritePort;   //Port pour l'envoi de données vers XPlane
        unsigned int XplaneReadPort;    //Port pour la reception des données d'XPlane

        EthernetUDP Udp;                //Pour la gestion des packet UDP

    public :

        EthernetInterface(unsigned int readPort, unsigned int writePort, IPAddress arduinoIP, uint8_t arduinoMAC[6], IPAddress xplaneIP, bool waitForXPlane = false);
        ~EthernetInterface();

        //Lit et parse les données reçues d'Xplane, retourne le nombre de données (paquet Xplane) reçus
        uint8_t ReadAllInput();

        //Envoi une commande à Xplane
        void SendCommand(const char* cmd);

        //Envoi une touche à Xplane
        void SendKey( const char* key);

        //Envoi une commande DREF à Xplane
        //void SendDrefCommand( const  char *dref, byte data[]);

        //Retourne les données d'un groupe de donnée reçu d'XPlane
        XPData* GetData( float group );
};





#endif // XPLANENETWORKINGCLASS_H_INCLUDED
