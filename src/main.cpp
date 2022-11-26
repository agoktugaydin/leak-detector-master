
// oled ve pio icin kutuphaneler dahil edildi
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>

// i2c kutuphanesi icin gerekli
#include <Wire.h>

// reset pini kullanilmiyor fakat i2c kutuphanesi icin gerekli
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

// pin and constant value assignees
const int analogIn = A0;
double RawValue = 0;
double Voltage = 0;
double rawSum = 0;
int limit = 1000;

void displayValues(){

  rawSum = analogRead(analogIn);
  analogReadResolution(12);

  // ornekleme 
  for (int i = 0; i < 500; i++){         
      rawSum += analogRead(analogIn);
  }

  RawValue = rawSum/500;
  Voltage = (RawValue / 4096.0) * 3300; // sensoru besleyen kaynagin voltaji onemli
  Serial.print("adc value = " );        // adc value
  Serial.print(RawValue);
  Serial.print("\n");
  Serial.print("mili volts = ");    // shows the voltage measured
  Serial.print(Voltage,0);
  Serial.print("\n");
  delay(300);

  display.clearDisplay();         // clear display
  display.setTextColor(WHITE);    //set color
  display.setTextSize(1);         //set font size
  display.setCursor(0,0);         //set cursor coordinates

  if (RawValue<limit){             // limitleme -degerle ilgilenilecek-
    display.print("NORMAL");
    display.setCursor(0,10); 
    display.print("raw value:"); 
    display.print(RawValue);
    display.print("\n");
  }
  else {
    display.print("LEAK");
    display.setCursor(0,10); 
    display.print("raw value:"); 
    display.print(RawValue);
    display.print("\n");
  }
}

void setup() {
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);   // oled i2c adress 0x3C
  Serial.begin(115200);
  delay(500);               // sensorun stabil hale gelmesi icin delay
}

void loop() {
  displayValues();
  display.display();
  delay(300);
}
