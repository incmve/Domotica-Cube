
/*
 Replacement of my glowing egg http://www.maredana.nl/home-automation/woonkamer-cv-raspberry/
 Source:  https://github.com/incmve/RF-temperature-block
 3D printer model: http://www.thingiverse.com/thing:988027 
 */


//Includes
#include "LedControlMS.h"
#include <dht.h>
#include <NewRemoteReceiver.h>
#include <Adafruit_NeoPixel.h>

//Define pins
#define NBR_MTX 2 // number of 8x8 displays
#define DHT11_PIN 8 // DHT11 pin
#define PIN 6
#define NUMPIXELS 1
LedControl lc=LedControl(12,11,10, NBR_MTX); // DataIn,CLK,LOAD, number of displays
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Variables
int NIGHT = 0;      // 0 daytime, 1 Night mode
dht DHT;
uint32_t red = pixels.Color(255, 0, 0);
uint32_t blue = pixels.Color(0, 0, 255);
uint32_t black = pixels.Color(0, 0, 0);

// 1 = enable debug, 0 = disable debug
boolean debug = 0;

void setup() {
    NewRemoteReceiver::init(0, 2, cube); // RF receiver on pin 2
  Serial.begin (9600);
  Serial.println("Setup");
  lc.shutdown(0,false);// turn off power saving, enables display
  lc.shutdown(1,false);// turn off power saving, enables display
  lc.setIntensity(0,6);// sets brightness (0~15 possible values)
  lc.setIntensity(1,6);// sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen just in case
  lc.clearDisplay(1);// clear screen just in case
  pixels.begin();
  pixels.show(); // Initialize all pixels to 'off'
  }


