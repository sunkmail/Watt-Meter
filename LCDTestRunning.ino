

//------------------------ LCDRunningSetup ------------------------------------
void LCDRunningSetup()
{
  lcd.clear();
  lcd.setCursor(16, 3);           
  lcd.print("Stop");              // Rt button action
  
  lcd.setCursor(0, 2);
  lcd.print("Runtime: ");
  
}
//------------------------ End of LCDTestRunning ------------------------------------





//------------------------ LCDRunningUpdate ------------------------------------

void LCDRunningUpdate()
{
//  lastTime = testTime.doingtime(runtimeNow,'d','s');
  lcd.setCursor(9, 2);
  testTime.doingTime(runtimeNow);                     // get elapsed time
  lastTime = testTime.showTime(runtimeNow,'d','s');
  lcd.print(lastTime);
}
//------------------------ End of LCDTestRunningUpdate ------------------------------------



