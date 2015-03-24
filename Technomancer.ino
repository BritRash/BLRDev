#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_Simple_AHRS.h>
#include <Adafruit_NeoPixel.h>

#define neoPixelPin 6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(12, neoPixelPin, NEO_GRB + NEO_KHZ800);

// Create sensor instances.
Adafruit_LSM303_Accel_Unified accel(30301);
Adafruit_LSM303_Mag_Unified   mag(30302);

// Create simple AHRS algorithm using the above sensors.
Adafruit_Simple_AHRS          ahrs(&accel, &mag);

float pos = 6;
long gesStart = 0;
long gesTime = 500;

bool spinCW = false;
bool spinCCW = false;
bool flash = false;
bool solid = false;
bool flashOn = false;

const float halfWidth = 1.25;

void setup(void)
{
  Serial.begin(115200);
  strip.begin();
  strip.show();
  
  accel.begin();
  mag.begin();
  resetModes();
}

void loop(void)
{
  sensors_vec_t orientation;
  ahrs.getOrientation(&orientation);
  
  float heading = orientation.heading;
  
  if (heading < 0)
  {
    heading = 360 + heading;
  }
  
  CheckForNods(orientation);
 
  if(spinCW)
  {
    pos +=1;
  }
  else if (spinCCW)
  {
    pos -=1;
  }
  
  while (round(pos) < 0) pos += 12.0;
  while (round(pos) > 11) pos -=12.0;
  
  uint32_t color = selectColor(heading, 250);
  
  if (solid)
  {
    for (int i = 0; i < 12; i++)
    {
      strip.setPixelColor(i, color);
    }
    delay(100);
    strip.show();
  }
  else if (flash)
  {
    for (int i=0; i < 12; i++)
    {
      if(flashOn)
      {
        strip.setPixelColor(i, color);
      }
      else
      {
        strip.setPixelColor(i, 0);
      }
    }
    flashOn = !flashOn;
    strip.show();
    delay(500);
    
  }
  else if (spinCW || spinCCW)
  {
    for (int i=0; i < 12; i++)
    {
      float diff = i - pos;
      
      if (fabs(diff) <= halfWidth)
      {
        strip.setPixelColor(i, color);
      }
      else
      {
        strip.setPixelColor (i, 0);
      }
    }
    strip.show();
    delay(100);
  }
  else //All off
  {
    for (int i=0; i < 12; i++)
    {
      strip.setPixelColor(i, 0);
      strip.show();
      delay(100);
    }
  }
}

uint32_t selectColor(float heading, float value)
{
  uint32_t color;
  
  if(heading < 60)
  {
    color = strip.Color(0, 0, value);
  }
  else if (heading < 120)
  {
    color = strip.Color(0, value, value);
  }
  else if (heading < 180)
  {
    color = strip.Color(0, value, 0);
  }
  else if (heading < 240)
  {
    color = strip.Color(value, value, 0);
  }
  else if (heading < 300)
  {
    color = strip.Color(value, 0, 0);
  }
  else
  {
    color = strip.Color(value, 0, value);
  }
}

void CheckForNods(sensors_vec_t orientation)
{
  float angle = orientation.roll;
  
  if (angle < 0)
  {
     angle = 360 + angle;
  }
  
  Serial.print("Angle = ");
  Serial.print(angle);
  Serial.print("orientation = ");
  Serial.println(orientation.roll);
  
  if(angle < 60) //all off... arm would be backwards
  {
    if(millis() - gesStart > gesTime)
    {
      flash = false;
      spinCW=false;
      spinCCW=false;
      solid=false;
      gesStart = millis();
    }
  }
  else if (angle < 120)
  {
    if(millis() - gesStart > gesTime)
    {
      flash = false;
      spinCW = false;
      spinCCW = false;
      solid = true;
      gesStart = millis();
    }
  }
  else if (angle < 150)
  {
    if(millis() - gesStart > gesTime)
    {
      flash = false;
      spinCW = true;
      spinCCW = false;
      solid = false;
      gesStart = millis();
    }
  }
  else if (angle < 210)
  {
    if (millis() - gesStart > gesTime)
    {
      flash = true;
      spinCW=false;
      spinCCW=false;
      solid=false;
      gesStart=millis();
    }
  }
  else if (angle < 240)
  {
    if (millis() - gesStart > gesTime)
    {
      flash = false;
      spinCW = false;
      spinCCW = true;
      solid = false;
      gesStart=millis();
    }
  }
  else
  {
    if (millis() - gesStart > gesTime)
    {
      flash = false;
      spinCW = false;
      spinCCW = false;
      solid = false;
      gesStart = millis();
    }
  }
}

void resetModes()
{
  flash = false;
  spinCW = false;
  spinCCW = false;
  solid = false;
}
