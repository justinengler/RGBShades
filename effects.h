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
char displayText[200] = "HACK ALL THE GLASSES!\0" ;
void hackadayText() {
  // startup tasks
  if (effectInit == false) {
    effectInit = true;
    effectDelay = 40 ;
    xOffset = 0;
  }
  for (byte x = 0; x < kMatrixWidth; x++) {
    for (byte y = 0; y < kMatrixHeight; y++) {
      leds[XY(x,y)] = CRGB::Black;
    }
  }
  //byte sinDistanceR = qmul8(sin8(sineOffset*9*16),2);
  //byte sinDistanceG = qmul8(sin8(sineOffset*10*16),2);
  //byte sinDistanceB = qmul8(sin8(sineOffset*11*16),2);
  
  CRGB c = CHSV(sineOffset,255,255);
  // clear all leds
  drawString(16-xOffset,0,c, CRGB::Black, displayText,&font3x5);
  xOffset++;
  if (xOffset > strlen(displayText)*6) { //roughly double the length of the string
    xOffset = 0;
  }
  sineOffset++; // byte will wrap from 255 to 0, matching sin8 0-255 cycle
}
void hackadayTextWhite() {
  // startup tasks
  if (effectInit == false) {
    effectInit = true;
    effectDelay = 40;
    xOffset = 0;
  }
  for (byte x = 0; x < kMatrixWidth; x++) {
    for (byte y = 0; y < kMatrixHeight; y++) {
      leds[XY(x,y)] = CRGB::Black;
    }
  }

  CRGB c = CRGB::White;
  // clear all leds
  drawString(16-xOffset,0,c, CRGB::Black, displayText,&font3x5);
  xOffset++;
  if (xOffset > strlen(displayText)*6) { //roughly double the length of the string
    xOffset = 0;
  }
  sineOffset++; // byte will wrap from 255 to 0, matching sin8 0-255 cycle
}

void hackadayTextInvert() {
  // startup tasks
  if (effectInit == false) {
    effectInit = true;
    effectDelay = 40;
    xOffset = 0;
  }
  //byte sinDistanceR = qmul8(sin8(sineOffset*9*16),2);
  //byte sinDistanceG = qmul8(sin8(sineOffset*10*16),2);
  //byte sinDistanceB = qmul8(sin8(sineOffset*11*16),2);
  
  CRGB c = CHSV(sineOffset,255,64);
  
  for (byte x = 0; x < kMatrixWidth; x++) {
    for (byte y = 0; y < kMatrixHeight; y++) {
      leds[XY(x,y)] = c;
    }
  }

  // clear all leds
  drawString(16-xOffset,0,CRGB::White, displayText,&font3x5);
  xOffset++;
  if (xOffset > strlen(displayText)*6) { //roughly double the length of the string
    xOffset = 0;
  }
  sineOffset++; // byte will wrap from 255 to 0, matching sin8 0-255 cycle
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
    effectFadeAmount = 1;
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


// sine ripple effect
uint8_t  sineRipple_dist[NUM_LEDS];
void sineRipple() {  // startup tasks
  
  if(effectInit == false) {
    effectInit = true;
    scrollEffect = false;
    effectDelay = 12;
    
    currentPalette = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::Black);
    uint8_t mlt256 = 256 / (kMatrixHeight + kMatrixWidth); // maximum possible distance to origin point
    for(uint8_t y=0; y<kMatrixHeight; y++) {
      for(uint8_t x=0; x<kMatrixWidth; x++) {
        // calculate distance to origin point sqrt((x+_xoffset)*(x+_xoffset) + (y+_yoffset)*(y+_yoffset)) * mlt256
        sineRipple_dist[XY(x,y)] = sqrt((x-7)*(x-7) + (y+1)*(y+1)) * mlt256;
      }
    }
  }
  
  for(uint8_t x=0;x<kMatrixWidth;x++) {
    for(uint8_t y=0;y<kMatrixHeight;y++) {
      leds[XY(x,y)] = ColorFromPalette(currentPalette, quadwave8((((sineRipple_dist[XY(x,y)] * 3) - offset)%256)));
    }
  }
  offset++;
}