void loop() { 
  
  if (NIGHT == 0) {
    lc.shutdown(0,false);
    lc.shutdown(1,false);
    int chk = DHT.read11(DHT11_PIN);
          if (debug)  {
                Serial.println(chk);
                      }
      switch (chk)
              {
                case DHTLIB_OK:
                float humfloat = DHT.temperature;
                int temperature = humfloat - 2;
                int   ones = (temperature%10); // extract ones from temperature
                int tens = ((temperature/10)%10); // extract tens from temperature
                  if (debug) {
                              Serial.print("Temp: ");
                              Serial.println(temperature);
                              Serial.print("Ones: ");
                              Serial.println(ones);
                              Serial.print("Tens: ");
                              Serial.println(tens);
                             }
                int temp1 = tens;
                int temp2 = ones;
                lc.displayChar(0 , temp1);
                lc.displayChar2(1 , temp2);
                delay(10000);
                break;
              }
  }
  if (debug) {
              Serial.print("Nightmode: ");
              Serial.println(NIGHT);
              }
}
void cube(NewRemoteCode receivedCode) {  
      if (debug) {
      // debug RF
     Serial.print("Addr ");
     Serial.print(receivedCode.address);
      if (receivedCode.groupBit) {
        Serial.print(" group");
      } else {
        Serial.print(" unit ");
        Serial.print(receivedCode.unit);
      }
      
      switch (receivedCode.switchType) {
        case 0:
          Serial.print(" off");
          break;
        case 1:
          Serial.print(" on");
          break;
        case 2:
          Serial.print(" dim level");
          Serial.print(receivedCode.dimLevel);
          break;
                  }
                }
            
  if (receivedCode.address == 66 && receivedCode.unit == 6 && receivedCode.switchType == 1) // Unit 66 ID 6 On signal
          {  
          NIGHT = 0;
               pixels.setPixelColor(0, black);
               pixels.show();
          for(int i = 0; i < 3; i++)
            {
         
            // fade in from min to max in increments of 1 points:
          for(int fadeValue = 0 ; fadeValue <= 255; fadeValue +=1) { 
            // sets the value (range from 0 to 255):
               pixels.setPixelColor(0, fadeValue,0,0);
               pixels.show();
            // wait for 500 milliseconds to see the dimming effect    
            delay(10);                            
              } 
               
          // fade out from max to min in increments of 1 points:
          for(int fadeValue = 255 ; fadeValue >= 15; fadeValue -=1) { 
            // sets the value (range from 0 to 255):
               pixels.setPixelColor(0, fadeValue,0,0);
               pixels.show();
            // wait for 500 milliseconds to see the dimming effect    
           delay(10);     
              }
            }  
          }
  
  if (receivedCode.address == 66 && receivedCode.unit == 6 && receivedCode.switchType == 0) // Unit 66 ID 6 Off signal
          {
        NIGHT = 0;
               pixels.setPixelColor(0, black);
               pixels.show();
        for(int i = 0; i < 3; i++)
          {
          // fade in from min to max in increments of 1 points:
        for(int fadeValue = 0 ; fadeValue <= 255; fadeValue +=1) { 
          // sets the value (range from 0 to 255):
               pixels.setPixelColor(0, 0,0,fadeValue);
               pixels.show();
          // wait for 500 milliseconds to see the dimming effect    
          delay(10);                            
        }
               pixels.setPixelColor(0, black);
               pixels.show(); 
      
        // fade out from max to min in increments of 1 points:
        for(int fadeValue = 255 ; fadeValue >= 15; fadeValue -=1) { 
          // sets the value (range from 0 to 255):
               pixels.setPixelColor(0, 0,0,fadeValue);
               pixels.show();
          // wait for 500 milliseconds to see the dimming effect    
          delay(10);     
            }
          }  
        }
  if (receivedCode.address == 66 && receivedCode.unit == 7 && receivedCode.switchType == 0) // Unit 66 ID 7 Off signal
          {
          pixels.setPixelColor(0, black);
          pixels.show(); // This sends the updated pixel color to the hardware in this case off.
          BlinkR(10);
        }
        
  if (receivedCode.address == 66 && receivedCode.unit == 7 && receivedCode.switchType == 1) // Unit 66 ID 7 On signal
          {
        pixels.setPixelColor(0, black);
        pixels.show(); // This sends the updated pixel color to the hardware in this case off.
        BlinkRB(10);
          }
        
  if (receivedCode.address == 66 && receivedCode.unit == 8 && receivedCode.switchType == 0) // Unit 66 ID 8 Off signal
        {
        pixels.setPixelColor(0, black);
        pixels.show(); // This sends the updated pixel color to the hardware
        pixels.setPixelColor(0, pixels.Color(255,20,147)); // Pink (n, red, green, blue)
        pixels.show(); // This sends the updated pixel color to the hardware
        }
        
  if (receivedCode.address == 66 && receivedCode.unit == 8 && receivedCode.switchType == 1) // Unit 66 ID 8 On signal
        {
        pixels.setPixelColor(0, black);
        pixels.show(); // This sends the updated pixel color to the hardware
        pixels.setPixelColor(0, pixels.Color(0,255,0)); // Green (n, red, green, blue)
        pixels.show(); // This sends the updated pixel color to the hardware
        }
        
  if (receivedCode.address == 66 && receivedCode.unit == 9 && receivedCode.switchType == 0) // Unit 66 ID 9 Off signal
        {
        pixels.setPixelColor(0, black);
        pixels.show(); // This sends the updated pixel color to the hardware in this case off.
        if (debug)  {
                    Serial.print("Nightmode: ");
                    Serial.print(NIGHT);
                    }
        lc.shutdown(0,true); //turn off matrix
        lc.shutdown(1,true);
        NIGHT = 1;
        }
        
  if (receivedCode.address == 66 && receivedCode.unit == 9 && receivedCode.switchType == 1) // Unit 66 ID 9 On signal
        {
        pixels.setPixelColor(0, black);
        pixels.show(); // This sends the updated pixel color to the hardware
        pixels.setPixelColor(0, pixels.Color(0,64,0)); // glow green (n, red, green, blue)
        pixels.show(); // This sends the updated pixel color to the hardware
        }


//Blink routine
}
void BlinkR(int times) //Unit 66 ID 7 On Blink
{
 for (int i=0; i< times; i++)
 {
  pixels.setPixelColor(0, black);
  pixels.show();
  pixels.setPixelColor(0, red);
  pixels.show();
  delay (100);
 }
}
void BlinkRB(int times) //Blink red/blue
{
 for (int i=0; i< times; i++)
 {
  pixels.setPixelColor(0, black);
  pixels.show();
  pixels.setPixelColor(0, red);
  pixels.show();
  delay (100);
  pixels.setPixelColor(0, black);
  pixels.show();
  pixels.setPixelColor(0, blue);
  pixels.show();
  delay (100);
  pixels.setPixelColor(0, black);
  pixels.show();
  pixels.setPixelColor(0, red);
  pixels.show();
  delay (100);
  pixels.setPixelColor(0, black);
  pixels.show();
  pixels.setPixelColor(0, blue);
  pixels.show();
  delay (100);
 }
}
