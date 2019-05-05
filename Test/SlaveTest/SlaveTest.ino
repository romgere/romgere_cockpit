#include <Arduino.h>
#include <Wire.h>

#include "src/RomgereCockpit/Application/SlaveBoardApplication.h"
SlaveBoardApplication* slaveCockpit;

void setup()
{
	slaveCockpit = new SlaveBoardApplication(1);
	slaveCockpit->RegisterI2C();
}

void loop()
{
	delay(100);
	slaveCockpit->loop();
}
