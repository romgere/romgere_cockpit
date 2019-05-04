/* Romgere Cockpit Application - http://romgere-xplane-baron58-cockpit.mestres.fr/
 * Réalisation Jérôme Mestres
 * Utilisation, modification et distribution autorisé pour une utilisation personnel non commercial.
 * L'auteur se reserve le droit à une utilisation commerciale.
 * L'auteur refuse tout utilisation commercial de tout ou partie de la librairie, en l'état ou modifiée.
 * Contact : http://romgere-xplane-baron58-cockpit.mestres.fr/
 */

#ifndef ARDUINOCONTROLCLASS_H_INCLUDED
#define ARDUINOCONTROLCLASS_H_INCLUDED

#include <Arduino.h>

#include "../Config/MainConfig.h"

///////////////////////////////////////////////////////////
//Classe mere pour la gestion des contrôles d'entrée/sortie de la carte Arduino
//Classe Virtuelle, non implémentable
///////////////////////////////////////////////////////////
class ArduinoControl{

public :

    //Type de contrôle
    typedef enum{
        AnalogControl = 0,  //Entrée/sortie analogique
        DigitalControl      //Entrée/sortie digital
    }ArduinoControlType;


protected :

    //Direction de contrôle
    typedef enum{
        InputControl = 0,  //Entrée (switch, encoder, ...)
        OutputControl      //Sortie (LED, Servo, ...)
    }ArduinoControlDir;

private :

    ArduinoControlDir   ControlDir;     //Direction du contrôle
    ArduinoControlType  ControlType;    //Type de contrôle


public :

#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE
    ArduinoControl( ArduinoControlDir dir, ArduinoControlType type, bool onSlaveBoard, uint8_t boardAdresse) : ControlDir(dir), ControlType( type), ControleOnSlaveBoard( onSlaveBoard), SlaveBoardAddress(boardAdresse){};
#else
    ArduinoControl( ArduinoControlDir dir, ArduinoControlType type) : ControlDir(dir), ControlType( type){};
#endif

    virtual ~ArduinoControl(){};

	ArduinoControlType getType(){ return ControlType; };

    //Lis l'état u contrôle et renvoi true si le contrôle necessite l'envoi d'une commande (input)
    virtual bool ReadInput() = 0;

    //Met à jour le controle en fonction de la valeur (output)
    virtual void WriteOutput() = 0;

    //Retourne des valeurs issuent de l'état du contrôle (input)
    //val : pointeur vers un tableau de valeur
    //retour : Nombre d'élement du tableu de valeur retourné
	virtual float getValue(){ return 0; };

	//Définit une valeur pour le contrôle (output) (retourne TRUE si la valeur a été modifiée, FALSE sinon)
	virtual bool setValue(float val){ return false; };

#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE

private :

    bool    ControleOnSlaveBoard;   //contrôle est-il situé sur une carte esclave I2C
    uint8_t SlaveBoardAddress;      //Adresse de l'esclave sur le I2C

public :
    //Permet de savoir si le contrôle est situé sur une carte esclave
    bool isOnSlaveBoard(){ return ControleOnSlaveBoard; };

    //Ici on va définir les méthode permettant de lire/ecrire les entrée Arduino
    //Permet de s'affranchir dans les classes de controles dérivée de la façon dont ces actions sont faites
    //(Par exemple pour une carte Esclave en I2C, cette méthode ne va pas appelée directement les fonction Arduino classique analogRead, analogWrite...)
    void    _pinMode(uint8_t p, uint8_t m);
    void    _digitalWrite(uint8_t p, uint8_t v);
    int     _digitalRead(uint8_t p);
    int     _analogRead(uint8_t p);
    void    _analogWrite(uint8_t p, int v);
#endif

};

#ifndef ACTIVE_MULTI_ARDUINO_BOARD_MODE
    #define _pinMode( a, b) pinMode( a, b)
    #define _digitalWrite( a, b) digitalWrite( a, b)
    #define _digitalRead( a) digitalRead( a)
    #define _analogRead( a) analogRead( a)
    #define _analogWrite( a, b) analogWrite( a, b)
#endif



/*******************************/
/* CLASS DE BASE INPUT CONTROL */
/*******************************/

class ArduinoInputControl : public ArduinoControl{


    public :

        //Type de contrôle input
        typedef enum{
            ITypePushButton = 0,
            ITypeToggleSwitch,
            ITypeRotarySwitch,
            ITypeRotaryEncode,
        }ArduinoInputControlType;

    private :

        ArduinoInputControlType inputType;

    public :

#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE
        ArduinoInputControl( ArduinoControlType type, ArduinoInputControlType iType, bool onSlaveBoard, uint8_t boardAdresse ) : ArduinoControl( InputControl, type, onSlaveBoard, boardAdresse), inputType( iType){};
#else
        ArduinoInputControl( ArduinoControlType type, ArduinoInputControlType iType ) : ArduinoControl( InputControl, type), inputType( iType){};
#endif
        ~ArduinoInputControl(){};
        void WriteOutput(){};

        ArduinoInputControlType getInputType(){ return this->inputType; }
};


/********************************/
/* CLASS DE BASE OUTPUT CONTROL */
/********************************/

class ArduinoOutputControl : public ArduinoControl{

    public :

        //Type de contrôle input
        typedef enum{
            OTypeLed = 0,
            OTypeServo,
        }ArduinoOutputControlType;

    private :

        ArduinoOutputControlType outputType;

    public :

#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE
        ArduinoOutputControl( ArduinoControlType type, ArduinoOutputControlType oType, bool onSlaveBoard, uint8_t boardAdresse ) : ArduinoControl( OutputControl, type, onSlaveBoard, boardAdresse), outputType(oType){};
#else
        ArduinoOutputControl( ArduinoControlType type, ArduinoOutputControlType oType) : ArduinoControl( OutputControl, type), outputType(oType){};
#endif
        ~ArduinoOutputControl(){};
        bool ReadInput(){return false;};

        ArduinoOutputControlType getOutputType(){ return this->outputType; }
};

#endif // ARDUINOCONTROLCLASS_H_INCLUDED
