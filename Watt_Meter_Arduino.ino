#include <Adafruit_ADS1015.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 20 chars by 4 line display

Adafruit_ADS1115 ads(0x48);       // Create an instance of the ADC  (With ADDR line to GND, address is 0x48 - Default))
int16_t adc0, adc1, adc2, adc3;   // Setup ADC output variables - signed integers of exactly 16 bits

const byte OutRelay = 12;       // Relay control pin #
boolean Relay_On = false;

const byte LtBut = 10;          // Pin #'s for menu control Buttons
const byte RtBut = 11;

float Vrms = 0;
float Iave = 0;
float Imax = 0;
float kWh = 0;
float Watts = 0;
long RunTime = 0;             // figure out some sort of runtime counter in readable time
float Hz = 0;



void setup() {

  pinMode(OutRelay, OUTPUT);        // Set Load control (output power) relay control pin as output
  digitalWrite(OutRelay, LOW);      // Ensure the Relay is not energised at start

  pinMode(LtBut, INPUT_PULLUP); // Pin setup for menu control Buttons
  pinMode(RtBut, INPUT_PULLUP);

  lcd.init();             // initialize the lcd
  lcd.init();             // Not sure why it's done twice, but is this way in examples
  lcd.backlight();        // turn on backlight
  LCDHomeMenuLayout();    // setup initial static Display elements

  ads.begin();            // Initialize ads1115
}

void loop() {


}

void LCDHomeMenuLayout()
{
    // Data Display from Previous Test
  lcd.setCursor(0, 0);      // move cursor to first position (0) of the Top (0) Line
  lcd.print("V: XXX.xx");
  lcd.setCursor(10, 0);
  lcd.print("Iave XX.xx");
  lcd.setCursor(0, 1);
  lcd.print("kWh XXX.xx");
  lcd.setCursor(11, 1);
  lcd.print("Imax XX.x");

    // Next Run Info
  lcd.setCursor(0, 2);
  lcd.print("Next Run Timer: ");   // Follow with

    // Main Menu buttons
  lcd.setCursor(0, 3);
  lcd.print("START");
  lcd.setCursor(14, 3);
  lcd.print("ADJUST");

}
