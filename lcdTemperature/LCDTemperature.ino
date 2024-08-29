/*
 * LCD Temperature
 * 11/07/2014
 * Renato M. Marco - rmarco@ciandt.com
 * Leandro Vendramin - lvendramin@ciandt.com
 *
 * Using Arduino UNO, this code will read a temperature sensor
 * and emits some sound, show a color on a RGB Led and show the 
 * name of this color on the LCD display.
 *
 * The intensity of the Buzz and the color shown will depend on
 * the temperature read and threshold set throughout buttons 
 * MINUS and PLUS that increase the threshold 1 degree at a time.
 *
 * Project developed to CI&T 3D Printer Project into The Garage
 */
 
#include <LiquidCrystal.h>

#define NOTE_1 147
#define NOTE_2 165
#define NOTE_3 175

// Pins map
char pinSensor 	 = A0;
char pinBTNMinus = 7;
char pinBTNPlus  = 8;
char pinBuzz 	 = A1;
char pinLED[3] 	 = {10, 9, 6};

// Colors to the RGB LED
int ledBlue[3] 	 = {0, 0, 255};
int ledGreen[3]  = {0, 255, 0};
int ledYellow[3] = {200, 230, 0};
int ledOrange[3] = {150, 150, 0};
int ledRed[3] 	 = {255, 0, 0};
int ledBlack[3]  = {0, 0, 0};

// Colors names
String colorBlue   	= "BLUE   ";
String colorGreen  	= "GREEN  ";
String colorYellow 	= "YELLOW ";
String colorOrange 	= "ORANGE ";
String colorRed    	= "RED    ";
String colorRedWarn	= "RED<!> ";
String colorNone   	= "   -   ";

//Global vars

// Sensor read temperature in celsius
int temperature = 0;

// Threshold
int tempThreshold = 23;

// Current color name
String color = colorNone;

// Control the note for the buzz
unsigned long noteTime = millis();
int currentNote = 0;

// Control the led color
unsigned long ledTime = millis();
int ledStatus = 0;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Default SETUP method
void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);

  // Sets the mode of each pin
  pinMode(pinSensor, INPUT);
  pinMode(pinBuzz, OUTPUT);
  pinMode(pinLED[0], OUTPUT);
  pinMode(pinLED[1], OUTPUT);
  pinMode(pinLED[2], OUTPUT);
  pinMode(pinBTNMinus, INPUT);
  pinMode(pinBTNPlus, INPUT); 
  
  // Show a presentation to the app
  showInitialization();
}

// Shows the presentation of the app, just for fun
void showInitialization() {
  // Puts the message "CI&T->THE GARAGE" on LCD
  lcd.print("CI&T->THE GARAGE");
  
  // Make it go from one line to another
  for (int i = 0; i < 6; i++) {
    lcd.clear();
    lcd.setCursor(0, i%2);
  	lcd.print("CI&T->THE GARAGE");
    delay(500);
  }
  
  // Wait and clear the LCD
  delay(200);
  lcd.clear();
  
  // Puts the names on the LCD
  String msg = "By Montanha";
  String msg2 = "& Vendramin";
  lcd.setCursor(1, 0);
  lcd.print(msg);
  lcd.setCursor(3, 1);
  lcd.print(msg2);
 
  // Insert a blinking cursor on LCD
  lcd.noBlink();
  delay(1000);
  lcd.blink();
  delay(1000);  
  
  // Make the text run to the left
  for(int i = 0; i < 14; i++) {
    lcd.scrollDisplayLeft();
    delay(150);    
  }  
  
  // Remove the blinking cursor and clear the LCD
  lcd.noBlink();
  lcd.clear();
  
  // The following laces are to show all the colors on the RGB LED
  for (int i = 0; i < 3; i++) {
    analogWrite(pinLED[i], ledBlue[i]);
  }
  delay(200);
  
  for (int i = 0; i < 3; i++) {
    analogWrite(pinLED[i], ledYellow[i]);
  }
  delay(200);
  
  for (int i = 0; i < 3; i++) {
    analogWrite(pinLED[i], ledOrange[i]);
  }
  delay(200);
  
  for (int i = 0; i < 3; i++) {
    analogWrite(pinLED[i], ledRed[i]);
  }
  delay(200);
  
  for (int i = 0; i < 3; i++) {
    analogWrite(pinLED[i], ledGreen[i]);
  }
  delay(200);
}

