/**
 * Romgere Cockpit Library - https://github.com/romgere/romgere_cockpit
 * @author jerome@mestres.fr
 */

#ifndef XPLANEINPUTCLASS_H_INCLUDED
#define XPLANEINPUTCLASS_H_INCLUDED

#include <Arduino.h>

//Used to bind ouptut control to datas received from X-Plane (DREF)
class XPlaneInputData{

    private :

        byte DataGroup; //Group to use
        uint8_t DataIndex; //Index of data in group to use

    public :

        XPlaneInputData(byte group, uint8_t index = 0) : DataGroup(group), DataIndex(index){};
        ~XPlaneInputData(){};

        byte getGroup(){ return DataGroup; };
        uint8_t getIndex(){ return DataIndex; };
};

#endif
