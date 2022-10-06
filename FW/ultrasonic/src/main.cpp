#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino NANO:       A4(SDA), A5(SCL)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define TRIG_PIN  9
#define ECHO_PIN 10
#define AVGSIZE 5

void printOnOLED(float dist);
float measureDistance(void);
float filteredDistance(uint8_t M);

float distance = 0.0f;
long durationTime = 0;



void setup()
{
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  // Clear the buffer
  display.clearDisplay();
  pinMode(TRIG_PIN,OUTPUT);
  pinMode(ECHO_PIN,INPUT);
}

void loop() 
{
  distance = measureDistance();
  // distance = filteredDistance(AVGSIZE);

  printOnOLED(distance);
  // Serial.print("Distance: ");
  Serial.println(distance);
  delay(100);
}

void printOnOLED(float dist)
{
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(20, 0);
  display.println("Distance");

  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0, 34);
  
  if(dist<100)
  {
    display.print(dist);
  display.println("cm");
  }
  else
  {
    display.print(dist/100);
  display.println("m");
  }

  display.display();
}

float measureDistance(void)
{
  digitalWrite(TRIG_PIN,LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN,HIGH);
  delayMicroseconds(8);
  digitalWrite(TRIG_PIN,LOW);

  durationTime = pulseIn(ECHO_PIN,HIGH);
  return durationTime*0.0342/2;
}

float filteredDistance(uint8_t M)
{
  float dist = 0;
  for(uint8_t i=0; i<M; i++)
  {
    dist += measureDistance();
    delayMicroseconds(100);
  }
  return dist/M;
}