// Default method that executes in loop after setup
void loop() {  
  // Read the temperature
  readTemp();
  
  // Update the RGB Led
  updateLED();
  
  // Update the LCD info
  updateLCD();
  
  // Read and update the Threshold
  updateThreshold();
  
  // Update the buzz
  updateBUZZ();
}

// Read the temperature on LM35 sensor and 
// record on global var as celsius
void readTemp() {
  temperature = analogRead(pinSensor);
  temperature = temperature * 0.4882;
}

/**
 * Update the Led color according to the ranges
 */
void updateLED() {
  int delta = tempThreshold - temperature;
  
  if(delta <= 12 && delta > 8) {
    setColor(colorGreen);
    printLed(ledGreen);
  } else if(delta <= 8 && delta >= 5) {
    setColor(colorYellow);
    printLed(ledYellow);    
  } else if(delta <= 4 && delta >= 1) {
    setColor(colorOrange);
    printLed(ledOrange);    
  } else if(delta <= 0 && delta > -5) {
    setColor(colorRed);
    printLed(ledRed);
  } else if(delta <= -5) {
    
    long dt = millis() - ledTime;
    
    if (ledStatus == 0 && dt > 200) {
      setColor(colorRedWarn);
      printLed(ledRed);
      ledTime = millis();
      ledStatus = 1;
    } else if (ledStatus == 1 && dt > 200) {
      setColor(colorRed);
      printLed(ledBlack);            
      ledTime = millis();
      ledStatus = 0;
    }
    
  } else {
    setColor(colorBlue);
    printLed(ledBlue);
  } 
}

// Send the color to LED
void printLed(int color[]) {
  for (int i = 0; i < 3; i++) {
    analogWrite(pinLED[i], color[i]);
  }
}

// Update LCD info with temperature, color name and threshold
void updateLCD() {
  lcd.setCursor(0, 0);
  lcd.print("Current temp:");  
  lcd.setCursor(14, 0);
  lcd.print(temperature);

  lcd.setCursor(0, 1);
  lcd.print("Code:");
  lcd.setCursor(5, 1);
  lcd.print(color);

  lcd.setCursor(12, 1);
  lcd.print("(");
  lcd.setCursor(13, 1);
  lcd.print(tempThreshold);
  lcd.setCursor(15, 1);
  lcd.print(")");
}

// Set the current color name
void setColor(String c) {
  color = c;
}

// Checks if the buzz should emit or not sounds
void updateBUZZ() {
  if (temperature >= tempThreshold) {    
    long delta = millis() - noteTime;
    if (delta > 500 && currentNote == NOTE_1) {
      tone(pinBuzz, NOTE_2, 250);
      noteTime = millis();
      currentNote = NOTE_2;    
    } else if (delta > 500 && currentNote == NOTE_2) {
      tone(pinBuzz, NOTE_3, 250);      
      noteTime = millis();
      currentNote = NOTE_3;    
    } else if (delta > 500) {
      tone(pinBuzz, NOTE_1, 250);
      noteTime = millis();
      currentNote = NOTE_1;    
    }
  } else {
    analogWrite(pinBuzz, 0);     
  }
}

// Read the push buttons and update the threshold
void updateThreshold() {
  int minus = digitalRead(pinBTNMinus);
  int plus = digitalRead(pinBTNPlus);
  
  if (minus == HIGH) {
    tempThreshold--;
  }
  
  if (plus == HIGH) {
    tempThreshold++;  
  }
  
  delay(120);
} 

