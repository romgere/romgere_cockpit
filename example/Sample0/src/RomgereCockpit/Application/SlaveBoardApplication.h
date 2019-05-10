/**
 * Romgere Cockpit Library - https://github.com/romgere/romgere_cockpit
 * @author jerome@mestres.fr
 */

#include "../Config/MainConfig.h"


#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE

#ifndef ROMGERESLAVEBOARDCLASS_H_INCLUDED

#define ROMGERESLAVEBOARDCLASS_H_INCLUDED

#include <Arduino.h>
#include <Wire.h>

//Queue library header (from http://playground.arduino.cc/Code/QueueArray)
#include "../Misc/QueueArray.h"

#include "../Misc/MasterToSlaveCommand.h"




class SlaveBoardApplication{

    private :

        uint8_t BoardI2CAdresse;

        MasterToSlaveCommand* lastPinForValueSend;    //Last "GetPINValue" command informations

        //Queue of received command from master. Allows keep commands received from I2C without doing any write/read
        //operations to free the I2C bus quickly.
        //Command are unqueue later by the main app loop.
        //
        //See http://playground.arduino.cc/Code/QueueArray
        QueueArray <MasterToSlaveCommand*> commandQueue;

    public :

        SlaveBoardApplication( uint8_t adresse );
        ~SlaveBoardApplication();

        //Init and register board on I2C bus
        void RegisterI2C();

        //Main slave board app loop :
        // - dequeue command from master board
        // - do action (init/read) on pin
        void loop();

        //Received data from master
        void receivedData( int dataSize);

        //Master board request a value
        void requestEvent();

    private :

        //Parse data from master Board
        void parseDataFromMaster( byte* data);

};


#endif // ROMGERESLAVEBOARDCLASS_H_INCLUDED
#endif // ACTIVE_MULTI_ARDUINO_BOARD_MODE
