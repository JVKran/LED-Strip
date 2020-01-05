#ifndef __LEDSTRIP_HPP
#define __LEDSTRIP_HPP

#include "mqttClient.hpp"

class color {
    public:
        uint16_t red;
        uint16_t green;
        uint16_t blue;

        color(const uint16_t red = 0, const uint16_t green = 0, const uint16_t blue = 0);
        uint16_t dominantColorValue();

        void operator*=(const int & factor);
};

class ledStrip : public messageListener {
	private:
		const uint8_t redPin;
		const uint8_t greenPin;
		const uint8_t bluePin;

		unsigned long brightnessReceived = 0;

		uint16_t currentBrightness = 0;
		uint16_t desiredBrightness = 0;
		unsigned long fadeDuration = 0;
		unsigned long fadePeriod = 0;
		unsigned long lastFade = 0;
		int16_t differencePerSec = 0;

		mqttClient & client;

		color lastColor = color();
	public:
		ledStrip(const uint8_t redPin, const uint8_t greenPin, const uint8_t bluePin, mqttClient & client);

		void setColor(const color & colorToEnable);
		void setBrightness(const uint16_t brightness);
		void fadeLights();

		void updateAvailabiliy(const bool stateAvailable);
		virtual void messageReceived(const String & receivedMessage, const char* topic = "") override;
};

#endif //__LEDSTRIP_HPP