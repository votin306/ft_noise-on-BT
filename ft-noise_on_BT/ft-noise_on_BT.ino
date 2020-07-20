#include <FastLED.h>

#define LED_PIN     3
#define BRIGHTNESS  96
#define Width 16
#define Height 16
boolean MatrixSerpentineLayout = true;

#define NUM_LEDS (Width * Height)
#define MAX_DIMENSION ((Width>Height) ? Width : Height)

CRGB leds[Width * Height];

int val_BT; 

uint16_t x;
uint16_t y;
uint16_t z;

uint16_t speed = 20;
uint16_t scale = 30;

uint8_t noise[MAX_DIMENSION][MAX_DIMENSION];
CRGBPalette16 currentPalette( PartyColors_p );
uint8_t       colorLoop = 1;

void setup() {
  Serial.begin(9600);
  LEDS.addLeds<WS2811, LED_PIN, GRB>(leds, NUM_LEDS);
  LEDS.setBrightness(BRIGHTNESS);
  x = random16();
  y = random16();
  z = random16();
}


void loop() {
  Change_effect();
  draw_pix();
  generator_palette();
  LEDS.show();
}
