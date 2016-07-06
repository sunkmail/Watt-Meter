/*
 * To Do:
 *      - Set-up menu system - Partially done
 *      - attach interrupt to PwrSense pin
 *      - WDT
 * 
*/ 
#include <Adafruit_ADS1015.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <TimeDuration.h>        // Custom Library available at: http://github.com/sunkmail/TimeDuration/releases

LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 20 chars by 4 line display

TimeDuration testTime;

Adafruit_ADS1115 ads(0x48);       // Create an instance of the ADC  (With ADDR line to GND, address is 0x48 - Default))
int16_t adc0, adc1, adc2, adc3;   // Setup ADC output variables - signed integers of exactly 16 bits

const byte Relay_ON = 12;       // Relay control pin #  - Refers to 'Relay_ON' net name on sch.

const byte PwrSense = 13;       // Input pin for PwrSense signal - indicates if supply voltage lost.
                                // if this input goes LOW, no input power

bool brownOut = false;          // Flag to show if last pwr loss (to whole cct) was during an active test

bool testRunning = false;       // Flag to indicate if the test is running on the load to be measured

bool keepRelayOn = false;       // Keep the load powered after countdown finished?   
                                // (Pressing 'STOP' will ALWAYS power down the load)

const byte LtBut = 10;          // Pin #'s for menu control Buttons
const byte RtBut = 11;

const byte arraySize = 5;       // data array size
//----------------------------------------------------------------------
int runtimePlan[arraySize] = {0,0,0,0,0};       // Runtime data for next run, as an array - default to zero.
int runtimeNow[arraySize] = {0,0,0,0,0};        // Runtime to use for actual test.  Copied from above at start of test.

unsigned long timeRun = 0;
unsigned long timePlan = 0;     // ms to run the test - Set default to 0 == Run until STOP pressed

String lastTime;                // Current/Previous Run's Runtime - in Human Readable format
String nextTime;                // For next Run's Runtime


//------------------------------------------------------------------------ -

float Vrms = 0;
float Iave = 0;
float Imax = 0;
float WattHours = 0;
float Watts = 0;
float Hz = 0;



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

  ads.begin();            // Initialize ads1115
  nextTime = testTime.showTime(runtimePlan,'d','s');

  delay(20);              // Delay for settling
}

void loop() {   
  LCDHomeMenuLayout();    // Display Home Menu
  
//  nextTime = testTime.showTime(runtimePlan, 'd','s');
              // re calculate after change in menu
  
  do 
//  while(testRunning == false);     // Stay in the Menu Loop whenever the test is not active
  {
    if (digitalRead(LtBut) == LOW)  // If START pressed from Main menu...
    {
      testRunning = true;           // Flag to indicate the test is running on the load to be measured
    }

  } while (testRunning == false);     // Stay in the Menu Loop whenever the test is not active


  if(testRunning ==true)            // Run ONCE at start of test
  {
    for(byte i = 0; i <= (arraySize-1); i++)
    {
      runtimeNow[i] = runtimePlan[i];  // copy plan time into active time
    }
    
    LCDTestRunningSetup();                      // set static display elements for testing
    timeRun = testTime.makeTime(runtimeNow);   // Convert human readable Runtime into ms
    testTime.startOfTime();                     // Start duration timer
    digitalWrite(Relay_ON, HIGH);               // Turn on the load

  }

    
  do                                // Repeat while test running
  {
    
    PowerMeasuring();               // Measure the power data
    LCDRunningUpdate();             // Display latest data
    
  if(digitalRead(RtBut) == LOW)         // If STOP pressed
    {
      testRunning = false;                // Set Flag to exit Loop
      digitalWrite(Relay_ON, LOW);        // Turn off load
      timeRun = testTime.doingTime(runtimeNow);     // Take one last duration measurement
    }
  } while (testRunning ==true);

  
  lastTime = testTime.showTime(runtimeNow,'d','s');
}


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

//------------------------ End of LCDHomeMenuLayout ------------------------------------
}





//------------------------ LCDTestRunningSetup ------------------------------------
void LCDTestRunningSetup()
{
  lcd.clear();
  lcd.setCursor(16, 3);           
  lcd.print("Stop");              // Rt button action
  
  lcd.setCursor(0, 2);
  lcd.print("Runtime: ");
  
}
//------------------------ End of LCDTestRunning ------------------------------------





//------------------------ PowerMeasuring ------------------------------------

void PowerMeasuring(void)
{

}
//------------------------ End of PowerMeasuring ------------------------------------




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

