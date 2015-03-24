#include <Adafruit_NeoPixel.h>

#define BUTTON 2
#define VIBE 0
#define NEOPIXEL 1

int startThreshold = 500;
int lowThreshold = 1000;
int midThreshold = 1500;
int highThreshold = 2000;
int activity = 0;
int sensorState = 0;
int lastSensorState = 0;

uint32_t color;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, NEOPIXEL, NEO_GRB + NEO_KHZ800);

void setup()
{
	pinMode(BUTTON, INPUT);
	pinMode(VIBE, INPUT);
	pinMode(NEOPIXEL, OUTPUT);
	digitalWrite(BUTTON, HIGH);
	digitalWrite(VIBE, HIGH);
	
	strip.begin();
	strip.show();
	
}

void loop()
{
	sensorState = digitalRead(VIBE);
	
	if(sensorState != lastSensorState){
		if(sensorState == LOW){
			activity++;
		}
		
	}
	lastSensorState = sensorState;
	
	if(activity > highThreshold)
	{
		color = strip.Color(0, 255, 0);
	}
	else if(activity > midThreshold)
	{
		color = strip.Color(255, 255, 0);
	}
	else if(activity > lowThreshold)
	{
		color = strip.Color(255, 0, 0);
	}
	else if(activity > startThreshold)
	{
		color = strip.Color(255, 255, 255);
	}
	else
	{
		color = strip.Color(0, 0, 0);
	}
	
	if(!digitalRead(BUTTON))
	{
		colorWipe(color, 0);
	}
	else
	{
		colorWipe(strip.Color(0, 0, 0), 0);
	}
	
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}
