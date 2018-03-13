#ifndef TEMPSENSOR_H_
#define TEMPSENSOR_H_

#include "photon-thermistor.h"
#include "Display.h"

class TempSensor {
	public:
		TempSensor(int pin, Display* display);
    void process(unsigned long currentMillis);

	private:
		void sampleTemp();
		boolean isValid(float newTemp);
		void calcTemp();
		void displayTemp();
		void reportTemp();
		float toF(float tempC);

		int pin;
		Display* display;
		Thermistor* thermistor;

		bool useF = true;
		float MIN_TEMP = -55.0;
		float MAX_TEMP = 125.0;
		float MAX_CHANGE = 25.0;

		float lastTemp = 0;

		unsigned long reportFreq = 5000;
		unsigned long lastReport = 0;

		unsigned long displayFreq = 1000;
		unsigned long lastDisplay = 0;

		int samplesPerTemp = 5;
		int sampleCount = 0;
		float sumSamples = 0;
		unsigned long sampleFreq = 100;
		unsigned long lastSample = 0;

		//unsigned long lastTempTs = 0;
};

#endif
