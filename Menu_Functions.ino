/*
  Menu Developed for a 2 button menu and setting system.
  - Used around a 20x04 LCD
  - Buttons are Left & Right
  - Bottom line of display will show dynamic button func. names
    - use 'menuButtons("left button name", "right button name");'
  - No active tests performed during menu selection
    - just to display and change varibles for running test later
    - delay() ok to use
*/







//------------------------ Start of menuNav  ---------------------------------

void menuNav(byte _leftAction, byte _rightAction)
{
  byte _action;

  if (butPress == left)
    _action = _leftAction;    // if left pressed, do the left action
  else if (butPress == right)
    _action = _rightAction;   // if right pressed, do the right action
  else
    _action = err;      // return error mode in case of problem


  switch (_action)
  {
    case (run):       // Run Test
      {
        testRunning = true; // Set flag to run test
        menuLevel = 0;    // reset menu levels
        menuSubLevel = 0;
        break;
      }

    case (next):   // Next Mneu
      {
        menuLevel++;
        menuSubLevel = 0;
        break;
      }
    case (adj):
      {
        menuSubLevel++;
        break;
      }

    case (root):
      {
        menuLevel = 0;    // reset menu levels
        menuSubLevel = 0;
        break;

      }

    case (nextSub):
      {
        menuSubLevel++;
        break;
      }

    case (none):
      {
        break;    // does nothing - keeps everything the same
      }

    case (prevSub):

      {
        menuSubLevel--;
        break;
      }

    default:    // case(err):   // also handles roll-over on MenuLevel
      {
        testRunning == false; // Error, reset everything back to main menu      menuLevel = 0;
        menuLevel = 0;    // reset menu levels
        menuSubLevel = 0;
        break;

      }
  }
  butPress = 0; // clear button press value
}
///////////////////////////// End of menuNav /////////////////////////////





//------------------------ menuRoot ------------------------------------
//  Display info only - nothing active.  Button check to Start in main program Loop.

void menuRoot(void)
{
  lcd.clear();
  // Data Display from Previous Test

  //     Top Row

  lcd.setCursor(4, 0);      // move cursor to first position (0) of the Top (0) Line
  lcd.print("AC Watt Meter");

  //      2nd Row
  lcd.setCursor(0, 1);               // move cursor to first position (0) of the second (1) Line
  lcd.print("Mode: ");
  lcd.print(StTimeMode[timeMode]);


  //------------ 3rd Row ------------
  //  lcd.setCursor(0, 2);
  //  nextTime = testTime.showTime(runtimePlan, 'd', 's');

  DBprint("timeMode: ");
  DBprint(timeMode);
  DBprint(" ");
  DBprintln(StTimeMode[timeMode]);

  if (timeMode != 0)         // If not counting up until stopped, show planned time.
    {
      lcd.setCursor(0, 2);
      nextTime = testTime.showTime(runtimePlan, 'd', 's');
      lcd.print(nextTime);      // Next planned runtime isn't zero
    }

  menuButtons("START", "NEXT");

  delay(200);

  butPress = checkButtons(true, true);   // Wait for  button press

  menuNav(run, next); // Nav, Where to go, pass what to go if left or right pressed

}
/////////////////////////  End of menuRoot ////////////////////////////////////




//------------------------------------ Start of menu2_Hist ------------------------------------
//             Details of last run

void menu2_Hist()
{
  delay(200);       // cheap debounce from button being released

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

  menuButtons("HOME", "NEXT");

  butPress = checkButtons(true, true);   // Wait for either button press

  menuNav(root, next); // Nav, Where to go, pass what to go if left or right pressed

}
/////////////////////////  End of menu2_Hist ////////////////////////////////////




//------------------------ Start of menu3_SetTimer  ---------------------------------

