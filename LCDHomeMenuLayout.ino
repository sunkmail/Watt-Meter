//------------------------ LCDHomeMenuLayout ------------------------------------


void LCDHomeMenuLayout(void)
{
  lcd.clear();
  // Data Display from Previous Test

  //------------  Vrms  ------------
  
  lcd.setCursor(0, 0);      // move cursor to first position (0) of the Top (0) Line
  if (Vrms >= 100)
    lcd.print("V: ");       //to display Vrms in the right spot if 2 leading digits
  else if(Vrms >= 10)
    lcd.print("V:  ");      // Add extra space for 1 leading digit Vrms
  else
    lcd.print("V:   ");

  lcd.print(Vrms, 2);       // Display previous run Value w/ 2 decimal places


  //------------ Iave ------------
  lcd.setCursor(11, 0);     // move cursor to position 10 of the Top (0) Line
  lcd.print("Iave ");
  lcd.print(Iave, 2);


  //------------ Watt Hours ------------
  lcd.setCursor(0, 1);               // move cursor to first position (0) of the second (1) Line
  if (WattHours < 10)
  {
    lcd.print("Wh:    ");            // Adjust spacing for 1 leading digit (0 -9.xx) WattHours reading
    lcd.print(WattHours, 2);
  }
  else if (WattHours < 100)
  {
    lcd.print("Wh:  ");             // Adjust spacing for 2 leading digit (10 -99.xx) WattHours reading
    lcd.print(WattHours, 2);
  }
  else if (WattHours < 1000)
  {
    lcd.print("Wh: ");              // Adjust spacing for 3 leading digit (100 -999.xx) WattHours reading
    lcd.print(WattHours, 2);
  }
  else
  {
    lcd.print("kWh: ");              // Adjust for kWh vs. Wh WattHours reading
    lcd.setCursor(5, 1);            // move cursor to position 4 of the second (1) Line
    lcd.print((WattHours / 1000), 2); // Display kWh to 2 decimals
    // *****  Add code here to switch to 2 decimal places if kWh overruns next display section ***
  }

  //------------ Imax ------------
  lcd.setCursor(12, 1);
  lcd.print("Imax ");
  lcd.print(Imax, 1);           // Imax displayed to 1 decimal place


  //------------ Next Run Info ------------
  lcd.setCursor(0, 2);
  lcd.print("Nxt Run: ");   // Follow with
  lcd.print(nextTime);

  //------------ Main Menu buttons ------------
  lcd.setCursor(0, 3);
  lcd.print("START");             //To Start the Test
  lcd.setCursor(16, 3);           
  lcd.print("Next");              // To go to next menu page
}

//------------------------ End of LCDHomeMenuLayout ------------------------------------

