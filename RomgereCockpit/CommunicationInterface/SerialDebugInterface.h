/* Romgere Cockpit Application - http://romgere-xplane-baron58-cockpit.mestres.fr/
 * Réalisation Jérôme Mestres
 * Utilisation, modification et distribution autorisé pour une utilisation personnel non commercial.
 * L'auteur se reserve le droit à une utilisation commerciale.
 * L'auteur refuse tout utilisation commercial de tout ou partie de la librairie, en l'état ou modifiée.
 * Contact : http://romgere-xplane-baron58-cockpit.mestres.fr/
 */

#ifndef ROMGERECOCKPITDEBUGINTERFACE_H_INCLUDED
#define ROMGERECOCKPITDEBUGINTERFACE_H_INCLUDED

#include <Arduino.h>

#include "../Config/MainConfig.h"
#include "BaseCommunicationInterface.h"


//Classe hérité par l'interface Network et Debug (et à terme Serial)
class SerialDebugInterface : public BaseCommunicationInterface{

    private :

        XPData  *FakeData;

    public :

        SerialDebugInterface( int serialSpeed = 9600 );
        ~SerialDebugInterface();

        //Lit et parse les données reçues d'Xplane, retourne le nombre de données (paquet Xplane) reçus
        uint8_t ReadAllInput();

        //Envoi une commande à Xplane
        void SendCommand(const char* cmd);

        //Envoi une touche à Xplane
        void SendKey( const char* key);

        //Envoi une commande DREF à Xplane
        void SendDrefCommand( const  char *dref, byte data[]);

        //Retourne les données d'un groupe de donnée reçu d'XPlane
        XPData* GetData( float group );
};



#endif // ROMGERECOCKPITDEBUGINTERFACE_H_INCLUDED
