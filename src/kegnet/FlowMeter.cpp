#include "FlowMeter.h"

FlowMeter::FlowMeter(int pin, Display* display) {
	this->pin = pin;
	this->display = display;

  pinMode(pin, INPUT);

	// this cool magic from https://community.particle.io/t/solved-how-to-register-a-c-instance-method-with-spark-function/13999/24
	auto interruptHandler = std::bind(&FlowMeter::pulse, this);
	attachInterrupt(pin, interruptHandler, RISING);
}

void FlowMeter::pulse() {
    interruptCount++;
}

void FlowMeter::process(unsigned long currentMillis) {
	if (interruptCount == 0)
		return;

	if (st == 0)
		st = currentMillis;

	if (interruptCount > pulseCount) {
		lastPulse = currentMillis;
		pulseCount = interruptCount;
		display->showPouring(isOz ? toOz(pulseCount) : toL(pulseCount), isOz);
	} else if (currentMillis - lastPulse > pourDelay) {
		interruptCount = 0;
		unsigned long et = (currentMillis - st);
		if (pulseCount < minPourPulses) {
			display->showNoPour();
		} else {
			lastPour = currentMillis;
			display->showPour(isOz ? toOz(pulseCount) : toL(pulseCount), isOz);
		 	publishPour(pulseCount, et);
		}
		pulseCount = 0;
		st = 0;
		lastPulse = 0;
	}
}

void FlowMeter::publishPour(int pulseCount, unsigned long et) {
	String msg = String();
	msg.concat("{\"pin\":");
	msg.concat(pin);
	msg.concat(",\"pulses\":");
	msg.concat(pulseCount);
	msg.concat(",\"ms\":");
	msg.concat(et);
	msg.concat(",\"ts\":");
	msg.concat(Time.now());
	msg.concat("}");

  Serial.println(msg);
	Particle.publish("pour", msg);
}

float FlowMeter::toL(float pulses) {
	return pulses/ppl;
}

float FlowMeter::toOz(float pulses) {
	return pulses / (ppl/33.814);
}
