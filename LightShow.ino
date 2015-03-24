#include <SPI.h>

//Add the SdFat Libraries
#include <SdFat.h>
#include <SdFatUtil.h>

//and the MP3 Shield Library
#include <SFEMP3Shield.h>
#include <fix_fft.h>


/**
 * \brief Object instancing the SdFat library.
 *
 * principal object for handling all SdCard functions.
 */
SdFat sd;

/**
 * \brief Object instancing the SFEMP3Shield library.
 *
 * principal object for handling all the attributes, members and functions for the library.
 */
SFEMP3Shield MP3player;

char im[128], data[128];

char data_avgs[5];

int i=0,val;

int ledPins[] = {A1, A2, A3, A4, A5};
void setup() {
  pinMode(A0, INPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);
 
  
  uint8_t result; //result code from some function as to be tested at later time.
  //Initialize the SdCard.
  if(!sd.begin(SD_SEL, SPI_FULL_SPEED)) sd.initErrorHalt();
  // depending upon your SdCard environment, SPI_HAVE_SPEED may work better.
  if(!sd.chdir("/")) sd.errorHalt("sd.chdir");

  //Initialize the MP3 Player Shield
  result = MP3player.begin();;
  MP3player.setVolume(10,10);
}

void loop(){
  for(int i=0; i < 255; i++)
  {
    if(!MP3player.playTrack(i))
    {
      while (MP3player.isPlaying())
      {
        processLights();
        
      }
      MP3player.stopTrack();
    }
  }
}

void processLights()
{
          for (i=0; i < 128; i++)
	{
          val = analogRead(A0);
          data[i] = val;
          im[i] = 0;
        };

        fix_fft(data,im,7,0);

        for (i=0; i< 64;i++){
          data[i] = sqrt(data[i] * data[i] + im[i] * im[i]); // this gets the absolute value of the values in the array, so we're only dealing with positive numbers
        };
  
        int bass = 0;
        for (int i=2; i < 7; i++)
        {
          bass += data[i];
        }
        bass = bass/2;
  
        int low = 0;
        for (int i=7; i < 20; i++)
        {
          low += data[i];
        }
        low = low/2;
  
        int mid = 0;
        for( int i=20; i < 35; i++)
        {
          mid += data[i];
        }
        mid = mid/2;
  
        int high = 0;
        for(int i=35; i < 50; i++)
        {
          high += data[i];
        }
        high = high/2;
  
        int treb = 0;
        for(int i=50; i < 64; i++)
        {
          treb += data[i];
        }
        treb = treb/2;

        if (bass > 40)
        {
           digitalWrite(ledPins[0], HIGH);
        }
        else
        {
          digitalWrite(ledPins[0], LOW);
        }

        if (low > 40)
        {
           digitalWrite(ledPins[1], HIGH);
        }
        else
        {
          digitalWrite(ledPins[1], LOW);
        }
  
        if (mid > 40)
        {
           digitalWrite(ledPins[2], HIGH);
        }
        else
        {
          digitalWrite(ledPins[2], LOW);
        }
        if (high > 40)
        {
           digitalWrite(ledPins[3], HIGH);
        }
        else
        {
          digitalWrite(ledPins[3], LOW);
        }
        if (treb > 40)
        {
           digitalWrite(ledPins[4], HIGH);
        }
        else
        {
          digitalWrite(ledPins[4], LOW);
        } 

}