// See stuff
void flashlight() {

  // startup tasks
  if (effectInit == false) {
    effectInit = true;
    effectDelay = 50;
  }
  
   for (byte x = 0; x < kMatrixWidth; x++) {
     for (byte y = 0; y < kMatrixHeight; y++) {
      
         leds[XY(x,y)] = CRGB::White;

     }
   }
}

#define CONWAY_DENSITY 128
// game of life
void conway() {

  // startup tasks
  if (effectInit == false) {
    effectInit = true;
    effectDelay = 150;
    effectFadeAmount = 0;
    selectRandomPalette();
    
    
    // new board
    if (random8() < 64) //Draw an LWS
    {
      fillAll(CRGB::Black);
      byte startingoffset = 11;
      leds[XY(1+startingoffset,0)] = ColorFromPalette(currentPalette, random16(255), 255);
      leds[XY(4+startingoffset,0)] = ColorFromPalette(currentPalette, random16(255), 255);
      leds[XY(0+startingoffset,1)] = ColorFromPalette(currentPalette, random16(255), 255);
      leds[XY(0+startingoffset,2)] = ColorFromPalette(currentPalette, random16(255), 255);
      leds[XY(4+startingoffset,2)] = ColorFromPalette(currentPalette, random16(255), 255);
      leds[XY(0+startingoffset,3)] = ColorFromPalette(currentPalette, random16(255), 255);
      leds[XY(1+startingoffset,3)] = ColorFromPalette(currentPalette, random16(255), 255);
      leds[XY(2+startingoffset,3)] = ColorFromPalette(currentPalette, random16(255), 255);
      leds[XY(3+startingoffset,3)] = ColorFromPalette(currentPalette, random16(255), 255);
    }
    else { //random board
      for (byte x = 0; x < kMatrixWidth; x++) {
        for (byte y = 0; y < kMatrixHeight; y++) {
          leds[XY(x,y)] =  random8() > CONWAY_DENSITY ? CRGB::Black : ColorFromPalette(currentPalette, random16(255), 255); 
        }
      }
    }
    
    return;
    
  }
  
  
   CRGB nextstate[ NUM_LEDS ];
   byte life = 0;
   byte growth = 0;
   for (byte x = 0; x < kMatrixWidth; x++) {
     for (byte y = 0; y < kMatrixHeight; y++) {
        nextstate[XY(x,y)] = CRGB::Black;
        //count neighbors
        int neighbors=0;
        if (x>0 && y>0 && leds[XY(x-1,y-1)] ) neighbors++; //top left
        if (y>0 && leds[XY(x,y-1)] ) neighbors++; //top
        if (x<kMatrixWidth-1 && y>0 && leds[XY(x+1,y-1)] ) neighbors++; //top right
        if (x>0 && leds[XY(x-1,y)] ) neighbors++; //left
        if (x<kMatrixWidth-1 && leds[XY(x+1,y)] ) neighbors++; //right
        if (x>0 && y<kMatrixHeight-1  &&  leds[XY(x-1,y+1)] ) neighbors++; //low left
        if ( y<kMatrixHeight-1 &&  leds[XY(x,y+1)] ) neighbors++; //low
        if (x<kMatrixWidth-1 && y<kMatrixHeight-1  &&  leds[XY(x+1,y+1)] ) neighbors++; //low right
        
        if (leds[XY(x,y)])
        {
          if (neighbors < 2) nextstate[XY(x,y)] = CRGB::Black;
          if (neighbors ==2 || neighbors==3)
          {
            nextstate[XY(x,y)] = leds[XY(x,y)].nscale8( 230); //fades old cells
            life++;
          }
          if (neighbors > 3) nextstate[XY(x,y)] = CRGB::Black;
        }
        else if (neighbors == 3) 
         {
           nextstate[XY(x,y)] = ColorFromPalette(currentPalette, random16(255), 255); 
           life++;
           growth++;
         }
     }
   }
   
   memcpy(leds, nextstate, sizeof(leds));
   
   if (life < 5 || growth == -1) //reset if there are too few cells on the board or it's static
   {
     effectDelay=1200;
     effectInit=false;
     effectFadeAmount=1;
   }
     
   
}

