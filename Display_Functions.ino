//--------------------------- Start of dispVrms ------------------------------------

void dispVrms()             // Set cursor position before calling
// uses 8 display Chars, "V:xxX.XX"
{
  if (Vrms >= 100)
    lcd.print("Vav:");       //to display Vrms in the right spot if 2 leading digits

  else if (Vrms >= 10)
    lcd.print("Vav: ");      // Add extra space for 1 leading digit Vrms
  else
    lcd.print("Vav:  ");

  lcd.print(Vrms, 2);       // Display Vrms Value w/ 2 decimal places
}

/////////////////////////  End of dispVrms ////////////////////////////////////





//------------------------------------ Start of dispIave ------------------------------------

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

/////////////////////////  End of dispIave ////////////////////////////////////





//------------------------------------ Start of dispWh ------------------------------------

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




//----------------------- Start of dispImax ------------------------------------

void dispImax()                 // Set cursor position before calling
// uses 9 Chars
{
  lcd.print("Imx: ");
  lcd.print(Imax, 2);           // Imax displayed to 2 decimal place

}
/////////////////////////  End of dispImax ////////////////////////////////////




//----------------------- Start of menuButtons ------------------------------------

void menuButtons(String _Left, String _Right)     // uses Bottom Line 
{
  byte _length;
  lcd.setCursor(0, 3);
  lcd.print(_Left);    
  _length = _Right.length();        // determine # chars in string
  lcd.setCursor((20 - _length), 3); // Set cursor back that many spaces
  lcd.print(_Right);
}


/////////////////////////  End of menuButtons ////////////////////////////////////
