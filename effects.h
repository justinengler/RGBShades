//   Graphical effects to run on the RGB Shades LED array
//   Each function should have the following components:
//    * Must be declared void with no parameters or will break function pointer array
//    * Check effectInit, if false then init any required settings and set effectInit true
//    * Set effectDelay (the time in milliseconds until the next run of this effect)
//    * All animation should be controlled with counters and effectDelay, no delay() or loops
//    * Pixel data should be written using leds[XY(x,y)] to map coordinates to the RGB Shades layout

// encode font into bit mask pattern
#include "font.h"
#include "font3x5.h"
// Hackaday Text
byte xOffset = 0;
byte sineOffset = 0; // counter for current position of sine waves
void hackadayText() {
  // startup tasks
  if (effectInit == false) {
    effectInit = true;
    effectDelay = 150;
    xOffset = 0;
  }
  for (byte x = 0; x < kMatrixWidth; x++) {
    for (byte y = 0; y < kMatrixHeight; y++) {
      leds[XY(x,y)] = CRGB::Black;
    }
  }
  byte sinDistanceR = qmul8(sin8(sineOffset*9*16),2);
  byte sinDistanceG = qmul8(sin8(sineOffset*10*16),2);
  byte sinDistanceB = qmul8(sin8(sineOffset*11*16),2);
  
  CRGB c = CHSV(sineOffset,255,255);
  // clear all leds
  drawString(16-xOffset,0,c, CRGB::Black, "HACKADAY.IO",&font3x5);
  xOffset++;
  if (xOffset > 60) {
    xOffset = 0;
  }
  sineOffset++; // byte will wrap from 255 to 0, matching sin8 0-255 cycle
}
void hackadayTextWhite() {
  // startup tasks
  if (effectInit == false) {
    effectInit = true;
    effectDelay = 150;
    xOffset = 0;
  }
  for (byte x = 0; x < kMatrixWidth; x++) {
    for (byte y = 0; y < kMatrixHeight; y++) {
      leds[XY(x,y)] = CRGB::Black;
    }
  }

  CRGB c = CRGB::White;
  // clear all leds
  drawString(16-xOffset,0,c, CRGB::Black, "HACKADAY.IO",&font3x5);
  xOffset++;
  if (xOffset > 60) {
    xOffset = 0;
  }
  sineOffset++; // byte will wrap from 255 to 0, matching sin8 0-255 cycle
}

void hackadayTextInvert() {
  // startup tasks
  if (effectInit == false) {
    effectInit = true;
    effectDelay = 150;
    xOffset = 0;
  }
  byte sinDistanceR = qmul8(sin8(sineOffset*9*16),2);
  byte sinDistanceG = qmul8(sin8(sineOffset*10*16),2);
  byte sinDistanceB = qmul8(sin8(sineOffset*11*16),2);
  
  CRGB c = CHSV(sineOffset,255,64);
  
  for (byte x = 0; x < kMatrixWidth; x++) {
    for (byte y = 0; y < kMatrixHeight; y++) {
      leds[XY(x,y)] = c;
    }
  }

  // clear all leds
  drawString(16-xOffset,0,CRGB::White, "HACKADAY.IO",&font3x5);
  xOffset++;
  if (xOffset > 60) {
    xOffset = 0;
  }
  sineOffset++; // byte will wrap from 255 to 0, matching sin8 0-255 cycle
}

