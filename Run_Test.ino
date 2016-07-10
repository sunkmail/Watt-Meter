//------------------------ RunTest -----------------------------------------
void runTest()
{
  for (byte i = 0; i <= (arraySize - 1); i++)
  {
    runtimeNow[i] = runtimePlan[i];  // copy plan time into active time
  }

  runTestSetup();                         // set static display elements for testing
  msTimeRun = testTime.makeTime(runtimeNow);   // Convert human readable Runtime into ms
  testTime.startOfTime();                     // Start duration timer
  digitalWrite(Relay_ON, HIGH);               // Turn on the load
//  testRunning = true;

  if (testRunning == true) 
  {
    do                                // Repeat while test running
    {
      powerMeasuring();               // Measure the power data
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
  lastTime = testTime.showTime(runtimeNow,'d','s');
  lcd.print(lastTime);
}
//------------------------ End of runTestUpdate ------------------------------------




//------------------------ powerMeasuring ------------------------------------

void powerMeasuring(void)
{

}
///////////////////////////// End of powerMeasuring ///////////////////////////////////