void menu3_SetTimer()
{
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Mode: ");
  lcd.print(StTimeMode[timeMode]);

  if(timeMode != 0)           // if not 'until Stopped', show next run timer
  {
    lcd.setCursor(0, 1);      // move cursor to first position (0) of the 2nd (1) Line
    lcd.print("Next Run Time: ");

    lcd.setCursor(4, 2);      // move cursor to first position (0) of the 2nd (1) Line
    lcd.print(nextTime);
  }
  menuButtons("CHANGE MODE", "NEXT");

  delay(200);

  butPress = checkButtons(true, true);


  ////////////////////////////////////////put into seperate routine & menu page
  if (butPress == left)
  {
    DBprintln("Enter timeModeSet()");
    /*    timeMode++;
        if (timeMode > 1)
          timeMode = 0;
        DBprintln("ModeChanged");
    */
    timerModeSet();
    DBprintln("Exit From timeModeSet()");
  }
  menuNav(none, nextSub); // Nav, Where to go, pass what to go if left or right pressed

}
///////////////////////////// End of menu3_SetTimer /////////////////////////////



//------------------------ Start of menu3_1_SetTime  ---------------------------------

void menu3_1_SetTime()
{
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Mode: ");
  lcd.print(StTimeMode[timeMode]);

  lcd.setCursor(0, 1);      // move cursor to first position (0) of the 2nd (1) Line
  lcd.print("Next Run Time: ");

  lcd.setCursor(5, 2);      // move cursor to first position (0) of the 2nd (1) Line
  lcd.print(nextTime);

  menuButtons("ADJUST TIME", "NEXT");

  delay(200);

  butPress = checkButtons(true, true);   // Wait for  button press


  if (butPress == left)
  {
    DBprintln("Start adjustPlanTime()");
    adjustPlanTime();
    DBprintln("Exit from adjustPlanTime()");
  }
  else
    menuNav(none, next);      // If no longer adjusting time, move to next menu


}
///////////////////////////// End of menu3_1_SetTime /////////////////////////////



//------------------------ Start of menu4_SetOpt  ---------------------------------

void menu4_SetOpt()
{
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Options Menu");

  //------------  Menu buttons ------------
//  lcd.setCursor(0, 3);
//  lcd.print("ADJUST");             //To Return to main (Home) menu
//  lcd.setCursor(16, 3);
//  lcd.print("NEXT");

  menuButtons("ADJUST", "NEXT");

  delay(200);

  butPress = checkButtons(true, true);

  menuNav(adj, next); // Nav, Where to go, pass what to go if left or right pressed


}
///////////////////////////// End of menu4_SetOpt /////////////////////////////




//------------------------ Start of menu4_1_SetOpt  ---------------------------------

void menu4_1_SetOpt()
{
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Adjust Options");


  //------------ Menu buttons ------------
//  lcd.setCursor(0, 3);
//  lcd.print("Options");             //To Return to main previous menu
  menuButtons("PREV", " ");

  delay(200);

  butPress = checkButtons(true, false);   // Wait for  button press

  menuNav(prevSub, none); // Nav, Where to go, pass what to go if left or right pressed

}
/////////////////////////////// End of menu4-1_SetOpt /////////////////////////////



