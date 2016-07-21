//------------------------ RunTest -----------------------------------------
void runTest()
{
  // Run once at start of testing run

  for (byte i = 0; i <= (arraySize - 1); i++)
  {
    runtimeNow[i] = runtimePlan[i];  // copy plan time into active time
  }

  runTestSetup();                         // set static display elements for testing

  msTimeRun = testTime.makeTime(runtimeNow);   // Convert human readable Runtime into ms

  testTime.startOfTime();                     // Start duration timer
  DBprintln("set reference time to zero");

  digitalWrite(Relay_ON, HIGH);               // Turn on the load
  DBprintln("Turn ON Load Relay");


  if (testRunning == true)      // Run in Loop while testing
  {
    do                                // Repeat while test running
    {
      powerMeasuring();               // Measure the power data

      computeData();              // Do the math for all variables

      runTestUpdate();             // Display latest data

      if (digitalRead(RtBut) == LOW)        // If STOP pressed
      {
        testRunning = false;                // Set Flag to exit Loop
        digitalWrite(Relay_ON, LOW);        // Turn off load
        msTimeRun = testTime.doingTime(runtimeNow);     // Take one last duration measurement
      }

    } while (testRunning == true);


    lastTime = testTime.showTime(runtimeNow, 'd', 's');
  }

}
//////////////////////// End of runTest ///////////////////



//------------------------ runTestSetup ------------------------------------
void runTestSetup()
{
  lcd.clear();
  lcd.setCursor(0, 2);
  lcd.print("Runtime: ");

  lcd.setCursor(16, 3);
  lcd.print("STOP");              // Rt button action

}
//------------------------ End of runTestSetup------------------------------------



//------------------------ runTestUpdate ------------------------------------

void runTestUpdate()
{
  //  lastTime = testTime.doingtime(runtimeNow,'d','s');
  lcd.setCursor(9, 2);
  testTime.doingTime(runtimeNow);                     // get elapsed time
  lastTime = testTime.showTime(runtimeNow, 'd', 's');
  lcd.print(lastTime); 
  
  DBprint(lastTime);
  DBprint(" ");
  DBprint(Vrms,3);
  DBprint("V, ");
  DBprint(Iave,3);
  DBprintln("A");
}
//------------------------ End of runTestUpdate ------------------------------------




//------------------------ powerMeasuring ------------------------------------

void powerMeasuring(void)
{
  float Vaveraging = 0.0;
  float Iaveraging = 0.0;

  for (byte i = 0; (i <= (SampleSize - 1)); i++)
  {
    Vworking[i] = voltageReading();
    Vaveraging += Vworking[i];
    Iworking[i] = currentReading();
    Iaveraging += Iworking[i];
  }
  sampleTime = testTime.doingTime(runtimeNow);  // record time last sample block taken
  
  Vaveraging = Vaveraging / SampleSize;   // average of the group of samples just taken
  Iaveraging = Iaveraging / SampleSize;   //

  Vrms = ((Vrms * TotalSamples) + (Vaveraging ) / (TotalSamples + 1)); // Cumulative moving average
  Iave = ((Iave * TotalSamples) + (Iaveraging ) / (TotalSamples + 1)); // Cumulative moving average
  TotalSamples++;




}
///////////////////////////// End of powerMeasuring ///////////////////////////////////





//------------------------ voltageReading ------------------------------------

float voltageReading(void)
{
  float V = 0.0;
  adcVsenseRaw = ads.readADC_SingleEnded(VsenseChannel);
  V = adcVsenseRaw * 0.0001875;  // ADC multiplier for Volts with 2/3 gain
  return V;
}
///////////////////////////// End of voltageReading ///////////////////////////////////





//------------------------ CurrentReading ------------------------------------

float currentReading(void)
{
  float I = 0.0;
  adcIsenseRaw = ads.readADC_SingleEnded(IsenseChannel);
  I = ((adcIsenseRaw * 0.0001875) + IZeroingValue);  // ADC multiplier for Volts with 2/3 gain
  return I;
}
///////////////////////////// End of CurrentReading ///////////////////////////////////






//------------------------ computeData ------------------------------------

void computeData(void)
{
  Watts = Vrms * Iave;

  WattHours = ((Watts * sampleTime) /OneHour );
}
///////////////////////////// End of computeData ///////////////////////////////////


