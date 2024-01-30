#include <Arduino.h>
#include <Adafruit_NeoPixel.h>


const uint8_t numLED = 12;
#define PIXEL_DATAPIN 16

Adafruit_NeoPixel strip = Adafruit_NeoPixel(numLED, PIXEL_DATAPIN, NEO_GRB + NEO_KHZ800);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Starting setup");
  strip.begin();
  Serial.println("Initialized Strip");
  strip.setBrightness(255);
  strip.setPixelColor(0, strip.Color(255,0,0));
  Serial.println("Committing");
  strip.show();
  Serial.println("Successfully finished setup");


}

uint8_t r=0;
uint8_t c=0;

void loop() {
  // put your main code here, to run repeatedly:
  
  strip.setBrightness(255);
  uint32_t col = 0;
  switch (c)
  {
  case 0:
    col = strip.Color(r,0,0);
    break;
  case 1:
    col = strip.Color(0,r,0);
    break;
  case 2:
    col = strip.Color(0,0,r);
    break;
  default:
    break;
  }
  
  for(int i=0;i<numLED;i++) {
      strip.setPixelColor(i, col);
  }
  strip.show();
  r = (r+1) % 256;
  if (r==0) 
    c = (c+1) % 3;
  delay(50);
}

// put function definitions here:
