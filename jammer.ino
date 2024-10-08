#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);

int selected = 0;
int entered = -1;
int potPin = A0; // Pin for potentiometer
int lastPotValue = 0;
const int numOptions = 4;
int exxitt = 0;

void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  pinMode(7, INPUT_PULLUP); // Enter button
  
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(25, 15);
  display.println("315Hz");
  display.setTextSize(1);
  display.setCursor(85, 22);
  display.print("BOX");
  display.setTextSize(1);
  display.setCursor(25, 35);
  display.println("by Hue");
  display.display();
  delay(2000);
  lastPotValue = analogRead(potPin); // Initial potentiometer value
}

void loop() {
  displaymenu();
}

void displaymenu(void) {
  exxitt = 0;
  int enter = digitalRead(7);
  int potValue = analogRead(potPin); // scrolling with potentiometer
  int scaledValue = map(potValue, 0, 1023, 0, numOptions - 1);
  if (scaledValue != selected) {
    selected = scaledValue;
    delay(200);
  }
  
  if (enter == LOW) { // selected option
    entered = selected;
  }

  const char *options[4] = {
    " 6700  ",
    " 10000 ",
    " 1000  ",
    " Random"
  };

  if (entered == -2) {     // default while jammering
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(37, 9);
    display.println("JAMMERING");
    display.setCursor(25, 20);
    display.println("click to stop");
    display.setCursor(45, 54);
    display.println("xd");
    display.display();
    while (digitalRead(7) != LOW) {   // to go back to menu
      Serial.print("i"); // debugging
    } 
    delay(200);
    entered = -1;
    exxitt = 1;
    noTone(8);
  }

  if (entered == -1) {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Menu");
    display.setCursor(0, 20);
    Serial.println("x");

    for (int i = 0; i < 4; i++) {    
      if (i == selected) {
        display.setTextSize(1);
        display.print(">");
        display.println(options[i]);
      } else if (i != selected) {
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.println(options[i]);
      }
    }
   
  } else if (entered == 0 && exxitt == 0) {
    displayFrequencyProgress("6700", 6700);
   
  } else if (entered == 1 && exxitt == 0) {
    displayFrequencyProgress("10000", 10000);
   
  } else if (entered == 2 && exxitt == 0) {
    displayFrequencyProgress("1000", 1000);
   
  } else if (entered == 3 && exxitt == 0) {
    displayRandomFrequency();
  }
  display.display();
}

void displayFrequencyProgress(const char* frequencyLabel, int frequencyValue) {
  exxitt = 0;
  entered = -2; // Mark as entered to prevent re-entry
  for (int i = 0; i <= 100; i++) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("frequency");
    display.setCursor(102, 0);
    display.println(frequencyLabel);
    display.setCursor(0, 10);
    display.println("cycle");
    display.setCursor(120, 10);
    display.println("0");
    display.setCursor(0, 25);
    display.println("progress");
    display.setCursor(110, 25);
    display.print(i);
    display.println("%");
    display.drawRect(10, 40, 100, 15, WHITE);
    display.fillRect(12, 42, i * 0.95, 11, WHITE);
    display.display();
    delay(50);
  }  
  tone(8, frequencyValue);
}

void displayRandomFrequency() {
  entered = -2;  // Mark as entered to prevent re-entry
  for (int i = 0; i <= 100; i++) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("frequency");
    display.setCursor(92, 0);
    display.println("random");
    display.setCursor(0, 10);
    display.println("cycle");
    display.setCursor(120, 10);
    display.println("1");
    display.setCursor(0, 25);
    display.println("progress");
    display.setCursor(110, 25);
    display.print(i);
    display.println("%");
    display.drawRect(10, 40, 100, 15, WHITE);
    display.fillRect(12, 42, i * 0.95, 11, WHITE);
    display.display();
    delay(50);
  }
  
  int randomFrequency = random(1000, 10000);
  tone(8, randomFrequency);
}