//------------------------ Start of adjustPlanTime ---------------------------------
void adjustPlanTime()
{
  byte i = 0;
  bool tens = true;

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Test Duration: ");

  lcd.setCursor(5, 1);
  lcd.print(nextTime);

  lcd.setCursor(0, 2);
  lcd.print("    (45 Days max.)");

  //------------ Menu buttons ------------
  menuButtons("Increment", "NEXT ");


  //  lcd.setCursor(5, 1);
  //lcd.blink();

  while (i < (arraySize - 1))
  {
    lcd.blink();
    if ((i == 0) && (tens == true))
      lcd.setCursor(5, 1);
    else if ((i == 0) && (tens == false))
      lcd.setCursor(6, 1);
    else if ((i == 1) && (tens == true))
      lcd.setCursor(8, 1);
    else if ((i == 1) && (tens == false))
      lcd.setCursor(9, 1);
    else if ((i == 2) && (tens == true))
      lcd.setCursor(11, 1);
    else if ((i == 2) && (tens == false))
      lcd.setCursor(12, 1);
    else if ((i == 3) && (tens == true))
      lcd.setCursor(14, 1);
    else if ((i == 3) && (tens == false))
      lcd.setCursor(15, 1);

    delay(200);

    butPress = checkButtons(true, true);   // Wait for  button press

    if ((butPress == left) && (tens == true))
    {
      //     runtimePlan[i] += 10;
      switch (i)
      {
        case (0):   // days
          if (runtimePlan[i] > 35)
          {
            runtimePlan[i] -= ((runtimePlan[i] / 10) * 10);
          }
          else
            runtimePlan[i] += 10;
          break;

        case (1):     // hours
          if (runtimePlan[i] > 14)
          {
            runtimePlan[i] -= ((runtimePlan[i] / 10) * 10);
          }
          else
            runtimePlan[i] += 10;
          break;

        case (2):     // mins
        case (3):     // secs
          if (runtimePlan[i] >= 50)
          {
            runtimePlan[i] -= 50;
          }
          else
            runtimePlan[i] += 10;
          break;

      }  // End Switch


    }
    else if ((butPress == left) && (tens == false))
    {
      //     runtimePlan[i] += 1;

      switch (i)
      {
        case (0):
          if (runtimePlan[i] >= 45)
          {
            //          runtimePlan[i] = ((runtimePlan[i] /= 10)*10);
            runtimePlan[i] = 40;
          }
          else if ((runtimePlan[i] == 39) || (runtimePlan[i] == 29) || (runtimePlan[i] == 19) || (runtimePlan[i] == 9))
            runtimePlan[i] -= 9;
          else
            runtimePlan[i] += 1;
          break;

        case (1):
          if (runtimePlan[i] >= 24)
          {
            //          runtimePlan[i] = ((runtimePlan[i] /= 10)*10);
            runtimePlan[i] = 20;
          }
          else if ((runtimePlan[i] == 19) || (runtimePlan[i] == 9))
            runtimePlan[i] -= 9;
          else
            runtimePlan[i] += 1;
          break;
        case (2):     // mins
        case (3):     // secs
          if ((runtimePlan[i] == 59) || (runtimePlan[i] == 49) || (runtimePlan[i] == 39) || (runtimePlan[i] == 29) || (runtimePlan[i] == 19) || (runtimePlan[i] == 9))
          {
            runtimePlan[i] -= 9;
          }
          else
            runtimePlan[i] += 1;
          break;

      }  // end switch


    }
    else if ((butPress == right) && (tens == true))
    {
      tens = !tens;   // swap between x10's & x1's digit
      Serial.println("move to next coloumn");
    }
    else if ((butPress == right) && (tens == false) && (i >= (arraySize - 1))) // If done adjusting seconds x1, go to prev menu
    {
      Serial.println("hit 'Next' on seconds x1");
      menuNav(none, prevSub); // Nav, Where to go, pass what to go if left or right pressed
    }
    else
    {
      tens = !tens;   // swap between x10's & x1's digit
      i++;         // move to next column
      Serial.println("move to next value");
    }

    nextTime = testTime.showTime(runtimePlan, 'd', 's');
    lcd.noBlink();

    //    if ((butPress == right) && (tens == false) && (i == (arraySize-1)))   // If done adjusting seconds x1, go to prev menu
    //      menuNav(none, prevSub); // Nav, Where to go, pass what to go if left or right pressed

    lcd.setCursor(5, 1);
    lcd.print(nextTime);
  }   // End While Loop



  // butPress = checkButtons(true, true);   // Wait for  button press

  // menuNav(none, prev); // Nav, Where to go, pass what to go if left or right pressed
}
/////////////////////////////// End of adjustPlanTime /////////////////////////////


//------------------------ Start of timeModeSet ---------------------------------
void timerModeSet()
{
  timeMode++;                 // Set next mode
  if (timeMode > 1)
    timeMode = 0;           // If past # of options, go back to default
  DBprint("ModeChanged to: ");
  DBprintln(timeMode);
}
/////////////////////////////// End of timeModeSet /////////////////////////////
