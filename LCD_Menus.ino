/*
   Included Functions:

    MenuLoop()      - Control function to handle menu system

    LCDHomeMenu()   - Basic info from last run
                        - Update to include: Time, W, WH, A (if space allows)
                    - Next run info
                    - START button
                    - NEXT button, for LCD2nd Menu

    LCD2ndMenu()



*/
//------------------------ MenuLoop ------------------------------------

void MenuLoop()
{
  LCDHomeMenu();                    // Display Main Menu
  delay(20);                        // Attempt at easy debouncing

  do  // Stay in the Menu Loop until the test is activated
  {
    if (digitalRead(LtBut) == LOW)  // If START pressed from Main menu...
    {
      testRunning = true;           // Flag to indicate the test is running on the load to be measured
    }


    if (digitalRead(RtBut) == LOW)  // If NEXT pressed from Main Menu
    {
      LCD2ndMenu();
    }


  } while (testRunning == false);     // Stay in the Menu Loop whenever the test is not active

}

////////////////  End of Menu Loop ////////////////////////////////////////////



//------------------------ LCDHomeMenu ------------------------------------
//  Display info only - nothing active.  Button check to Start in main program Loop.

void LCDHomeMenu(void)
{
  lcd.clear();
  // Data Display from Previous Test

  //     Top Row

  lcd.setCursor(0, 0);      // move cursor to first position (0) of the Top (0) Line
  dispVrms();

  lcd.setCursor(11, 0);     // move cursor to position 10 of the Top (0) Line
  dispIave();

  //      2nd Row
  lcd.setCursor(0, 1);               // move cursor to first position (0) of the second (1) Line
  dispWh();

  lcd.setCursor(11, 1);
  dispImax();


  //------------ Next Run Info ------------
  lcd.setCursor(0, 2);
  lcd.print("Nxt Run: ");   // Follow with time, or 'run till stop'
  if (timePlan == 0)            // if planned # of ms is zero
    lcd.print("'til Stop'd");
  else
    lcd.print(nextTime);      // Next planned runtime isn't zero

  //------------ Main Menu buttons ------------
  lcd.setCursor(0, 3);
  lcd.print("START");             //To Start the Test
  lcd.setCursor(16, 3);
  lcd.print("Next");              // To go to next menu page
}

/////////////////////////  End of LCDHomeMenu ////////////////////////////////////




/////////////////////////Start of LCD2ndMenu ------------------------------------
//             Details of last run

void LCD2ndMenu()
{
  //  do
  //  {
  lcd.clear();
  //------------ Top Row
  lcd.setCursor(0, 0);
  lcd.print("Elapsed: ");
  lcd.print(lastTime);

  //------------ Second Row
  lcd.setCursor(0, 1);      // move cursor to first position (0) of the 2nd (1) Line
  dispVrms();

  lcd.setCursor(11, 1);
  dispIave();

  //-------------- 3rd Row
  lcd.setCursor(0, 2);      // move cursor to first position (0) of the 3rd (2) Line
  dispWh();

  lcd.setCursor(11, 2);
  dispImax();


  //------------ 2nd Menu buttons ------------
  lcd.setCursor(0, 3);
  lcd.print("HOME");             //To Return to main (Home) menu
  lcd.setCursor(16, 3);
  lcd.print("Next");             // To go to next menu page - 'Set-up'
  //  }
}
/////////////////////////  End of LCD2ndMenu ////////////////////////////////////



///////////////////////// Start of dispVrms ------------------------------------

void dispVrms()             // Set cursor position before calling
// uses 8 display Chars, "V:xxX.XX"
{
  if (Vrms >= 100)
    lcd.print("Vav:");       //to display Vrms in the right spot if 2 leading digits

  else if (Vrms >= 10)
    lcd.print("Vav: ");      // Add extra space for 1 leading digit Vrms
  else
    lcd.print("Vav:  ");

  lcd.print(Vrms, 2);       // Display previous run Value w/ 2 decimal places
}

/////////////////////////  End of dispVrms ////////////////////////////////////






///////////////////////// Start of dispIave ------------------------------------

void dispIave()             // Current never goes above 2 digits (15A)
// Set cursor position before calling
// uses 8 Chars,   "Iav xX.xx"
{
  if (Iave < 10)
    lcd.print("Iav: ");
  else
    lcd.print("Iav:");

  lcd.print(Iave, 2);       // Iave to 2 decimal
}

/////////////////////////  End of dispVrms ////////////////////////////////////



///////////////////////// Start of dispWh ------------------------------------
// Set cursor position before calling
void dispWh()                       // uses 10 Chars
{
  float kWattHours =  WattHours / 1000;
  if (WattHours < 10)
  {
    lcd.print("Wh:  ");            // Adjust spacing for (0 -9.xxx) WattHours reading
    lcd.print(WattHours, 3);
  }
  else if (WattHours < 100)
  {
    lcd.print("Wh: ");             // Adjust spacing for (10 -99.xxx) WattHours reading
    lcd.print(WattHours, 3);
  }
  else if (WattHours < 1000)
  {
    lcd.print("kWh:  ");              // Adjust spacing (0 - 9.xxx) kilo WattHours reading
    lcd.print(kWattHours, 2);
  }
  else if (kWattHours < 100)
  {
    lcd.print("kWh: ");              // Adjust spacing for (10 - 99.xxx) kilo WattHours reading
    lcd.print(kWattHours, 3);
  }
  else if (kWattHours < 1000)
  {
    lcd.print("kWh:");              // Adjust spacing for (100 - 999.xxx) kilo WattHours reading
    lcd.print(kWattHours, 3);
  }
  else
  {
    lcd.print("kWh: ");              // Adjust spacing for > 1,000) kilo WattHours reading
    lcd.print(kWattHours, 2);           // Display kWh to 2 decimals
  }

}

/////////////////////////  End of dispWh ////////////////////////////////////




///////////////////////// Start of dispImax ------------------------------------

void dispImax()                 // Set cursor position before calling
// uses 9 Chars
{
  lcd.print("Imx: ");
  lcd.print(Imax, 2);           // Imax displayed to 2 decimal place
}


/////////////////////////  End of dispImax ////////////////////////////////////
