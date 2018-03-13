#include "kegnet/Display.h"
#include "kegnet/FlowMeter.h"
#include "kegnet/TempSensor.h"

#undef SSD1306_128_64
#define SSD1306_128_32

#define FLOW_PIN D4
#define TEMP_PIN A0
#define SDA_PIN D0
#define SCL_PIN D1
#define RST_PIN D2

Display *display;
FlowMeter *flowMeter;
TempSensor *tempSensor;

void setup()   {
  Serial.begin(9600);

  display = new Display(SDA_PIN, SCL_PIN, RST_PIN);
  flowMeter = new FlowMeter(FLOW_PIN, display);
  tempSensor = new TempSensor(TEMP_PIN, display);
}

void loop() {
  unsigned long currentMillis = millis();
  flowMeter->process(currentMillis);
  tempSensor->process(currentMillis);
}
