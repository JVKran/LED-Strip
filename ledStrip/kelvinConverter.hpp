#ifndef __KELVIN_CONVERTER_HPP
#define __KELVIN_CONVERTER_HPP

#include "Arduino.h"

class kelvinConverter{
	private:
		unsigned int _temperature;
		int _brightness;
		uint8_t calculateRed();
		uint8_t calculateGreen();
		uint8_t calculateBlue();
	public:
		kelvinConverter(const unsigned int temperature, const int brightness);
		unsigned int Temperature;
		int Brightness;
		uint8_t Red;
		uint8_t Green;
		uint8_t Blue;
		unsigned long Color;
};

#endif //__KELVIN_CONVERTER_HPP