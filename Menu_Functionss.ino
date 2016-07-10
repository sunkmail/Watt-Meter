/*
Menu Developed for a 2 button menu and setting system.
  - Used around a 20x04 LCD
  - Buttons are Left & Right
  - Bottom line of display will show dynamic button func. names
  - No active tests performed during menu selection
    - just to display and change varibles for running test later
    - delay() ok to use
*/

//------------------------ Start of menuNav  ---------------------------------

void menuNav(byte _leftAction, byte _rightAction)
{
  byte _action;
  
  if(butPress == left)
    _action = _leftAction;    // if left pressed, do the left action
  else if(butPress == right)
    _action = _rightAction;   // if right pressed, do the right action
  else 
    _action = err;      // return error mode in case of problem
  
  
  switch(_action)
  {
    case(run):        // Run Test
      {
        testRunning = true; // Set flag to run test
        menuLevel = 0;    // reset menu levels
        menuSubLevel = 0;
        break;
      }
      
    case(next):    // Next Mneu
      {
        menuLevel++;
        break;
      }
    case(adj):
      {
        menuSubLevel++;
        break;
      }
      
    case(root):
      {
        menuLevel = 0;    // reset menu levels
        menuSubLevel = 0;
        break;
        
      }
      
    case(nextSub):
      {
        menuSubLevel++;
        break;
      }
      
    case(none):
      {
        break;    // does nothing - keeps everything the same
      }
      
    case(prev):
      
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
} 
///////////////////////////// End of menuNav /////////////////////////////








//------------------------ menuRoot ------------------------------------
//  Display info only - nothing active.  Button check to Start in main program Loop.

void menuRoot(void)
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
  if (msTimePlan == 0)            // if planned # of ms is zero
    lcd.print("'til Stop'd");
  else
    lcd.print(nextTime);      // Next planned runtime isn't zero

  //------------ Main Menu buttons ------------
  lcd.setCursor(0, 3);
  lcd.print("START");             //To Start the Test
  lcd.setCursor(16, 3);
  lcd.print("Next");              // To go to next menu page

  delay(200);

  butPress = checkButtons(true,true);    // Wait for  button press
  
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


  //------------ 2nd Menu buttons ------------
  lcd.setCursor(0, 3);
  lcd.print("HOME");             //To Return to main (Home) menu
  lcd.setCursor(16, 3);
  lcd.print("Next");             // To go to next menu page - 'Set-up'
  //  }

  butPress = checkButtons(true,true);    // Wait for either button press
  
  menuNav(root, next); // Nav, Where to go, pass what to go if left or right pressed
  
}
/////////////////////////  End of menu2_Hist ////////////////////////////////////




//------------------------ Start of menu3_SetTimer  ---------------------------------

void menu3_SetTimer()
{
  lcd.clear();
  
  lcd.setCursor(0, 0);
  lcd.print("Timer Mode: Run 'till Stopped");

  lcd.setCursor(0,1);       // move cursor to first position (0) of the 2nd (1) Line
  lcd.print("Next Run Time: TBD");
  
  //------------ 2nd Menu buttons ------------
  lcd.setCursor(0, 3);
  lcd.print("ADJUST");             //To Return to main (Home) menu
  lcd.setCursor(16, 3);
  lcd.print("NEXT");  

  delay(200);
  
  butPress = checkButtons(true, true);
  
  menuNav(adj, next); // Nav, Where to go, pass what to go if left or right pressed
  
}
///////////////////////////// End of menu3_SetTimer /////////////////////////////




//------------------------ Start of menu3_1_SetTime  ---------------------------------

void menu3_1_SetTime()
{
  lcd.clear();
  
  lcd.setCursor(0, 0);
  lcd.print("adjust timer functions");

  
  //------------ 2nd Menu buttons ------------
  lcd.setCursor(0, 3);
  lcd.print("Prev");             //To Return to main previous menu
  
  delay(200);
  
  butPress = checkButtons(true,false);    // Wait for  button press
  
  menuNav(prev, none); // Nav, Where to go, pass what to go if left or right pressed
  
}
///////////////////////////// End of menu3_1_SetTime /////////////////////////////



//------------------------ Start of menu4_SetOpt  ---------------------------------

void menu4_SetOpt()
{
  lcd.clear();
  
  lcd.setCursor(0, 0);
  lcd.print("Option Setting Menu");

//------------  Menu buttons ------------
  lcd.setCursor(0, 3);
  lcd.print("ADJUST");             //To Return to main (Home) menu
  lcd.setCursor(16, 3);
  lcd.print("NEXT");  

  delay(200);
  
  butPress = checkButtons(true,true);
  
  menuNav(adj, next); // Nav, Where to go, pass what to go if left or right pressed
  
  
}
///////////////////////////// End of menu4_SetOpt /////////////////////////////  




//------------------------ Start of menu4_1_SetOpt  ---------------------------------

void menu4_1_SetOpt()
{
  lcd.clear();
  
  lcd.setCursor(0, 0);
  lcd.print("adjust option functions");

  
  //------------ Menu buttons ------------
  lcd.setCursor(0, 3);
  lcd.print("Prev");             //To Return to main previous menu
  
  delay(200);
    
  butPress = checkButtons(true,false);    // Wait for  button press
  
  menuNav(prev, none); // Nav, Where to go, pass what to go if left or right pressed
  
}
/////////////////////////////// End of menu4-1_SetOpt /////////////////////////////

