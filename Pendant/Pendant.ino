#include <Adafruit_NeoPixel.h>
#define LED_PIN 0
#define SEED_PIN 2

#define BRIGHTNESS (uint8_t)0x10    //Starting brightness
#define COLOR (uint32_t)0xFF0000    //Starting color
#define MAX_OFFSET (uint8_t)0x80    //Maximum offset from the starting color for each color component.

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(16, LED_PIN);

void setup() {
  randomSeed(analogRead(SEED_PIN));
  pixels.begin();
  pixels.setBrightness(BRIGHTNESS);
  for (uint8_t i=0; i<pixels.numPixels(); i++) {
    pixels.setPixelColor(i, COLOR);
  }
}
 
void loop() {
  //for each LED,
  for (uint8_t i=0; i<pixels.numPixels(); i++) {
    uint32_t color = pixels.getPixelColor(i);
    // for each color component...
    for (uint8_t c=0; c<3; c++) {
      //get current component value
      uint8_t component = (color >> 8 * c) & 0xFF;
      uint8_t avgComp = (COLOR >> 8 * c) & 0xFF;
      //choose whether to add, subtract, or remain the same
      uint8_t offset = random(4);
      offset *= 5;
      uint8_t maxim = ((avgComp + MAX_OFFSET > avgComp) ? avgComp + MAX_OFFSET : 0xFF);
      uint8_t minim = ((avgComp - MAX_OFFSET < avgComp) ? avgComp - MAX_OFFSET : 0x00);
      switch (random(3)) {
        case 0: //add
          if ((component + offset > component) && (component + offset < maxim)) component += offset;
          break;
        case 1: //subtract
          if ((component - offset < component) && (component - offset > minim)) component -= offset;
          break;
        default:
          break;
      }
      //replace the color component
      //clear the bitfield
      color &= ~(uint32_t)0xFF << 8 * c;
      //set the bitfield
      color |= (uint32_t)component << 8 * c;
    }
    delay(50);
  }
  pixels.show();
}
