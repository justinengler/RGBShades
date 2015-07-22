

#define ARM_MATH_CM4
#include <arm_math.h>

////////////////////////////////////////////////////////////////////////////////
// CONIFIGURATION 
// These values can be changed to alter the behavior of the spectrum display.
////////////////////////////////////////////////////////////////////////////////

int SAMPLE_RATE_HZ = 30000;             // Sample rate of the audio in hertz.
float SPECTRUM_MIN_DB = 20.0;          // Audio intensity (in decibels) that maps to low LED brightness.
float SPECTRUM_MAX_DB = 60.0;          // Audio intensity (in decibels) that maps to high LED brightness.

const int FFT_SIZE = 256;              // Size of the FFT.  Realistically can only be at most 256 
                                       // without running out of memory for buffers and other state.
const int ANALOG_READ_RESOLUTION = 10; // Bits of resolution for the ADC.
const int ANALOG_READ_AVERAGING = 16;  // Number of samples to average with each ADC reading.


float samples[FFT_SIZE*2];
float magnitudes[FFT_SIZE];
int sampleCounter = 0;

const byte WINDOWCOUNT = 16;
float frequencyWindow[WINDOWCOUNT+1];

int SAMPDELAY =  5;//1000000/SAMPLE_RATE_HZ;

int simpleSample(int numsamps)
{
   unsigned int sample;

   //unsigned long startMillis= millis();  // Start of sample window
   unsigned int peakToPeak = 0;   // peak-to-peak level

   unsigned int signalMax = 0;
   unsigned int signalMin = 1024;

  
   for  (int i=0; i<numsamps; i++)
   {
      sample = analogRead(SOUND_PIN); 
      //Serial.println("Read Value:");
      //Serial.println(sample);
      if (sample < 1024)  // toss out spurious readings
      {
         if (sample > signalMax)
         {
            signalMax = sample;  // save just the max levels
         }
         else if (sample < signalMin)
         {
            signalMin = sample;  // save just the min levels
         }
      }
   }
   peakToPeak = signalMax - signalMin;
   return peakToPeak;
}


////////////////////////////////////////////////////////////////////////////////
// SAMPLING FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

void samplingCallback() {
  while (sampleCounter < FFT_SIZE*2){
    // Read from the ADC and store the sample data
    samples[sampleCounter] = (float32_t)analogRead(SOUND_PIN);
    // Complex FFT functions require a coefficient for the imaginary part of the input.
    // Since we only have real data, set this coefficient to zero.
    samples[sampleCounter+1] = 0.0;
    // Update sample buffer position and stop after the buffer is filled
    sampleCounter += 2;
  }
  if (sampleCounter >= FFT_SIZE*2) {
  //  samplingTimer.end();
  }
}

/*
void samplingBegin() {
  // Reset sample buffer position and start callback at necessary rate.
  //sampleCounter = 0;
  //samplingTimer.begin(samplingCallback, 1000000/SAMPLE_RATE_HZ);
}*/

boolean samplingIsDone() {
  if (sampleCounter >= FFT_SIZE*2){
    //Serial.print("Called samplingIsDone When it was done - ");
    //Serial.println(sampleCounter);
  }
  
  return sampleCounter >= FFT_SIZE*2;
}


// Convert a frequency to the appropriate FFT bin it will fall within.
int frequencyToBin(float frequency) {
  float binFrequency = float(SAMPLE_RATE_HZ) / float(FFT_SIZE);
  return int(frequency / binFrequency);
}


// Compute the average magnitude of a target frequency window vs. all other frequencies.
void windowMean(float* magnitudes, int lowBin, int highBin, float* windowMean, float* otherMean) {
    *windowMean = 0;
    *otherMean = 0;
    // Notice the first magnitude bin is skipped because it represents the
    // average power of the signal.
    for (int i = 1; i < FFT_SIZE/2; ++i) {
      if (i >= lowBin && i <= highBin) {
        *windowMean += magnitudes[i];
      }
      else {
        *otherMean += magnitudes[i];
      }
    }
    *windowMean /= (highBin - lowBin) + 1;
    *otherMean /= (FFT_SIZE / 2 - (highBin - lowBin));
}

void spectrumSetup(int windowcount) {
  // Set the frequency window values by evenly dividing the possible frequency
  // spectrum across the number of neo pixels.
  float windowSize = (SAMPLE_RATE_HZ / 2.0) / float(windowcount);
  for (int i = 0; i < windowcount+1; ++i) {
    frequencyWindow[i] = i*windowSize;
    Serial.print(" Window ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(frequencyWindow[i]);
  }

}

void spectrumSetupExp(int windowcount) {
  
  float windowSize = log((SAMPLE_RATE_HZ / 2.0))/log(2);
  
  for (int i = 0; i < windowcount+1; i++)
  {
    frequencyWindow[i]=windowSize;
    windowSize = windowSize*2;
    Serial.print(" Window ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(frequencyWindow[i]);
  }
}
