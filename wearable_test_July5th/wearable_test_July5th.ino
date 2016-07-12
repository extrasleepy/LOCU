//Code for 72u wearable electroincs project. Summer 2016

/* TODO::
   DONE: White lights spin 2 times when all parameters are correct BUT ONLY ONCE PER SESSION
   ->What's the final color when all parameters are met??
   ->Make cool tones (JAVI working on this)
   ->Perfect environment tone should only play once per 4 hours? even if you temporarily exit perfect environment. If you don't leave perfect environment, it won't go off ever again.
   DONE: (needs to be uncommented) How often do we see the lights? once every 60 sec
   ->Go into deep sleep when no movement is detected over 30 min wake up on accelerometer movement (D3 D4 are interrupts)
   ->Touch "remember an idea" sequence  - - reminder after 2 hours.
   ->We need real numbers on the sensor min/max values (JAVI working on this)
   DONE: light should change to yellow, temp should be cyan, magenta sound
*/

#include <Adafruit_CircuitPlayground.h>   //required library to use module
//#include <Narcoleptic.h>  //library that allows low power sleeping

#define TEMP A0  //Analog 0 is connected to temperature sensor
#define SOUND A4  //Analog 4 is connected to sound sensor/microphone
#define LIGHT A5  //Analog 5 is connected to light sensor

#define TONE_DURATION_MS 100  // Duration in milliseconds to play a tone when touched.

float tMin = 510;   //min max variables for sensors   < - - - these are currently a little arbitrary
float tMax = 580;
float sMin = 300;
float sMax = 400;
float lMin = 900;
float lMax = 1100;

int pixels[] = {0, 2, 4, 5, 7, 9};   //array of used light pins
int tones[] = {100, 500, 1000};   //array for adding sound
int tlength[] = {200, 250, 200};  //array for adding sound durations

bool resetSpin = true;

void setup() {
  CircuitPlayground.begin();     // Setup Circuit Playground library.
  CircuitPlayground.strip.setBrightness(200); //brightness is between 0 and 255
  Serial.begin(9600);     // Setup serial port.
}

void loop() {
  // Get the sensor sensor values
  uint16_t tempValue = analogRead(TEMP);
  uint16_t soundValue = analogRead(SOUND);
  uint16_t lightValue = analogRead(LIGHT);
  //print sensor values to serial monitor  < --- comment out to save power
  Serial.print("raw temp= ");
  Serial.println(tempValue, DEC);
  Serial.print("raw sound= ");
  Serial.println(soundValue, DEC);
  Serial.print("raw light= ");
  Serial.println(lightValue, DEC);

  lightUp(tempValue, soundValue, lightValue);   //function to lights fades
  delay(5000);    // eventually 60 sec
  //Narcoleptic.delay(60000); // 60 second sleep between readings <----bring in later
}


uint16_t lightUp(uint16_t tempValue, uint16_t soundValue, uint16_t lightValue) {

  //play tone if environment is perfect   <----causing clicking??
  if (tempValue > tMin && tempValue < tMax && soundValue > sMin && soundValue < sMax && lightValue > lMin && lightValue < lMax && resetSpin==true) {
    for (int i = 0; i <= sizeof(tones-1); i++) {
      CircuitPlayground.playTone(tones[i], tlength[i]);
      delay(tlength[i]);
    }
    
    //spin twice
    for (int spin = 0; spin < 3; spin++) {   //spin happens twice
      for (int i = 0; i < 10; i++) {    //fade each of the 6 lights
        for (int fd = 0; fd <= 255; fd+=3) {
          CircuitPlayground.strip.setPixelColor(i, fd, fd, fd);   //white
          CircuitPlayground.strip.setPixelColor(9-i, fd, fd, fd);   //white
          CircuitPlayground.strip.show();  // update pixels!
          delayMicroseconds(1);
        }
        for (int fd = 255; fd >= 0; fd-=3) {
          CircuitPlayground.strip.setPixelColor(i, fd, fd, fd);   //white
          CircuitPlayground.strip.setPixelColor(9-i, fd, fd, fd);   //white
          CircuitPlayground.strip.show();  // update pixels!
          delayMicroseconds(1);
        }
      }
    }
    resetSpin=false;
  }

  //fade in loop
  for (int fd = 0; fd < 255; fd++) {
    if (tempValue > tMin && tempValue < tMax && soundValue > sMin && soundValue < sMax && lightValue > lMin && lightValue < lMax) {
      for (int i = 0; i < 7; i++) {
        CircuitPlayground.strip.setPixelColor(pixels[i], fd, fd, fd);   //white
        CircuitPlayground.strip.show();  // update pixels!
      }
    }
    else {
      resetSpin=true;
      
      if (tempValue > tMin && tempValue < tMax) {
        CircuitPlayground.strip.setPixelColor(5, 0, fd, fd);     //cyan (temp)
        CircuitPlayground.strip.setPixelColor(7, 0, fd, fd);     //cyan (temp)

      }
      if (soundValue > sMin && soundValue < sMax) {
        CircuitPlayground.strip.setPixelColor(0, fd, 0, fd);   //magenta (sound)
        CircuitPlayground.strip.setPixelColor(9, fd, 0, fd);   //magenta (sound)

      }
      if (lightValue > lMin && lightValue < lMax) {
        CircuitPlayground.strip.setPixelColor(2, fd, fd, 0);    //yellow (light)
        CircuitPlayground.strip.setPixelColor(4, fd, fd, 0);    //yellow (light)
      }
    }
    CircuitPlayground.strip.show();  // update pixels!
    delay(10);
  }

  //fade out loop
  for (int fd = 255; fd > 0; fd--) {
    if (tempValue > tMin && tempValue < tMax && soundValue > sMin && soundValue < sMax && lightValue > lMin && lightValue < lMax) {
      for (int i = 0; i < 7; i++) {
        CircuitPlayground.strip.setPixelColor(pixels[i], fd, fd, fd);   //white
        CircuitPlayground.strip.show();  // update pixels!
      }
    }
    else {
      if (tempValue > tMin && tempValue < tMax) {
        CircuitPlayground.strip.setPixelColor(5, 0, fd, fd);     //cyan (temp)
        CircuitPlayground.strip.setPixelColor(7, 0, fd, fd);     //cyan (temp)
      }
      if (soundValue > sMin && soundValue < sMax) {
        CircuitPlayground.strip.setPixelColor(0, fd, 0, fd);   //magenta (sound)
        CircuitPlayground.strip.setPixelColor(9, fd, 0, fd);   //magenta (sound)
      }
      if (lightValue > lMin && lightValue < lMax) {
        CircuitPlayground.strip.setPixelColor(2, fd, fd, 0);    //yellow (light)
        CircuitPlayground.strip.setPixelColor(4, fd, fd, 0);    //yellow (light)
      }
    }
    CircuitPlayground.strip.show();  // update pixels!
    delay(10);
  }
}