//VU testing
#include "audioSamp.h"

const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
const int VUeffectDelay = 25;  //5
const int SAMPS=1200;  //600
void VU() 
{
  
  /****************************************
  Scrolling Sound Meter Sketch for the 
  Adafruit Microphone Amplifier
  ****************************************/

  // startup tasks
  if (effectInit == false) {
    effectInit = true;
    effectDelay = VUeffectDelay;    
    //Serial.println("In VU Init");
  }
  
   int peakToPeak = simpleSample(SAMPS);

   // map 1v p-p level to the max scale of the display
   int displayPeak = map(peakToPeak, 0, 1023, 0, kMatrixWidth-1);
   //int displayPeak = map(peakToPeak, 0, 768, 0, kMatrixWidth-1);


  int i;
  int j;
  // Update the display:
   for (i = kMatrixHeight-1; i>0; i--)  // shift the display down
   {
      for (j = 0; j < kMatrixWidth; j++)
      {
        leds[XY(j,i)] = leds[XY(j,i-1)];
      }
   }

   // draw the new sample
   for (i = 0; i < kMatrixWidth; i++)
   {
       int y=0; //kMatrixWidth-1;
     
      if (i < displayPeak) // Below threshold, draw in color
      {
  
         leds[XY(i,y)]=CHSV(map(i,0,(kMatrixWidth-1)/2,40,0), 255, 255);
        // Serial.print("Samp: ");
        // Serial.println(peakToPeak);
        // Serial.print(" displayPeak: ");
        // Serial.print(displayPeak);
        // Serial.print(" Row: ");
         //Serial.print(i);


      }
      /*else if ( i > kMatrixWidth/2)
      {
        leds[XY(kMatrixWidth-i,y)] = CHSV(map(i,0,kMatrixWidth-1,86,0), 255, 255);
      }
      */
      else   // blank these pixels
      {
         leds[XY(i,y)] = CRGB::Black;
      }

   }
}

//Spectrum Analyzer Test
int localWindowCount;
void spectrumAnalyzerTest()
{
   // startup tasks
  if (effectInit == false) {
    effectInit = true;
    effectDelay = 3;    
    effectNeedsFFT = true;
    effectNeedsSamples = true;
    effectFadeAmount = 12;
    
    localWindowCount=kMatrixWidth;
    
    spectrumSetup(localWindowCount);

    
    //Serial.println("In SA Init");
  }
  
  float intensity, otherMean;
  int x=3;
  int y=1;
  //leds[XY(2,2)]=CRGB::Red;
  for (x = 0; x < localWindowCount; ++x) {
    windowMean(magnitudes, 
               frequencyToBin(frequencyWindow[x]),
               frequencyToBin(frequencyWindow[x+1]),
               &intensity,
               &otherMean);
    // Convert intensity to decibels.
    intensity = 20.0*log10(intensity);
    // Scale the intensity and clamp between 0 and 1.0.
    intensity -= SPECTRUM_MIN_DB;
    intensity = intensity < 0.0 ? 0.0 : intensity;
    intensity /= (SPECTRUM_MAX_DB-SPECTRUM_MIN_DB);
    intensity = intensity * (1+(x*.01));//SCALE experiment
    intensity = intensity > 1.0 ? 1.0 : intensity;
    //pixels.setPixelColor(i, pixelHSVtoRGBColor(hues[i], 1.0, intensity));
    for (y=0; y < kMatrixHeight; y++)
    {
      if (intensity < (0.2+y*.15))
      {
        //leds[XY(x,y)]=CRGB::Black;
        //leds[XY(x,y)].fadeToBlackBy(20);
      }
      else
      {
        leds[XY(x,y)]=CHSV(map(intensity*255,0,255,40,0), 255, 255);
      }
    }
    /*Serial.print("i: ");
    Serial.print(i);
    Serial.print(" - ");
    Serial.println((byte)(intensity*255));*/

    
    //leds[XY(2,2)]=CRGB::Blue;
    
  }

  
}

