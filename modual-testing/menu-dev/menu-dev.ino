/*
Menu Developmet for a 2 button menu and setting system.
	- Used around a 20x04 LCD
	- Buttons are Left & Right
	- Bottom line of display will show dynamic button func. names
	- No active tests performed during menu selection
		- just to display and change varibles for running test later
		- delay() ok to use


*/

const byte LtBut = 10; 
const byte RtBut = 11;

const byte Relay_ON = 12;

////////-------------------
const byte left = 1;
const byte right = 2;

byte butPress = 0;              // Tri-state yte for passing button press info.
// 0 = None
// 1 = Left
// 2 = right
////////------------------

byte menuLevel = 0;
byte menuSubLevel = 0;

////////-------------------
// Menu nav options
const byte run = 0;		// Run Test
const byte next = 1;		// Next Menu
const byte adj = 2;		// Adjust valus -- go to first sub menu item
const byte home = 3;		// Go back to Root menu page
const byte nextSub = 4;    // Go to nex sub menu
const byte none = 5;		// no button function
const byte prev = 6;		// go back 1 sub menu level

const byte err = 255;		// non valid value - reset settings & return to root
////////-------------------


bool testRunning = false;       // Flag to indicate if the test is running on the load to be measured






void setup() 
{
	Serial.begin(9600);      // open the serial port at 9600 bps:
	
	
	pinMode(LtBut, INPUT_PULLUP); // Pin setup for menu control Buttons
	pinMode(RtBut, INPUT_PULLUP);
	
	
	
}

