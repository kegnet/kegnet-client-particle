#include "TempSensor.h"

TempSensor::TempSensor(int pin, Display* display) {
	this->pin = pin;
	this->display = display;
  this->thermistor = new Thermistor(pin, 10000, 4095, 10000, 25, 3950, 1, 0);
}

void TempSensor::process(unsigned long currentMillis) {
	if (currentMillis - lastSample > sampleFreq) {
    sampleTemp();
		lastSample = currentMillis;
  }

	if (sampleCount == samplesPerTemp) {
		calcTemp();

		if (currentMillis - lastDisplay > displayFreq) {
	    displayTemp();
			lastDisplay = currentMillis;
	  }

		if (currentMillis - lastReport > reportFreq) {
	    reportTemp();
			lastReport = currentMillis;
	  }
	}
}

void TempSensor::sampleTemp() {
	float temp = thermistor->readTempC();
	if (!isValid(temp)) {
		Serial.printf("WARN: Bad temp: %s\n", temp);
		return;
	}
	sumSamples += temp;
	sampleCount++;
}

boolean TempSensor::isValid(float newTemp) {
	if (newTemp < MIN_TEMP || newTemp > MAX_TEMP)
    return false;

	if (lastTemp == 0)
		return true;

  float percentChange = abs(((newTemp - lastTemp)/newTemp)*100);
  if (percentChange > MAX_CHANGE)
    return false;

	return true;
}

void TempSensor::calcTemp() {
	lastTemp = (sumSamples/(float)sampleCount);;
	sumSamples = 0;
	sampleCount = 0;
}

void TempSensor::displayTemp() {
	display->showTemp(useF ? toF(lastTemp) : lastTemp, useF);
}

void TempSensor::reportTemp() {
	if (useF)
		Particle.publish(String("tempF"), String(toF(lastTemp)));
	else
		Particle.publish(String("tempC"), String(lastTemp));
}

float TempSensor::toF(float tempC) {
	return (tempC * 1.8) + 32;
}
