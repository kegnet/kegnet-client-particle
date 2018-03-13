#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "SPI.h"
//#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"

#define I2C_ADDRESS 0x3C

class Display {
	public:
		Display(int sda, int scl, int rst);
		void showPouring(float p, boolean isOz);
		void showNoPour();
		void showPour(float p, boolean isOz);
		void showTemp(float t, bool isF);
    void showStatus(String s);
    void showStatus(String s1, String s2);
    void clear();

	private:
		Adafruit_SSD1306 *display;

		int pourMessageDelay = 6000;
		int noPourMessageDelay = 1000;

		unsigned long lastMessageTs = 0;
		int lastMessageDelay = 0;
		boolean pouring = false;

		void writeCenter(int textSize, String s);
		void writeCenter(int textSize, String s1, String s2);
};

#endif
