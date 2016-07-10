
/*
   To Do:
        - Set-up menu system - Partially done
        - countdown timer implementation
        - keep relay on after timer hits zero
        - Add I2C sensor input
        - Add Power math calculations
        - attach interrupt to PwrSense pin
        - BrownOut function from PwrSense interrupt
        - WDT



*/
#include <Adafruit_ADS1015.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <TimeDuration.h>        // Custom Library available at: http://github.com/sunkmail/TimeDuration/releases

LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 20 chars by 4 line display

TimeDuration testTime;


//-------------------------------

Adafruit_ADS1115 ads(0x48);       // Create an instance of the ADC  (With ADDR line to GND, address is 0x48 - Default))
int16_t adc0, adc1, adc2, adc3;   // Setup ADC output variables - signed integers of exactly 16 bits

//-------------------------------
const byte Relay_ON = 12;       // Relay control pin #  - Refers to 'Relay_ON' net name on sch.

const byte PwrSense = 13;       // Input pin for PwrSense signal - indicates if supply voltage lost.
// if this input goes LOW, no input power

////////-------------------
bool brownOut = false;          // Flag to show if last pwr loss (to whole cct) was during an active test

bool testRunning = false;       // Flag to indicate if the test is running on the load to be measured

bool keepRelayOn = false;       // Keep the load powered after countdown finished?
// (Pressing 'STOP' will ALWAYS power down the load)



////////----------------- Button Stuff ----------------------
const byte LtBut = 10;          // Pin #'s for menu control Buttons
const byte RtBut = 11;

const byte left = 1;
const byte right = 2;

byte butPress = 0;              // Tri-state yte for passing button press info.
// 0 = None
// 1 = Left
// 2 = right
////////-----------------


//////////------ Timing Related Stuff --------------

//----------------------------------------------------------------------
const byte arraySize = 5;       // data array size
//----------------------------------------------------------------------
int runtimePlan[arraySize] = {0, 0, 0, 0, 0};   // Runtime data for next run, as an array - default to zero.
int runtimeNow[arraySize] = {0, 0, 0, 0, 0};    // Runtime to use for actual test.  Copied from above at start of test.

unsigned long msTimeRun = 0;
unsigned long msTimePlan = 0;     // ms to run the test - Set default to 0 == Run until STOP pressed

String lastTime;                // Current/Previous Run's Runtime - in Human Readable format
String nextTime;                // For next Run's Runtime



//---------------- Power Result Variables--------------------------------
float Vrms = 0;
float Iave = 0;
float Imax = 0;
float WattHours = 0;
//float kWattHours = 0;         // Moved inside function
float Watts = 0;
float Hz = 0;
//------------------------------------------------------------------------

////////------ Menu System Related -------------

byte menuLevel = 0;
byte menuSubLevel = 0;

////////------------------
// Menu nav options
const byte run = 0;    // Run Test
const byte next = 1;    // Next Menu
const byte adj = 2;   // Adjust valus -- go to first sub menu item
const byte root = 3;    // Go back to Root menu page
const byte nextSub = 4;    // Go to nex sub menu
const byte none = 5;    // no button function
const byte prev = 6;    // go back 1 sub menu level

const byte err = 255;   // non valid value - reset settings & return to root
////////-------------------

void setup() {

  pinMode(Relay_ON, OUTPUT);        // Set Load control (output power) relay control pin as output
  digitalWrite(Relay_ON, LOW);      // Ensure the Relay is not energised at start

  pinMode(PwrSense, INPUT);         // Set PwrSense line as input
  //---------From "To Do"--------------- attach interrupt to PwrSense pin.


  pinMode(LtBut, INPUT_PULLUP); // Pin setup for menu control Buttons
  pinMode(RtBut, INPUT_PULLUP);

  lcd.init();             // initialize the lcd
  lcd.init();             // Not sure why it's done twice, but is this way in examples
  lcd.backlight();        // turn on backlight

  // Future update //
  // Pull previous settings & variables from Memory

  ads.begin();            // Initialize ads1115
  
  nextTime = testTime.showTime(runtimePlan, 'd', 's');
  msTimePlan = testTime.makeTime(runtimePlan);

  delay(20);              // Delay for settling

}



//------------------------ Start of loop  ------------------------------------

void loop()
{
  while (testRunning == false)
  {

    switch (menuLevel)
    {
      case (0):    // Home Menu
        {
          menuRoot();
          break;
        }

      case (1):    // Last Run info
        {
          menu2_Hist();
          break;
        }

      case (2):  //menu3_SetTimer
        {
          switch (menuSubLevel)
          {
            case (0):
              {
                menu3_SetTimer();
                break;
              }

            case (1):
              {
                menu3_1_SetTime();
                break;
              }
          }

          break;   // End Sub Menu switch
        }

      case (3):     //menu4_SetOpt
        {
          switch (menuSubLevel)
          {
            case (0):
              {
                menu4_SetOpt();
                break;
              }

            case (1):
              {
                menu4_1_SetOpt();
                break;
              }
          }

          break;   // End Sub Menu switch

        }

      default:
        {
          menuLevel = 0;
          menuSubLevel = 0;
          break;
        }

    }


  }    // end while(testRunning == false);

  // When testTunning != false, run the testing routines
  runTest();

}
/////////////////////////  End of Loop ////////////////////////////////////
  

//------------------------ Start of checkButtons  ------------------------------------

byte checkButtons(bool _Lt, bool _Rt)
{
  byte _butPress = 0;

  do  // Stay in the Menu Loop until a button is pressed
  {
    if ((digitalRead(LtBut) == LOW) && (_Lt == true)) // If Left menu button pressed
    {
      _butPress = left;
    }

    if ((digitalRead(RtBut) == LOW) && (_Rt == true)) // If Right menu button pressed
    {
      _butPress = right;
    }

  } while (_butPress == 0);     // Stay in the Menu Loop until a button is pressed

  delay(100);          // Debounce delay

  while ((digitalRead(LtBut) == LOW) || (digitalRead(RtBut) == LOW))
  {
    // Do nothing until all buttons released
  }

  return _butPress;
}
///////////////////////////// End of checkButtons /////////////////////////////






  //------------------------ Sart of BrownOut ------------------------------------

void BrownOut()
  {
    // Store variables to memory


  }
  ///////////////////////////// End of BrownOut /////////////////////////////

