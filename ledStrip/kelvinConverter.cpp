#include "Arduino.h"
#include "kelvinConverter.hpp"

kelvinConverter::kelvinConverter(const unsigned int temperature, const int brightness)
{
	Temperature = constrain(temperature, 0, 65500);
	Brightness = constrain(brightness, 0, 100);
	Red = map(Brightness, 0, 100, 0, calculateRed());
	Green = map(Brightness, 0, 100, 0, calculateGreen());
	Blue = map(Brightness, 0, 100, 0, calculateBlue());
	Color = Red;
	Color = Color<<8;
	Color = Color|Green;
	Color = Color<<8;
	Color = Color|Blue;
}

uint8_t kelvinConverter::calculateRed(){
	float red = 255;
	int temperature = Temperature/100;
	if(temperature>66) {
		red = temperature - 60;
	    red = 329.698727466 * pow(red,-0.1332047592);
	}
	return (uint8_t)constrain(red, 0, 255);
}

uint8_t kelvinConverter::calculateGreen(){
	float green;
	int temperature = Temperature/100;
	if(temperature<= 66){
		green = temperature;
		green = (99.4708025861 * log(green)) - 161.1195681661;
	} else {
		green = temperature - 60;
		green = 288.1221695283 * pow(green, -0.0755148492);
	}
	return (uint8_t)constrain(green, 0, 255);
}

uint8_t kelvinConverter::calculateBlue(){
  float blue = 255;
  int temperature = Temperature/100;
  if(temperature<65){
    if(temperature <= 19){
      blue = 0;
    } else {
      blue = temperature - 10;
      blue = (138.5177312231 * log(blue)) - 305.0447927307;
    }
  }
  return (uint8_t)constrain(blue, 0, 255);
}