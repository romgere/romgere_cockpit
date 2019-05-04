/* Romgere Cockpit Application - http://romgere-xplane-baron58-cockpit.mestres.fr/
 * Réalisation Jérôme Mestres
 * Utilisation, modification et distribution autorisé pour une utilisation personnel non commercial.
 * L'auteur se reserve le droit à une utilisation commerciale.
 * L'auteur refuse tout utilisation commercial de tout ou partie de la librairie, en l'état ou modifiée.
 * Contact : http://romgere-xplane-baron58-cockpit.mestres.fr/
 */

#include "../Config/MainConfig.h"


#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE

#ifndef ROMGERESLAVEBOARDCLASS_H_INCLUDED

#define ROMGERESLAVEBOARDCLASS_H_INCLUDED

#include <Arduino.h>
#include <Wire.h>

// include queue library header (from http://playground.arduino.cc/Code/QueueArray)
#include "../Misc/QueueArray.h"

#include "../Misc/MasterToSlaveCommand.h"




class SlaveBoardApplication{

    private :

        uint8_t BoardI2CAdresse;

        MasterToSlaveCommand* lastPinForValueSend;    //Dernière commande de type get envoyée

        //Pile de commande reçue. Permet de stocker les commandes reçues par I²C sans faire d'interraction
        //sur les entrée/sortie arduino pour libérer plus vite le I²C.
        //Les commandes sont ensuite dépilées par le programme principal via la méthode loop.
        //
        //Voir http://playground.arduino.cc/Code/QueueArray
        QueueArray <MasterToSlaveCommand*> commandQueue;

    public :

        SlaveBoardApplication( uint8_t adresse );
        ~SlaveBoardApplication();

        //S'enregistre sur le port I²C et commence à recevoir les messages du master
        void RegisterI2C();

        //A appeler dans la loop du programme principal pour dépiler les action (write/read/init sur les input/output arduino)
        //Permet de sortir ces actions des événements (interruption) I²C
        void loop();

        //On reçoi des données du Maitre
        void receivedData( int dataSize);

        //Le master fait une demande (pour une valeur)
        void requestEvent();

    private :

        //On parse les données reçues
        void parseDataFromMaster( byte* data);

};


#endif // ROMGERESLAVEBOARDCLASS_H_INCLUDED
#endif // ACTIVE_MULTI_ARDUINO_BOARD_MODE
