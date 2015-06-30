





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

