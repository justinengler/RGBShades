//   RGB Shades Demo Code
//   Copyright (c) 2014 macetech LLC
//   This software is provided under the MIT License (see license.txt)
//   Special credit to Mark Kriegsman for XY mapping code
//
//   Use Version 3.0 or later https://github.com/FastLED/FastLED
//   ZIP file https://github.com/FastLED/FastLED/archive/master.zip
//
//   Use Arduino IDE 1.0 or later
//   Select device "Arduino Pro or Pro Mini (5V, 16MHz) w/ATmega328
//
//   [Press] the SW1 button to cycle through available effects
//   Effects will also automatically cycle at startup
//   [Press and hold] the SW1 button (one second) to switch between auto and manual mode
//     * Auto Mode (one blue blink): Effects automatically cycle over time
//     * Manual Mode (two red blinks): Effects must be selected manually with SW1 button
//
//   [Press] the SW2 button to cycle through available brightness levels
//   [Press and hold] the SW2 button (one second) to reset brightness to startup value



// RGB Shades data output to LEDs is on pin 5
#define LED_PIN  3

//D23=A9?
#define SOUND_PIN A9

//Hardware Serial
#define HWSERIAL Serial1

//Bluetooth
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_UART.h"
#include "BluefruitConfig.h"
Adafruit_BluefruitLE_UART ble(BLUEFRUIT_HWSERIAL_NAME, BLUEFRUIT_UART_MODE_PIN);
// function prototypes over in packetparser.cpp
uint8_t readPacket(Adafruit_BLE *ble, uint16_t timeout);
float parsefloat(uint8_t *buffer);
void printHex(const uint8_t * data, const uint32_t numBytes);
// the packet buffer
extern uint8_t packetbuffer[];
#define BUTTONUP '5'
#define BUTTONDOWN '6'
#define BUTTONLEFT '7'
#define BUTTONRIGHT '8'


//Fakey Grounds Here
#define G1 9
#define G2 10 //this one doesn't appear to work?

// RGB Shades color order (Green/Red/Blue)
#define COLOR_ORDER GRB
#define CHIPSET     WS2811

// Global maximum brightness value, maximum 255
#define MAXBRIGHTNESS 72
#define STARTBRIGHTNESS 32
byte currentBrightness = STARTBRIGHTNESS; // 0-255 will be scaled to 0-MAXBRIGHTNESS

// Include FastLED library and other useful files
#include <FastLED.h>
#include "XYmap.h"
#include "utils.h"
#include "effects.h"
#include "buttons.h"

// Runs one time at the start of the program (power up or reset)
void setup() {
  
  delay(200); //prevents startup crash

  //Adding debugging serial
  //while (!Serial);  // required waits for serial connection
  Serial.begin(9600);
  Serial.println("Booting");
  
  analogReadResolution(ANALOG_READ_RESOLUTION);
  analogReadAveraging(ANALOG_READ_AVERAGING);
  analogReference(INTERNAL);
  
  // write FastLED configuration data
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, LAST_VISIBLE_LED + 1);//.setCorrection(TypicalSMD5050);
  
  // set global brightness value
  FastLED.setBrightness( scale8(STARTBRIGHTNESS, MAXBRIGHTNESS) );

  // configure input buttons
  pinMode(MODEBUTTON, INPUT_PULLUP);
  pinMode(BRIGHTNESSBUTTON, INPUT_PULLUP);

  // fakey grounds setup
  pinMode(G1, OUTPUT);
  pinMode(G1, OUTPUT);
  digitalWrite(G1, LOW);
  digitalWrite(G2, LOW);
  
  //CTS setup
  pinMode(2,OUTPUT);
  digitalWrite(2,LOW);

  //HWSERIAL.println("Setup Done");

  //Bluetooth
  ble.begin();
  ble.echo(false);
  Serial.println( F("Switching to DATA mode!") );
  ble.setMode(BLUEFRUIT_MODE_DATA);
  
}

// list of functions that will be displayed
functionList effectList[] = {
//hackadayText,hackadayTextWhite,hackadayTextInvert,hackadayTextMulti,

                             spectrumAnalyzerTest,
                             VU,
                             threeSine,
                             //threeDee,
                             plasma,
                             confetti,
                             rider,
                             //glitter,
                             slantBars,
                             //colorFill,
                             sideRain,
                             flashlight
                           };

// Timing parameters
#define cycleTime 15000
#define hueTime 30

void initEffect(){
    cycleMillis = currentMillis; 
     effectInit = false; // trigger effect initialization when new effect is selected
     effectNeedsSamples = false;
     effectNeedsFFT = false;
     effectFadeAmount = 0;
     Serial.print("New Mode:");
     Serial.println(currentEffect);
}
     
void nextEffect(){
      if (++currentEffect >= numEffects) currentEffect = 0; // loop to start of effect list
      initEffect();
}

void prevEffect(){
      if (--currentEffect < 0 || currentEffect > numEffects) currentEffect = numEffects-1; // loop to start of effect list
      initEffect();
}

void cycleModeToggle(){
      autoCycle = !autoCycle; // toggle auto cycle mode
      confirmBlink(); // one blue blink: auto mode. two red blinks: manual mode.
}

