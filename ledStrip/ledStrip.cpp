#include "ledStrip.hpp"
#include "kelvinConverter.hpp"

color::color(const uint16_t red, const uint16_t green, const uint16_t blue):
	red(red),
	green(green),
	blue(blue)
{}

uint16_t color::dominantColorValue(){
	if(red > green && red > blue){
		return red;
	} else if (green > red && green > blue){
		return green;
	}
	return blue;
}

void color::operator*=(const int & factor){
	red *= factor;
	green *= factor;
	blue *= factor;
}

ledStrip::ledStrip(const uint8_t redPin, const uint8_t greenPin, const uint8_t bluePin, mqttClient & client):
	redPin(redPin),
	greenPin(greenPin),
	bluePin(bluePin),
	client(client)
{
	pinMode(redPin, OUTPUT);
	pinMode(greenPin, OUTPUT);
	pinMode(bluePin, OUTPUT);
	setBrightness(0);
}

void ledStrip::updateAvailabiliy(const bool stateAvailable){
	if(stateAvailable){
		client.sendMessage("/woonkamer/ledstrip/availability", "online"); 
	} else {
		client.sendMessage("/woonkamer/ledstrip/availability", "offline");
	}
}

void ledStrip::setColor(const color & colorToEnable){
	analogWrite(redPin, colorToEnable.red);
	analogWrite(greenPin, colorToEnable.green);
	analogWrite(bluePin, colorToEnable.blue);
	if(colorToEnable.red != 0 && colorToEnable.green != 0 && colorToEnable.blue != 0){
		lastColor = colorToEnable;
	}
}

void ledStrip::setBrightness(const uint16_t brightness){
	desiredBrightness = brightness;
	differencePerSec = (desiredBrightness - currentBrightness);
	brightnessReceived = millis();
}

void ledStrip::fadeLights(){
	if(desiredBrightness != currentBrightness && millis() > lastFade + 10){
		lastFade = millis();
		if(currentBrightness + differencePerSec / 70 > desiredBrightness && differencePerSec > 0){
			currentBrightness = desiredBrightness;
		} else if(currentBrightness + differencePerSec / 70 < desiredBrightness && differencePerSec < 0){
			currentBrightness = desiredBrightness;
		} else {
			currentBrightness += differencePerSec / 70;
		}
		setColor(color(currentBrightness, currentBrightness, currentBrightness));
	}
}

void ledStrip::messageReceived(const String & receivedMessage, const char* topic){
	if(receivedMessage == "ON"){
		if(millis() > brightnessReceived + 1000){
			setBrightness(1023);
			client.sendMessage("/woonkamer/ledstrip/state", "ON");
		}
	} else if (receivedMessage == "OFF"){
		setBrightness(0);
		client.sendMessage("/woonkamer/ledstrip/state", "OFF");
	} else if (receivedMessage == "CONNECTED"){
		updateAvailabiliy(true);
	} else if(strcmp(topic, "/woonkamer/ledstrip/rgb/command") == 0){
		color newColor = color();
		int commaIndex = receivedMessage.indexOf(',');
		int secondCommaIndex = receivedMessage.indexOf(',', commaIndex + 1);
		newColor.red = receivedMessage.substring(0, commaIndex).toInt() * 4;
		newColor.green = receivedMessage.substring(commaIndex + 1, secondCommaIndex).toInt() * 4;
		newColor.blue = receivedMessage.substring(secondCommaIndex + 1).toInt() * 4;
		setColor(newColor);
	} else if(strcmp(topic, "/woonkamer/ledstrip/color/temp") == 0){
		int kelvinTemperature = 1000000 / receivedMessage.toInt();
		kelvinConverter kelvin(kelvinTemperature, currentBrightness);
		setColor(color(kelvin.Red * 4, kelvin.Green * 4, kelvin.Blue * 4));
	} else if(strcmp(topic, "/woonkamer/ledstrip/brightness") == 0){
		setBrightness(receivedMessage.toInt());
	}
}