int offCounter = 0;
void hackadayTextMulti() {
  if (effectInit == false) {
    effectInit = true;
    effectDelay = 10;
    sineOffset = 0;
    xOffset = 0;
    offCounter = 0;
  }
  
  // clear
  for (byte x = 0; x < kMatrixWidth; x++) {
    for (byte y = 0; y < kMatrixHeight; y++) {
      leds[XY(x,y)] = CRGB::Black;
    }
  }
  
  int xcnt, ycnt, i = 0, offset = 0;
  int16_t x = 16-xOffset, y = 0;
  char character;
  const bitmap_font *font = &font3x5;
  const char text[] = "HACKADAY.IO";
  
  // limit text to 10 chars, why?
  for (i = 0; i < 20; i++) {
      character = text[offset++];
      if (character == '\0')
          break;
      
      CRGB c = CHSV((i*30)+sineOffset,255,128);
      
      for (ycnt = 0; ycnt < font->Height; ycnt++) {
          for (xcnt = 0; xcnt < font->Width; xcnt++) {
              if (getBitmapFontPixelAtXY(character, xcnt, ycnt, font)) {
                  leds[XY(x + xcnt,y + ycnt)] = c;
              }
          }
      }
      x += font->Width;
  }
  if (offCounter > 15) {
    xOffset++;
    if (xOffset > 60) {
      xOffset = 0;
    }
    offCounter = 0;
  }
  if (offCounter%3==0) {  
    sineOffset++;
  }
  offCounter++;
}
// Triple Sine Waves
void threeSine() {
  
  // startup tasks
  if (effectInit == false) {
    effectInit = true;
    effectDelay = 20;
  }

  // Draw one frame of the animation into the LED array
  for (byte x = 0; x < kMatrixWidth; x++) {
    for (int y = 0; y < kMatrixHeight; y++) {

      // Calculate "sine" waves with varying periods
      // sin8 is used for speed; cos8, quadwave8, or triwave8 would also work here
      byte sinDistanceR = qmul8(abs(y*(255/kMatrixHeight) - sin8(sineOffset*9+x*16)),2);
      byte sinDistanceG = qmul8(abs(y*(255/kMatrixHeight) - sin8(sineOffset*10+x*16)),2);
      byte sinDistanceB = qmul8(abs(y*(255/kMatrixHeight) - sin8(sineOffset*11+x*16)),2);

      leds[XY(x,y)] = CRGB(255-sinDistanceR, 255-sinDistanceG, 255-sinDistanceB); 
    }
  }
  
  sineOffset++; // byte will wrap from 255 to 0, matching sin8 0-255 cycle

}





// RGB Plasma
byte offset  = 0; // counter for radial color wave motion
int plasVector = 0; // counter for orbiting plasma center
void plasma() {

  // startup tasks
  if (effectInit == false) {
    effectInit = true;
    effectDelay = 10;
  }

  // Calculate current center of plasma pattern (can be offscreen)
  int xOffset = cos8(plasVector/256);
  int yOffset = sin8(plasVector/256);

  // Draw one frame of the animation into the LED array
  for (int x = 0; x < kMatrixWidth; x++) {
    for (int y = 0; y < kMatrixHeight; y++) {
      byte color = sin8(sqrt(sq(((float)x-7.5)*10+xOffset-127)+sq(((float)y-2)*10+yOffset-127))+offset);
      leds[XY(x,y)] = CHSV(color, 255, 255);
    }    
  }

  offset++; // wraps at 255 for sin8
  plasVector += 16; // using an int for slower orbit (wraps at 65536)

}


// Scanning pattern left/right, uses global hue cycle
byte riderPos = 0;
void rider() {
  
  // startup tasks
  if (effectInit == false) {
    effectInit = true;
    effectDelay = 5;
    riderPos = 0;
  }

  // Draw one frame of the animation into the LED array
  for (byte x = 0; x < kMatrixWidth; x++) {
    int brightness = abs(x*(256/kMatrixWidth) - triwave8(riderPos)*2 + 127)*3;
    if (brightness > 255) brightness = 255;
    brightness = 255 - brightness;
    CRGB riderColor = CHSV(cycleHue, 255, brightness);
    for (byte y = 0; y < kMatrixHeight; y++) {
      leds[XY(x,y)] = riderColor;
    }
  }

  riderPos++; // byte wraps to 0 at 255, triwave8 is also 0-255 periodic

}



// Shimmering noise, uses global hue cycle
void glitter() {

  // startup tasks
  if (effectInit == false) {
    effectInit = true;
    effectDelay = 15;
  }

  // Draw one frame of the animation into the LED array
  for (int x = 0; x < kMatrixWidth; x++) {
    for (int y = 0; y <kMatrixHeight; y++) {
      leds[XY(x,y)] = CHSV(cycleHue,255,random8(5)*63);
    }
  }

}