void cycleMode(boolean newmode){
      autoCycle=newmode;
}

void brightnessUp(){
       currentBrightness += 16; // increase the brightness (wraps to lowest)
      FastLED.setBrightness(scale8(currentBrightness,MAXBRIGHTNESS));
      Serial.println("BrightnessUp");
}

void brightnessDown(){
       currentBrightness -= 16; // increase the brightness (wraps to highest?)
      FastLED.setBrightness(scale8(currentBrightness,MAXBRIGHTNESS));
      Serial.println("BrightnessDown");
}

void brightnessDefault(){
        currentBrightness = STARTBRIGHTNESS; // reset brightness to startup value
      FastLED.setBrightness(scale8(currentBrightness,MAXBRIGHTNESS));
      Serial.println("BrightnessDefault");
}

// Runs over and over until power off or reset
void loop()
{
  currentMillis = millis(); // save the current timer value

  
  
 //Bluetooth
 /* Wait for new data to arrive */
  uint8_t blelen = readPacket(&ble, BLE_READPACKET_TIMEOUT);
  
  //if there was BLE data, do something
  if (blelen != 0)
  {  
    
      /* Got a packet! */
    printHex(packetbuffer, blelen);
    if (packetbuffer[1]=='C')  //color packet
    {
    }
    if (packetbuffer[1]=='B')  //button packet
    {
      //brightness
      if (packetbuffer[2]==BUTTONUP && packetbuffer[3]=='0')
        brightnessUp();
      if (packetbuffer[2]==BUTTONDOWN && packetbuffer[3]=='0')
        brightnessDown();
      if (packetbuffer[2]=='2' && packetbuffer[3]=='0')
        brightnessDefault();
        
      //mode
      if (packetbuffer[2]==BUTTONRIGHT && packetbuffer[3]=='0')
        nextEffect();
      if (packetbuffer[2]==BUTTONLEFT && packetbuffer[3]=='0')
        prevEffect();
      if (packetbuffer[2]=='1' && packetbuffer[3]=='0')
        cycleModeToggle();

    }
      
  }
  
  updateButtons(); // read, debounce, and process the buttons
  // Check the mode button (for switching between effects)
  switch(buttonStatus(0)) {
    
    case BTNRELEASED: // button was pressed and released quickly
      nextEffect();
    break;
    
    case BTNLONGPRESS: // button was held down for a while
      cycleModeToggle();
    break;
  
  }
  
  // Check the brightness adjust button  
  switch(buttonStatus(1)) {
    
    case BTNRELEASED: // button was pressed and released quickly
      brightnessUp();
    break;
    
    case BTNLONGPRESS: // button was held down for a while
      brightnessDefault();
    break;
  
  }
  
  // switch to a new effect every cycleTime milliseconds
  if (currentMillis - cycleMillis > cycleTime && autoCycle == true) {
    nextEffect();
  }
  
  // increment the global hue value every hueTime milliseconds
  if (currentMillis - hueMillis > hueTime) {
    hueMillis = currentMillis;
    hueCycle(1); // increment the global hue value
  }
  
  // run the currently selected effect every effectDelay milliseconds
  if (currentMillis - effectMillis > effectDelay) {
    effectMillis = currentMillis;
    effectList[currentEffect](); // run the selected effect function
    random16_add_entropy(1); // make the random values a bit more random-ish
  }
  
  // take samples if it's time
  if (effectNeedsSamples && ((currentMillis - sampMillis) > SAMPDELAY))
  {
    sampMillis = currentMillis;
    samplingCallback();
    
    /*Serial.print("Called samplingCallback() - ");
    Serial.print("currentMillis=");
    Serial.print(currentMillis);
    Serial.print(" sampMillis=");
    Serial.print(sampMillis);
    Serial.print(" SAMPDELAY=");
    Serial.println(SAMPDELAY);
    */
  //}
  
  //if (effectNeedsFFT && (samplingIsDone()))
  //{
    //Serial.println("FFT Time");
    // Run FFT on sample data.
    arm_cfft_radix4_instance_f32 fft_inst;
    arm_cfft_radix4_init_f32(&fft_inst, FFT_SIZE, 0, 1);
    arm_cfft_radix4_f32(&fft_inst, samples);
    // Calculate magnitude of complex numbers output by the FFT.
    arm_cmplx_mag_f32(samples, magnitudes, FFT_SIZE);
    
    /*Serial.println("RUN START");
    for (int i=0; i<FFT_SIZE; i++){
      Serial.print("Row ");
      Serial.print(i);
      Serial.print(" - Samp1=");    
      Serial.print(samples[i*2]);
      Serial.print(" - Samp2=");
      Serial.print(samples[(i*2)+1]);
      Serial.print(" Mag=");
      Serial.println(magnitudes[i]);
    }
    Serial.println("RUN END");*/
    sampleCounter = 0;
  }
  
  // run a fade effect too if the confetti effect is running
  if (effectFadeAmount) fadeAll(effectFadeAmount);
  
  FastLED.show(); // send the contents of the led memory to the LEDs

}





