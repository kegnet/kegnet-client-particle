#ifndef FLOWMETER_H_
#define FLOWMETER_H_

#include "Display.h"

class FlowMeter {

	public:
		FlowMeter(int pin, Display* display);
    void process(unsigned long currentMillis);
		void pulse();

	private:
		int pin;
		Display* display;

		boolean isOz = true;
		unsigned long pourDelay = 1500;
		int minPourPulses = 250;
		unsigned long pollDelay = 50;
		float ppl = 5600;

		volatile int interruptCount = 0;

		unsigned long st = 0;
		unsigned long pulseCount = 0;
		unsigned long lastPour = 0;
		unsigned long lastPulse = 0;

		void publishPour(int pulses, unsigned long et);

		float toL(float pulses);
		float toOz(float pulses);
};

#endif