// Fills saturated colors into the array from alternating directions
byte currentColor = 0;
byte currentRow = 0;
byte currentDirection = 0;
void colorFill() {
  
  // startup tasks
  if (effectInit == false) {
    effectInit = true;
    effectDelay = 45;
    currentColor = 0;
    currentRow = 0;
    currentDirection = 0;
    currentPalette = RainbowColors_p;
  }

  // test a bitmask to fill up or down when currentDirection is 0 or 2 (0b00 or 0b10)
  if (!(currentDirection & 1)) {
    effectDelay = 45; // slower since vertical has fewer pixels
    for (byte x = 0; x < kMatrixWidth; x++) {
      byte y = currentRow;
      if (currentDirection == 2) y = kMatrixHeight - 1 - currentRow;
      leds[XY(x,y)] = currentPalette[currentColor];
    }
  } 

  // test a bitmask to fill left or right when currentDirection is 1 or 3 (0b01 or 0b11)
  if (currentDirection & 1) {
    effectDelay = 20; // faster since horizontal has more pixels
    for (byte y = 0; y < kMatrixHeight; y++) {
      byte x = currentRow;
      if (currentDirection == 3) x = kMatrixWidth - 1 - currentRow;
      leds[XY(x,y)] = currentPalette[currentColor];
    }
  }

  currentRow++;
  
  // detect when a fill is complete, change color and direction
  if ((!(currentDirection & 1) && currentRow >= kMatrixHeight) || ((currentDirection & 1) && currentRow >= kMatrixWidth)) {
    currentRow = 0;
    currentColor += random8(3,6);
    if (currentColor > 15) currentColor -= 16;
    currentDirection++;
    if (currentDirection > 3) currentDirection = 0;
    effectDelay = 300; // wait a little bit longer after completing a fill
  }


}

// Emulate 3D anaglyph glasses
void threeDee() {

  // startup tasks
  if (effectInit == false) {
    effectInit = true;
    effectDelay = 50;
  }
  
   for (byte x = 0; x < kMatrixWidth; x++) {
     for (byte y = 0; y < kMatrixHeight; y++) {
       if (x < 7) {
         leds[XY(x,y)] = CRGB::Blue;
       } else if (x > 8) {
         leds[XY(x,y)] = CRGB::Red;
       } else {
         leds[XY(x,y)] = CRGB::Black;
       }
     }
   }
   
   leds[XY(6,0)] = CRGB::Black;
   leds[XY(9,0)] = CRGB::Black;

}

// Random pixels scroll sideways, uses current hue
#define rainDir 0
void sideRain() {

  // startup tasks
  if (effectInit == false) {
    effectInit = true;
    effectDelay = 30;
  }
  
  scrollArray(rainDir);
  byte randPixel = random8(kMatrixHeight);
  for (byte y = 0; y < kMatrixHeight; y++) leds[XY((kMatrixWidth-1) * rainDir,y)] = CRGB::Black;
  leds[XY((kMatrixWidth-1)*rainDir, randPixel)] = CHSV(cycleHue, 255, 255); 

}

// Pixels with random locations and random colors selected from a palette
// Use with the fadeAll function to allow old pixels to decay
void confetti() {

  // startup tasks
  if (effectInit == false) {
    effectInit = true;
    effectDelay = 10;
    selectRandomPalette();
  }

  // scatter random colored pixels at several random coordinates
  for (byte i = 0; i < 4; i++) {
    leds[XY(random16(kMatrixWidth),random16(kMatrixHeight))] = ColorFromPalette(currentPalette, random16(255), 255);//CHSV(random16(255), 255, 255);
    random16_add_entropy(1);
  }
   
}


// Draw slanting bars scrolling across the array, uses current hue
byte slantPos = 0;
void slantBars() {

  // startup tasks
  if (effectInit == false) {
    effectInit = true;
    effectDelay = 5;
  }

  for (byte x = 0; x < kMatrixWidth; x++) {
    for (byte y = 0; y < kMatrixHeight; y++) {
      leds[XY(x,y)] = CHSV(cycleHue, 255, quadwave8(x*32+y*32+slantPos));
    }
  }

  slantPos-=4;

}

