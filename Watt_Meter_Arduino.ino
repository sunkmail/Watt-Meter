#include <Adafruit_ADS1015.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 20 chars and 4 line display

Adafruit_ADS1115 ads(0x48);       // Create an instance of the ADC  (With ADDR line to GND, address is 0x48 - Default))
int16_t adc0, adc1, adc2, adc3;   // Setup ADC output variables - signed integers of exactly 16 bits

const byte OutRelay = 12;       // Relay control pin #
boolean Relay_On = false;

const byte LtBut = 10;          // Pin #'s for menu control Buttons
const byte RtBut = 11;



void setup() {

  pinMode(OutRelay, OUTPUT);        // Set Load control (output power) relay control pin as output
  digitalWrite(OutRelay, LOW);      // Ensure the Relay is not energised at start

  pinMode(LtBut, INPUT_PULLUP); // Pin setup for menu control Buttons
  pinMode(RtBut, INPUT_PULLUP);
  
  lcd.init();             // initialize the lcd
  lcd.init();             // Not sure why it's done twice, but is this way in examples
  lcd.backlight();        // turn on backlight
  LCDLayout();            // setup static Display elements

  ads.begin();            // Initialize ads1115
}

void loop() {
  // put your main code here, to run repeatedly:

}

void LCDLayout()
{
  lcd.setCursor(0, 0);      // move cursor to first position (0) of the Top (0) Line
  lcd.print("Current Mode: ");
  lcd.setCursor(12, 0);
  lcd.print("Vrms:");
  lcd.setCursor(9, 1);
  lcd.print("Countdown: ");
  
}
