/**
 * Romgere Cockpit Library - https://github.com/romgere/romgere_cockpit
 * @author jerome@mestres.fr
 */

#include "ArduinoPotentiometerControl.h"

#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE
ArduinoPotentiometerControl::ArduinoPotentiometerControl( uint8_t pin, float step, float min, float max, int boardAddress) : ArduinoInputControl( AnalogControl, ITypePotentiometer, (boardAddress != -1), boardAddress ){
#else
ArduinoPotentiometerControl::ArduinoPotentiometerControl( uint8_t pin, float step, float min, float max) : ArduinoInputControl( AnalogControl, ITypePotentiometer){
#endif

    this->min = min;
    this->max = max;
    this->step = step;
    this->nbStep =  (int)((float)(this->max - this->min) / this->step) + 1;
    this->Pin1 = pin;
}

float ArduinoPotentiometerControl::ReadAndConvertInput() {
  float inValPerc = (float) _analogRead( this->Pin1) / 1024;
  return (int)(inValPerc * this->nbStep) * this->step;
}

bool ArduinoPotentiometerControl::ReadInput(){

  float newVal = ReadAndConvertInput();
  bool hasChange = this->currentValue != newVal;
  this->currentValue = newVal;
  return hasChange;
}

float ArduinoPotentiometerControl::getValue() {
  return this->currentValue ;
}