void loop() 
{
	//do{
	while(testRunning == false)
	{
		
		switch(menuLevel)
		{
			case(0):			// Home Menu
				{
					LCDRootMenu();
					break;
				}
				
			case(1):			// Last Run info
				{
					LCDLastRunMenu();
					break;
				}
				
			case(2):		//LCDSetTimeMenu
				{
					switch(menuSubLevel)
					{
						case(0):
							{
								LCDSetTimeMenu();
								break;
							}
							
						case(1):
							{
								LCDSetTime();
								break;
							}
					}
					
					break;   // End Sub Menu switch
				}
				
			case(3):  		//LCDSetOptMenu
				{
					switch(menuSubLevel)
					{
						case(0):
							{
								LCDSetOptMenu();
								break;
							}
							
						case(1):
							{
								LCDSetOpt();
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
		
		
	}    //while(testRunning == false);
	
	
	
	if(testRunning == true)
	{
		digitalWrite(Relay_ON, HIGH);               // Turn on the load
		
		butPress = 0;
		butPress = digitalRead(Relay_ON);	// Wait for  button press
		
		Serial.println();
		Serial.println();
		Serial.println();
		Serial.println();
		Serial.println("Functions to run Test");
		Serial.println();
		Serial.println();
		Serial.println("Rt = Stop Test");
		
		
		do
		{
			
			if(digitalRead(RtBut) == LOW)   // If STOP Pressed
			{
				delay(200);				// cheap debouce
				while(digitalRead(RtBut) == LOW) // wait for button release
				{
				}
				testRunning = false;
				digitalWrite(Relay_ON, LOW);	// Turn off load
				
			}
		} while(testRunning == true);
		
	}
	
	
}
///////////////////////////// End of loop /////////////////////////////



//------------------------ Start of LCDSetTime  ---------------------------------

void LCDSetTime()
{
	Serial.println();
	Serial.println();
	Serial.println();
	Serial.println();
	Serial.println("Function to set the time");
	Serial.println();
	Serial.println();
	Serial.println("Lt = Prev");
	
	butPress = checkButtons(true,false);		// Wait for  button press
	
	menuNav(prev, none); // Nav, Where to go, pass what to go if left or right pressed
	
}



///////////////////////////// End of LCDSetTime /////////////////////////////





//------------------------ Start of LCDRootMenu  ---------------------------------

void LCDRootMenu()
{
	Serial.println();
	Serial.println();
	Serial.println();
	Serial.println();
	Serial.println("Home (Root) Menu");
	Serial.println();
	Serial.println();
	Serial.println("Lt = Start       Rt = Next");
	
	butPress = checkButtons(true,true);		// Wait for  button press
	
	menuNav(run, next); // Nav, Where to go, pass what to go if left or right pressed
	
}
///////////////////////////// End of LCDRootMenu /////////////////////////////




//------------------------ Start of menuNav  ---------------------------------

void menuNav(byte _leftAction, byte _rightAction)
{
	byte _action;
	
	if(butPress == left)
		_action = _leftAction;		// if left pressed, do the left action
	else if(butPress == right)
		_action = _rightAction;		// if right pressed, do the right action
	else 
		_action = err;			// return error mode in case of problem
	
	
	switch(_action)
	{
		case(run):				// Run Test
			{
				testRunning = true;	// Set flag to run test
				menuLevel = 0;		// reset menu levels
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
			
		case(home):
			{
				menuLevel = 0;		// reset menu levels
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
				break;		// does nothing - keeps everything the same
			}
			
		case(prev):
			
			{
				menuSubLevel--;
				break;
			}
			
		default:		// case(err):   // also handles roll-over on MenuLevel
			{
				testRunning == false;	// Error, reset everything back to main menu			menuLevel = 0;		
				menuLevel = 0;		// reset menu levels
				menuSubLevel = 0;
				break;
				
			}
	}
}	
///////////////////////////// End of menuNav /////////////////////////////




//------------------------ Start of LCDLastRunMenu  ---------------------------------

void LCDLastRunMenu()
{
	Serial.println();
	Serial.println();
	Serial.println();
	Serial.println();
	Serial.println("Last Run Info");
	Serial.println();
	Serial.println();
	Serial.println("Lt = Home       Rt = Next");
	
	butPress = checkButtons(true, true);
	
	menuNav(home, next); // Nav, Where to go, pass what to go if left or right pressed
	
	
}
///////////////////////////// End of LCDLastRunMenu /////////////////////////////




//------------------------ Start of LCDSetTimeMenu  ---------------------------------

void LCDSetTimeMenu()
{
	Serial.println();
	Serial.println();
	Serial.println();
	Serial.println();
	Serial.println("Timer Setting Menu");
	Serial.println();
	Serial.println();
	Serial.println("Lt = ADJ       Rt = Next");
	
	butPress = checkButtons(true, true);
	
	menuNav(adj, next); // Nav, Where to go, pass what to go if left or right pressed
	
	
	
}
///////////////////////////// End of LCDSetTimeMenu /////////////////////////////




//------------------------ Start of LCDSetOptMenu  ---------------------------------

void LCDSetOptMenu()
{
	Serial.println();
	Serial.println();
	Serial.println();
	Serial.println();
	Serial.println("Option Setting Menu");
	Serial.println();
	Serial.println();
	Serial.println("Lt = ADJ       Rt = Next");
	
	butPress = checkButtons(true,true);
	
	menuNav(adj, next); // Nav, Where to go, pass what to go if left or right pressed
	
	
}
///////////////////////////// End of LCDSetOptMenu /////////////////////////////	




//------------------------ Start of checkButtons  ------------------------------------

byte checkButtons(bool _Lt, bool _Rt)
{
	byte _butPress = 0;
	
	do  // Stay in the Menu Loop until a button is pressed
	{
		if((digitalRead(LtBut) == LOW) && (_Lt == true))  // If Left menu button pressed
		{
			_butPress = left;
		}
		
		else if((digitalRead(RtBut) == LOW) && (_Rt == true))  // If Right menu button pressed
		{
			_butPress = right;
		}
		
		
	} while (_butPress == 0);     // Stay in the Menu Loop until a button is pressed
	// exit Do - While loop
	
	delay(100);  				// Debounce delay
	
	while((digitalRead(LtBut) == LOW) || (digitalRead(RtBut) == LOW))    // Do nothing until button released
	{
	}
	
	return _butPress;
}

///////////////////////////// End of checkButtons /////////////////////////////


//------------------------ Start of LCDSetOpt  ---------------------------------

void LCDSetOpt()
{
	Serial.println();
	Serial.println();
	Serial.println();
	Serial.println();
	Serial.println("Function to set the Options");
	Serial.println();
	Serial.println();
	Serial.println("Lt = Prev");
	
	butPress = checkButtons(true,false);		// Wait for  button press
	
	menuNav(prev, none); // Nav, Where to go, pass what to go if left or right pressed
	
}
/////////////////////////////// End of LCDSetOpt /////////////////////////////





