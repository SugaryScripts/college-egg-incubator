/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

/**
gnd = gnd
vcc = 5v
sda = gpio 4  or 21
scl = gpio 5  or 22
**/

#include <LiquidCrystal_I2C.h>

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  

void setup(){
  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();

  Serial.begin(9600);
  Serial.println("\nI2C Scanner");
}

void loop(){
  // set cursor to first column, first row
  lcd.setCursor(0, 0);
  // print message
  lcd.print("Hello, World!");
Serial.println("hey\n");

  delay(1000);
  // clears the display to print new message
  lcd.clear();
  // set cursor to first column, second row
  lcd.setCursor(0,1);
  lcd.print("Hello, another!");

Serial.println("done\n");

  delay(1000);
  lcd.clear(); 
}