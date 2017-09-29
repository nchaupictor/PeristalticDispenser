/*
 Programming arduino mini
 -> 5 connection wires are required (hook up to an arduino uno -without the chip)
 1. 5V 
 2. GND
 3. TX - TX
 4. RX- RX
 5. RST- RST

 -> Choose board : Arduino Duemilanove or D... 
 -> if uploading fail, just unplug and replug usb connection from PC. It will work eveytime after- don't ask why.
 
  The circuit:
 * LCD RS pin to digital pin 7
 * LCD Enable pin to digital pin 8
 * LCD D4 pin to digital pin 9
 * LCD D5 pin to digital pin 10
 * LCD D6 pin to digital pin 11
 * LCD D7 pin to digital pin 12
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V

 */

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

// Declare pins
const int setVolumepin = 14; // use this for an analog input pin for volume dispense
const int setSpeedpin = 16  ; // use this for an analog input pin for speed of motor
const int setResetpin = 17; // use this for an analog input pin for set/reset button
const int eStoppin= A4; // e-stop pin that triggers reset function
const int LED= 4; // LED pin output
const int dispensePin= 15; //Dispense button
const int stepPin=5; //steppin pin for motor driver
const int dirPin= 6; //dir pin for motor driver
const int enablePin=3; //enable pin for motor driver

//Global Variables
int desiredSpeed=0;
int desiredVolume=0;
int eStopValue;
int delayValues[3] = {2000, 1200,600};
int fastPulses = 920; //925
int slowPulses= 975;
//int setResetValue=0;
String motorSpeed_lcd[3]= {"Slow", "Medium", "Fast"};
bool speed_fast;
String volumeDispense_lcd[6] = {"1" , "2" , "4" , "6" , "7" , "12"}; 
void setup() {
  // set up the LCD's number of columns and rows:
   digitalWrite( enablePin, HIGH); // set enable pin to high to disable motor driver
  digitalWrite(LED,LOW); //Set LED to low
  lcd.begin(16, 2);
  //Program Initialisation
  initialiseLED();
  //Set up pin
  pinMode( setResetpin, INPUT_PULLUP);
  pinMode( dispensePin, INPUT_PULLUP);
  pinMode (eStoppin, INPUT_PULLUP);
  pinMode( setVolumepin, INPUT);
  pinMode( setSpeedpin, INPUT);
  pinMode( LED, OUTPUT);
  pinMode( stepPin, OUTPUT);
  pinMode( dirPin, OUTPUT);
  pinMode( enablePin, OUTPUT);
  
 

  //check to ensure reset button is untriggered
   eStopValue = digitalRead(eStoppin);

  while (eStopValue==0)
  {
    //reset E-stop button before moving on
     eStopSetLED(false);
     eStopValue = digitalRead(eStoppin);
     delay(1000);
  }
 
}

void loop() {
 
  //Display enter variables LCD
  promptUSerLED();
  Settings:
 //Set desired speed and volume
  setSettings();
  //while reset button is not pressed, allow dispense to occur.
  int setResetValue= analogRead(setResetpin);
  int dispenseValue= 1;// set as high
 
  while (setResetValue>1000)
  {
   //display dispense
    dispenseLED(false);
    dispenseValue= digitalRead(dispensePin);
    //check for estop
      eStopValue= digitalRead(eStoppin);
      if (eStopValue==0)
      {
        eStopSetLED(true);
      }
      
    //Start dispensing when button is pressed
    if (dispenseValue==0)
    {
     
      activateMotor(desiredSpeed,desiredVolume);
      
      //check for estop
      eStopValue= digitalRead(eStoppin);
      if (eStopValue==0)
      {
        eStopSetLED(true);
      }
      //call motor function - start and dispense
     
    }
  
    setResetValue= analogRead(setResetpin);
    
  }
  //turn led off - indicate user wants to set settings
  digitalWrite(LED,LOW);



}

void initialiseLED (){
  lcd.setCursor(0,0);
  lcd.print("Welcome User!");
  delay(2500);
}

void promptUSerLED (){
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,0);
  lcd.print("Please enter");
  lcd.setCursor(0,1);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("Speed & Volume");
  delay(3000);
}

void displaySettings() {
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,0);
  lcd.print("Speed:");
  lcd.setCursor(0,1);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("Volume:");
  delay(1500);
}

void setSettings(){
  displaySettings();
  int setInput= digitalRead(setResetpin);
  int volumeDispense= 0;
  int motorSpeed= 0;
  //wait until the set button is pushed.
  while (setInput==1)
  {
    setInput= digitalRead(setResetpin);
    volumeDispense=  (analogRead(setVolumepin))/170.5   ;
    motorSpeed=  (analogRead(setSpeedpin)) / 341  ;
  
      if (volumeDispense == 6)
      {
        volumeDispense= 5;
      }
    
      if (motorSpeed == 3)
      {
        motorSpeed==2;
      }
      
    delayMicroseconds(300);
      lcd.setCursor(7,0);
      lcd.print("      ");
      lcd.setCursor(7,0);
      lcd.print( motorSpeed_lcd[motorSpeed]);
      lcd.setCursor(7,1);
      lcd.print("     ");
      lcd.setCursor(7,1);
      lcd.print(volumeDispense_lcd[volumeDispense]);
       lcd.setCursor(10,1);
      lcd.print("ml");
     
  }
  // Store values to global varialbe 
  desiredSpeed=motorSpeed;
  desiredVolume=volumeDispense_lcd[volumeDispense].toInt();
    if (desiredSpeed==2)
    {
      speed_fast==true;
    }
    else
    {
      speed_fast==false;
    }
  //Turn led on to indicate set button is pushed.
  digitalWrite(LED,HIGH);
  displaySettingsCompleted();
  return;
}

void displaySettingsCompleted(){
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,0);
  lcd.print("Values Stored!");
  lcd.setCursor(0,1);
  lcd.print("                ");
  delay(3000);
}

void dispenseLED(boolean dispense){
  if (dispense == true)
  {
    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,0);
    lcd.print("Dispensing..");
    delay(100);
  }
  else
  {
    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,0);
    lcd.print("Start Dispense");
    delay(10);
  }
}

//Reset Function
void(* resetFunc) (void) = 0; //declare reset function @ address 0

void eStopSetLED(boolean restart)
{
  if (restart==true)
  {
    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,0);
    lcd.print("Estop Activated");
    lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("Restarting..");
    delay(700);
     resetFunc();
  }
  else
  {
     lcd.setCursor(0,0);
     lcd.print("                ");
     lcd.setCursor(0,0);
     lcd.print("Please reset");
     lcd.setCursor(0,1);
     lcd.print("                ");
     lcd.setCursor(0,1);
     lcd.print("E-stop button.");
  }
}


void activateMotor(int motorSpeed, int volumeDispense)
{
  int vol = 0;
  digitalWrite( enablePin, LOW);// set low to enable board
  //Display on LED scree that it's dispensing
  dispenseLED(true);
  motorSpeed= delayValues[motorSpeed];
  if (speed_fast==true)
  {
    volumeDispense= volumeDispense*fastPulses;
  }
  else
  {
    volumeDispense= volumeDispense*slowPulses;
  }

  digitalWrite(dirPin, LOW);
  for (int x =0; x < volumeDispense; x++)
  {
    
    digitalWrite(stepPin,HIGH);
    delayMicroseconds(motorSpeed); // 
    digitalWrite(stepPin,LOW);
    delayMicroseconds(motorSpeed); //

    //check for estop
      eStopValue= digitalRead(eStoppin);
      if (eStopValue==0)
      {
        eStopSetLED(true);
      }
  }

}


