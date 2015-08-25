//We always have to include the library
#include "LedControlMS.h"
#include <dht.h>
#include <NewRemoteReceiver.h>

/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 We have only a single MAX72XX.
 */
#define NBR_MTX 2 
#define DHT11_PIN 8
LedControl lc=LedControl(12,11,10, NBR_MTX);
dht DHT;
int ledPinR = 5;    // Red LED connected to digital pin 5
int ledPinB = 9;    // Blue LED connected to digital pin 9
int ledPinG = 6;    // Green LED connected to digital pin 6
int NIGHT = 0;

/* we always wait a bit between updates of the display */
//unsigned long delaytime=300;


void setup() {
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
   NewRemoteReceiver::init(0, 2, egg);
  Serial.begin (9600);
  Serial.println("Setup");
  lc.shutdown(0,false);// turn off power saving, enables display
  lc.setIntensity(0,8);// sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen

  lc.shutdown(1,false);// turn off power saving, enables display
  lc.setIntensity(1,8);// sets brightness (0~15 possible values)
  lc.clearDisplay(1);// clear screen
  }




void loop() { 
  
  if (NIGHT == 0) {
    lc.shutdown(0,false);
    lc.shutdown(1,false);
  int chk = DHT.read11(DHT11_PIN);
    Serial.println(chk);
    
    switch (chk)
    {
      case DHTLIB_OK:
      float humfloat = DHT.temperature;
      int temperature = humfloat;
        int   ones = (temperature%10);
 int tens = ((temperature/10)%10);
      Serial.println(temperature);
      Serial.println(ones);
      Serial.println(tens);
    //  delay(10000); // wait for 10 seconds to go to next sensor
 int temp1 = tens;
 int temp2 = ones;
  lc.displayChar(0 , temp1);
  lc.displayChar(1 , temp2);
  delay(10000);
  // lc.clearAll();
      break;
    }
  }
  Serial.println(NIGHT);
}
void egg(NewRemoteCode receivedCode) {  // Note: interrupts are disabled. You can re-enable them if needed.
  
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
  
    if (receivedCode.address == 66 && receivedCode.unit == 6 && receivedCode.switchType == 1) // Unit 66 ID 6 On signal
  {  
  NIGHT = 0;
  analogWrite(ledPinR, 0); 
  analogWrite(ledPinG, 0);
  analogWrite(ledPinB, 0);
  for(int i = 0; i < 3; i++)
    {
  delay(500);
  Serial.print("Off"); //print to serial
       // fade in from min to max in increments of 5 points:
  for(int fadeValue = 0 ; fadeValue <= 255; fadeValue +=1) { 
    // sets the value (range from 0 to 255):
    analogWrite(ledPinR, fadeValue);         
    // wait for 500 milliseconds to see the dimming effect    
    delay(500);                            
      } 

  // fade out from max to min in increments of 5 points:
  for(int fadeValue = 255 ; fadeValue >= 15; fadeValue -=1) { 
    // sets the value (range from 0 to 255):
    analogWrite(ledPinR, fadeValue);         
    // wait for 500 milliseconds to see the dimming effect    
    delay(500);     
      }
    }  
  }
  if (receivedCode.address == 66 && receivedCode.unit == 6 && receivedCode.switchType == 0) // Unit 66 ID 6 Off signal
  {
  NIGHT = 0;
  analogWrite(ledPinR, 0); 
  analogWrite(ledPinG, 0);
  analogWrite(ledPinB, 0);
  for(int i = 0; i < 3; i++)
    {
  delay(500);
  Serial.print("On"); //print to serial
       // fade in from min to max in increments of 5 points:
  for(int fadeValue = 0 ; fadeValue <= 255; fadeValue +=1) { 
    // sets the value (range from 0 to 255):
    analogWrite(ledPinB, fadeValue);         
    // wait for 500 milliseconds to see the dimming effect    
    delay(500);                            
  } 

  // fade out from max to min in increments of 5 points:
  for(int fadeValue = 255 ; fadeValue >= 15; fadeValue -=1) { 
    // sets the value (range from 0 to 255):
    analogWrite(ledPinB, fadeValue);         
    // wait for 500 milliseconds to see the dimming effect    
    delay(500);     
      }
    }  
  }
  if (receivedCode.address == 66 && receivedCode.unit == 7 && receivedCode.switchType == 0) // Unit 66 ID 7 Off signal
    {
  analogWrite(ledPinR, 0); 
  analogWrite(ledPinG, 0);
  analogWrite(ledPinB, 0);
    {
  BlinkR(ledPinR,10);
  delay(50);
    }
  }
  if (receivedCode.address == 66 && receivedCode.unit == 7 && receivedCode.switchType == 1) // Unit 66 ID 7 On signal
    {
  analogWrite(ledPinR, 0); 
  analogWrite(ledPinG, 0);
  analogWrite(ledPinB, 0);
    {
  BlinkRB(ledPinR,10);
  delay(50);
    }
  }
  if (receivedCode.address == 66 && receivedCode.unit == 8 && receivedCode.switchType == 0) // Unit 66 ID 8 Off signal
  {
    analogWrite(ledPinR, 0); 
  analogWrite(ledPinG, 0);
  analogWrite(ledPinB, 0);
  // Glow Pink
  analogWrite(ledPinR, 255); 
  analogWrite(ledPinG, 51);
  analogWrite(ledPinB, 153); 
  }
        if (receivedCode.address == 66 && receivedCode.unit == 8 && receivedCode.switchType == 1) // Unit 66 ID 8 On signal
  {
    analogWrite(ledPinR, 0); 
  analogWrite(ledPinG, 0);
  analogWrite(ledPinB, 0);
  // Glow Green
  analogWrite(ledPinG, 255);
  }
        if (receivedCode.address == 66 && receivedCode.unit == 9 && receivedCode.switchType == 0) // Unit 66 ID 9 Off signal
  {
    analogWrite(ledPinR, 0); 
  analogWrite(ledPinG, 0);
  analogWrite(ledPinB, 0);
  NIGHT = 1;
  Serial.println(NIGHT);
  lc.shutdown(0,true);
  lc.shutdown(1,true);
  }
        if (receivedCode.address == 66 && receivedCode.unit == 9 && receivedCode.switchType == 1) // Unit 66 ID 9 On signal
  {
    analogWrite(ledPinR, 0); 
  analogWrite(ledPinG,40);
  analogWrite(ledPinB, 0);
  }


//Blink routine
}
void BlinkR(int led, int times) //Unit 66 ID 7 On Blink
{
 for (int i=0; i< times; i++)
 {
  digitalWrite(ledPinR,HIGH);
  delay (10000);
  digitalWrite(ledPinR,LOW);
  delay (10000);
 }
}
void BlinkRB(int led, int times) //Unit 66 ID 7 Off Blink
{
 for (int i=0; i< times; i++)
 {
  digitalWrite(ledPinB,HIGH);
  delay (10000);
  digitalWrite(ledPinR,HIGH);
  delay (10000);
  digitalWrite(ledPinB,LOW);
  delay (10000);
  digitalWrite(ledPinR,LOW);
  delay (10000);
 }
}
