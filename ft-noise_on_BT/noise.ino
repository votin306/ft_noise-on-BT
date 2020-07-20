void draw_pix() {
  // If we're runing at a low "speed", some 8-bit artifacts become visible
  // from frame-to-frame.  In order to reduce this, we can do some fast data-smoothing.
  // The amount of data smoothing we're doing depends on "speed".
  uint8_t dataSmoothing = 0;
  if ( speed < 50) {
    dataSmoothing = 200 - (speed * 4);
  }

  for (int i = 0; i < MAX_DIMENSION; i++) {
    int ioffset = scale * i;
    for (int j = 0; j < MAX_DIMENSION; j++) {
      int joffset = scale * j;

      uint8_t data = inoise8(x + ioffset, y + joffset, z);

      // The range of the inoise8 function is roughly 16-238.
      // These two operations expand those values out to roughly 0..255
      // You can comment them out if you want the raw noise data.
      data = qsub8(data, 16);
      data = qadd8(data, scale8(data, 39));

      if ( dataSmoothing ) {
        uint8_t olddata = noise[i][j];
        uint8_t newdata = scale8( olddata, dataSmoothing) + scale8( data, 256 - dataSmoothing);
        data = newdata;
      }

      noise[i][j] = data;
    }
  }

  z += speed;
  x += speed / 8;
  y -= speed / 16;
}

void generator_palette()
{
  static uint8_t ihue = 0;

  for (int i = 0; i < Width; i++) {
    for (int j = 0; j < Height; j++) {
      // We use the value at the (i,j) coordinate in the noise
      // array for our brightness, and the flipped value from (j,i)
      // for our pixel's index into the color palette.

      uint8_t index = noise[j][i];
      uint8_t bri =   noise[i][j];

      // if this palette is a 'loop', add a slowly-changing base value
      if ( colorLoop) {
        index += ihue;
      }

      // brighten up, as the color palette itself often contains the
      // light/dark dynamic range desired
      if ( bri > 127 ) {
        bri = 255;
      } else {
        bri = dim8_raw( bri * 2);
      }

      CRGB color = ColorFromPalette( currentPalette, index, bri);
      leds[XY(i, j)] = color;
    }
  }

  ihue += 1;
}

#define HOLD_PALETTES_X_TIMES_AS_LONG 1

void Change_effect() {
  if (Serial.available()) {
    val_BT = Serial.parseInt();
    if (val_BT == 1)  {
      currentPalette = RainbowColors_p;
      speed = 20;
      scale = 30;
      colorLoop = 1;
    }
    if (val_BT == 2)  {
      SetupPurpleAndGreenPalette();
      speed = 10;
      scale = 50;
      colorLoop = 1;
    }
    if (val_BT == 3)  {
      SetupBlackAndWhiteStripedPalette();
      speed = 20;
      scale = 30;
      colorLoop = 1;
    }
    if (val_BT == 4)  {
      currentPalette = ForestColors_p;
      speed =  8;
      scale = 120;
      colorLoop = 0;
    }
    if (val_BT == 5)  {
      currentPalette = CloudColors_p;
      speed =  4;
      scale = 30;
      colorLoop = 0;
    }
    if (val_BT == 6)  {
      currentPalette = LavaColors_p;
      speed =  8;
      scale = 50;
      colorLoop = 0;
    }
    if (val_BT == 7)  {
      currentPalette = OceanColors_p;
      speed = 20;
      scale = 90;
      colorLoop = 0;
    }
    if (val_BT == 8)  {
      currentPalette = PartyColors_p;
      speed = 20;
      scale = 30;
      colorLoop = 1;
    }
    if (val_BT == 9)  {
      SetupRandomPalette();
      speed = 20;
      scale = 20;
      colorLoop = 1;
    }
    if (val_BT == 10)  {
      SetupRandomPalette();
      speed = 50;
      scale = 50;
      colorLoop = 1;
    }
    if (val_BT == 11)  {
      SetupRandomPalette();
      speed = 90;
      scale = 90;
      colorLoop = 1;
    }
    if (val_BT == 12)  {
      currentPalette = RainbowStripeColors_p;
      speed = 30;
      scale = 20;
      colorLoop = 1;
    }
  }
}

void SetupRandomPalette()
{
  currentPalette = CRGBPalette16(
                     CHSV( random8(), 255, 32),
                     CHSV( random8(), 255, 255),
                     CHSV( random8(), 128, 255),
                     CHSV( random8(), 255, 255));
}

void SetupBlackAndWhiteStripedPalette()
{
  // 'black out' all 16 palette entries...
  fill_solid( currentPalette, 16, CRGB::Black);
  // and set every fourth one to white.
  currentPalette[0] = CRGB::White;
  currentPalette[4] = CRGB::White;
  currentPalette[8] = CRGB::White;
  currentPalette[12] = CRGB::White;

}

void SetupPurpleAndGreenPalette()
{
  CRGB purple = CHSV( HUE_PURPLE, 255, 255);
  CRGB green  = CHSV( HUE_GREEN, 255, 255);
  CRGB black  = CRGB::Black;

  currentPalette = CRGBPalette16(
                     green,  green,  black,  black,
                     purple, purple, black,  black,
                     green,  green,  black,  black,
                     purple, purple, black,  black );
}

uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t i;
  if ( MatrixSerpentineLayout == false) {
    i = (y * Width) + x;
  }
  if ( MatrixSerpentineLayout == true) {
    if ( y & 0x01) {
      uint8_t reverseX = (Width - 1) - x;
      i = (y * Width) + reverseX;
    } else {
      // Even rows run forwards
      i = (y * Width) + x;
    }
  }
  return i;
}
