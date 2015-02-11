#include <Adafruit_NeoPixel.h>
// Pin definitions
#define LED_PIN 0  //output LED pin.
#define SEED_PIN 2 //Leave this pin unconnected.  The ambient noise will be sufficient for a random seed.

#define BRIGHTNESS (uint8_t)0x10    //Starting brightness
#define COLOR (uint32_t)0xFF0000    //Base color
#define MAX_OFFSET (uint8_t)0x80    //Maximum offset from the starting color for each color component.
#define MAX_SHIFT  (uint8_t)0x04    //Maximum amount of the color shift. The higher this is, the more drastic the shift.

Adafruit_NeoPixel ring = Adafruit_NeoPixel(16, LED_PIN);

void setup() {
  randomSeed(analogRead(SEED_PIN));
  ring.begin();
  ring.setBrightness(BRIGHTNESS);
  for (uint8_t i=0; i<ring.numPixels(); i++) {
    ring.setPixelColor(i, COLOR);
  }
}

void loop() {
  //for each LED,
  for (uint8_t i=0; i<ring.numPixels(); i++) {
    uint32_t curColor = ring.getPixelColor(i);
    // for each color component...
    for (uint8_t c=0; c<3; c++) {
      //get current component value
      uint8_t component = (curColor >> 8 * c) & 0xFF;
      uint8_t baseComp = (COLOR >> 8 * c) & 0xFF;
      //decide how much to shift
      uint8_t offset = random(MAX_SHIFT);
      uint8_t maxim = ((baseComp + MAX_OFFSET > baseComp) ? baseComp + MAX_OFFSET : 0xFF);
      uint8_t minim = ((baseComp - MAX_OFFSET < baseComp) ? baseComp - MAX_OFFSET : 0x00);
      //choose whether to add, subtract, or remain the same
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
      curColor &= ~(uint32_t)0xFF << 8 * c;
      //set the bitfield
      curColor |= (uint32_t)component << 8 * c;
    }
    ring.setPixelColor(i, curColor);
  }
  delay(50);
  ring.show();
}
