
#include <Adafruit_ADS1015.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
// #include <TimeDuration.h>        // Custom Library available at: http://github.com/sunkmail/TimeDuration/releases

LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 20 chars by 4 line display

Adafruit_ADS1115 ADC(0x48);       // Create an instance of the ADC  (With ADDR line to GND, address is 0x48 - Default))
int16_t adc0, adc1, adc2, adc3;   // Setup ADC output variables - signed integers of exactly 16 bits

const byte OutRelay = 12;       // Relay control pin #
bool Relay_On = false;
bool keepRelayOn = false;       // Keep the load powered after finishing test?

const byte LtBut = 10;          // Pin #'s for menu control Buttons
const byte RtBut = 11;


//----------------------------------------------------------------------
int RuntimePlan[5];       // Runtime data for next run, as an array, using TimeDuration library

unsigned long TimeRun = 0;
unsigned long TimePlan = 0;     // Set default countdown timer to 0 == Run until STOP pressed

String lastTime;                // Previous Run's Runtime - in Human Readable format
String nextTime;                // For next Run's Runtime


//------------------------------------------------------------------------ -

float Vrms = 0;
float Iave = 0;
float Imax = 0;
float WattHours = 0;
float Watts = 0;
float Hz = 0;



void setup() {

  pinMode(OutRelay, OUTPUT);        // Set Load control (output power) relay control pin as output
  digitalWrite(OutRelay, LOW);      // Ensure the Relay is not energised at start

  pinMode(LtBut, INPUT_PULLUP); // Pin setup for menu control Buttons
  pinMode(RtBut, INPUT_PULLUP);

  lcd.init();             // initialize the lcd
  lcd.init();             // Not sure why it's done twice, but is this way in examples
  lcd.backlight();        // turn on backlight

  ADC.begin();            // Initialize ads1115

  delay(20);              // Delay for settling
}

void loop() {
  do
  {
    LCDHomeMenuLayout();    // Display Home Menu

    if (LtBut == LOW)         // If START pressed from Main menu...
    {
      Relay_On = true;      // Active relay to load to be measured

  
    }

  } while (Relay_On == false);

  do
  {
  PowerMeasuring();         // If not in the menu - measure the power
  
  } while (Relay_On ==true);
  
}


//------------------------------------------------------------


void LCDHomeMenuLayout(void)
{
  // Data Display from Previous Test

  //Vrms
  lcd.setCursor(0, 0);      // move cursor to first position (0) of the Top (0) Line
  if (Vrms >= 100)
    lcd.print("V: ");       //to display Vrms in the right spot if 2 leading digits
  else
    lcd.print("V:  ");      // Add extra space for 2 leading digit Vrms

  lcd.print(Vrms, 2);       // Display previous run Value w/ 2 decimal places

  // Iave
  lcd.setCursor(10, 0);     // move cursor to position 10 of the Top (0) Line
  lcd.print("Iave ");
  lcd.print(Iave, 2);

  // Watt Hours
  lcd.setCursor(0, 1);               // move cursor to first position (0) of the second (1) Line
  if (WattHours < 10)
  {
    lcd.print("Wh:    ");            // Adjust spacing for 1 leading digit (0 -9.xx) WattHours reading
    lcd.print(WattHours, 2);
  }
  else if (WattHours < 100)
  {
    lcd.print("Wh:   ");             // Adjust spacing for 2 leading digit (10 -99.xx) WattHours reading
    lcd.print(WattHours, 2);
  }
  else if (WattHours < 1000)
  {
    lcd.print("Wh:  ");              // Adjust spacing for 3 leading digit (100 -999.xx) WattHours reading
    lcd.print(WattHours, 3);
  }
  else
  {
    lcd.print("kWh ");              // Adjust for kWh vs. Wh WattHours reading
    lcd.setCursor(4, 1);            // move cursor to position 4 of the second (1) Line
    lcd.print((WattHours / 1000), 3); // Display kWh to 3 decimals
    // *****  Add code here to switch to 2 decimal places if kWh overruns next display section ***
  }

  //Imax
  lcd.setCursor(11, 1);
  lcd.print("Imax ");
  lcd.print(Imax, 1);           // Imax displayed to 1 decimal place


  // Next Run Info
  lcd.setCursor(0, 2);
  lcd.print("Next Run Timer: ");   // Follow with


  // Main Menu buttons
  lcd.setCursor(0, 3);
  lcd.print("START");
  lcd.setCursor(14, 3);
  lcd.print("ADJUST");


}

//  ---------------------------------------------------------------------- -

void PowerMeasuring(void)
{
  while (Relay_On == true)
  {
    digitalWrite(OutRelay, HIGH);         // Turn on the load, if not already on




    if (RtBut == LOW)                      // If STOP pressed
      Relay_On = false;                   // Set Flag to exit Loop
  }
}
