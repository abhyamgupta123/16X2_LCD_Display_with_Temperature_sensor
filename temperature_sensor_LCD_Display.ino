/*
  LiquidCrystal Library - display() and noDisplay()

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints data received from temperature sensor 
 to the LCD and uses the temperature sensor DHT to get the data.
 

 The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 modified 7 Nov 2016
 by Arturo Guadalupi

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystalDisplay

*/


// include the library code:
#include <LiquidCrystal.h>

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 8     // Digital pin connected to the DHT sensor 
#define DHTTYPE    DHT11     // DHT 11


DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// make some custom characters:
//this is custom charachter of degree sign used with celcius notation.
byte degree[8] = {
0b00111,
0b00101,
0b00111,
0b00000,
0b00000,
0b00000,
0b00000,
0b00000
};

void setup() {
  //Initialised LCD Display
  lcd.begin(16, 2);
  
  //custom charachter loaded.
  lcd.createChar(0, degree);

  // lcd.home() makes the display to print from first row and first column.
  lcd.home();
  lcd.print("Initialised...");

  //DHT SENSOR SETUP
  Serial.begin(9600);
  
  // Initialize device.
  dht.begin();
  sensor_t sensor;
  // for twmpweature sensor details.
  dht.temperature().getSensor(&sensor);
  
  // for humidity sensor details.
  dht.humidity().getSensor(&sensor);
  
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;
    
}


void loop() {
  
  // Delay between measurements.
  delay(delayMS);

  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
    lcd.scrollDisplayLeft();
    delay(400);
    lcd.setCursor(0,0);
    lcd.print("Error reading temperature!");
    
  }
  else {
    Serial.print(F("Temperature : "));
    lcd.setCursor(0,0);
    lcd.print(F("Temp : "));
    Serial.print(event.temperature);
    lcd.cursor();
    lcd.print(event.temperature);
    Serial.println(F("°C"));
    
    lcd.cursor();
    lcd.write(byte(0));
    lcd.cursor();
    lcd.print(F("C  "));
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
    lcd.setCursor(0,1);
    lcd.print(F("Error reading humidity!"));
  }
  else {
    Serial.print(F("Humidity: "));
    lcd.setCursor(0,1);
    lcd.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    lcd.cursor();
    lcd.print(event.relative_humidity);
    Serial.println(F("%"));
    lcd.cursor();
    lcd.print(F("%"));
  }


  
}
