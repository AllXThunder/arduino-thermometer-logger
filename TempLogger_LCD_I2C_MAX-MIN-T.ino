#include <OneWire.h>
#include <Wire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_PCF8574.h>
#define ONE_WIRE_BUS 2
#define LED 13
#define BUTTON_PIN 3

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
LiquidCrystal_PCF8574 lcd(0x27);

unsigned long prevMillisSensors = 0;
unsigned long prevMillisBlink = 0;
const long sensorsInterval = 1000;
const long blinkingInterval = 500;

bool ledState = false;
bool isBlinking = false;
volatile bool buttonPressed = false;
int displayMode = 0;
bool lastButtonState = HIGH;

float tempCMin = 127.00;
float tempCMax = -127.00;


void setup() {
  sensors.begin();
  pinMode(LED, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handleButton, FALLING);

  lcd.begin(16, 2);
  lcd.setBacklight(50);
  lcd.clear();
  lcd.print("Loading...");
}

void handleButton() {
  buttonPressed = true;
}

void loop() {

  unsigned long currentMillis = millis();
  if (currentMillis - prevMillisSensors >= sensorsInterval) {
   prevMillisSensors = currentMillis;

  if (buttonPressed) {
    buttonPressed = false;
    displayMode++;
    if (displayMode > 1) displayMode = 0;
    lcd.clear();
  }
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);
  
  if (tempC < tempCMin) {
    tempCMin = tempC;
    } if (tempC > tempCMax) {
    tempCMax = tempC; 
    }
    lcd.setCursor(0, 0);
  if (displayMode == 0) {
    lcd.print("T: ");
    lcd.print(tempC);
    lcd.print(" C");
  } else if (displayMode == 1) {
    lcd.print ("MaxT: ");
    lcd.print (tempCMax);
    lcd.print (" C");
    lcd.setCursor (0, 1);
    lcd.print ("MinT: ");
    lcd.print (tempCMin);
    lcd.print (" C");
  }
  
  if (tempC <= 22.0) {
    isBlinking = true;
  } else if (tempC >= 30.0){
      isBlinking = false;
      digitalWrite(LED, HIGH);
      ledState = true;
  } else {
      isBlinking = false;
      digitalWrite(LED, LOW);
      ledState = false;
  }
      }
      if (isBlinking) {
    if (currentMillis - prevMillisBlink >= blinkingInterval) {
      prevMillisBlink = currentMillis;

      ledState = !ledState;
      digitalWrite(LED, ledState);
    }
    }
  }
