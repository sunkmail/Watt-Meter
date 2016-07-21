
/*
   To Do:
        - Set-up menu system - Partially done
        - countdown timer implementation - Partially done
        - keep relay on after timer hits zero
        - Add I2C sensor input - Partially done
        - Add Power math calculations - Partially done
        - attach interrupt to PwrSense pin
        - BrownOut function from PwrSense interrupt
        - WDT
        - Voltage input (120/240) auto sense/calculation (from 120/240 selector switch)
*/

#define DEBUG true
#include <easyDebug.h>



#include <Adafruit_ADS1015.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <TimeDuration.h>        // Custom Library available at: http://github.com/sunkmail/TimeDuration/releases
#include <avr/pgmspace.h>

LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 20 chars by 4 line display

TimeDuration testTime;


//-------------------------------

Adafruit_ADS1115 ads(0x48);       // Create an instance of the ADC  (With ADDR line to GND, address is 0x48 - Default))
uint16_t adc0, adcVsenseRaw, adcAutoCalRaw, adcIsenseRaw;   // Setup ADC output variables - signed integers of exactly 16 bits
const byte VsenseChannel PROGMEM = 1;
const byte IsenseChannel PROGMEM = 3;
const byte autoCalChannel PROGMEM = 2;


//-------------------------------
const byte Relay_ON PROGMEM = 12;       // Relay control pin #  - Refers to 'Relay_ON' net name on sch.

const byte PwrSense PROGMEM = 13;       // Input pin for PwrSense signal - indicates if supply voltage lost.
// if this input goes LOW, no input power


////////-------------------
bool brownOut = false;          // Flag to show if last pwr loss (to whole cct) was during an active test

bool testRunning = false;       // Flag to indicate if the test is running on the load to be measured

bool keepRelayOn = false;       // Keep the load powered after countdown finished?
// (Pressing 'STOP' will ALWAYS power down the load)



////////----------------- Button Stuff ----------------------
const byte LtBut PROGMEM = 10;          // Pin #'s for menu control Buttons
const byte RtBut PROGMEM = 11;

const byte left PROGMEM = 1;
const byte right PROGMEM = 2;

byte butPress = 0;              // Tri-state yte for passing button press info.
// 0 = None
// 1 = Left
// 2 = right
////////-----------------


//////////------ Timing Related Stuff --------------

//----------------------------------------------------------------------
const byte PROGMEM arraySize = 5;       // data array size
//----------------------------------------------------------------------
int runtimePlan[arraySize] = {0, 0, 0, 0, 0};   // Runtime data for next run, as an array - default to zero.
int runtimeNow[arraySize] = {0, 0, 0, 0, 0};    // Runtime to use for actual test.  Copied from above at start of test.

unsigned long msTimeRun = 0;
unsigned long msTimePlan = 0;     // ms to run the test - Set default to 0 == Run until STOP pressed

String lastTime;                // Current/Previous Run's Runtime - in Human Readable format
String nextTime;                // For next Run's Runtime

const String StTimeMode[] PROGMEM  = {"Until Stopped"," Countdown"};
byte timeMode = 0;        // 0 = Until Stopped, 1 = Countdown

//---------------- Power Result Variables--------------------------------
const byte PROGMEM SampleSize = 50;
unsigned long TotalSamples = 0;
unsigned long sampleTime = 0;       // ms after start the last sample was taken

const int IZeroingValue PROGMEM = 16383;         ////////////  Half of signed 16bit - need to be fine tuned??
float Vrms = 0.0;
float Vworking[SampleSize];
float Iave = 0.0;
float Iworking[SampleSize];
float Imax = 0.0;
float WattHours = 0.0;
const unsigned long OneHour PROGMEM = 3600000;   // ms in an hour
//float kWattHours = 0;         // Moved inside function
float Watts = 0.0;
float Hz = 0.0;
//------------------------------------------------------------------------

////////------ Menu System Related -------------

byte menuLevel = 0;
byte menuSubLevel = 0;

////////------------------
// Menu nav options
const byte PROGMEM run = 0;    // Run Test
const byte PROGMEM next = 1;    // Next Menu
const byte PROGMEM adj = 2;   // Adjust valus -- go to first sub menu item
const byte PROGMEM root = 3;    // Go back to Root menu page
const byte PROGMEM nextSub = 4;    // Go to nex sub menu
const byte PROGMEM none = 5;    // no button function / Stay in same menu
const byte PROGMEM prevSub = 6;    // go back 1 sub menu level

const byte PROGMEM err = 255;   // non valid value - reset settings & return to root
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
  ads.setGain(GAIN_TWOTHIRDS);  // Set gain at 2/3x gain +/- 6.144V, 0.1875mV/bit - Don't exceed Vin (5V)
  
  nextTime = testTime.showTime(runtimePlan, 'd', 's');
  msTimePlan = testTime.makeTime(runtimePlan);

  DEBUG_Begin(9600);    //only start if in DEBUG mode
  DBprintln("setup done");

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

    DBprint("Button Pressed: ");       
    DBprintln(_butPress);  
  return _butPress;
}
///////////////////////////// End of checkButtons /////////////////////////////








  //------------------------ Sart of BrownOut ------------------------------------

void BrownOut()
  {
    // Store variables to memory


  }
  ///////////////////////////// End of BrownOut /////////////////////////////

