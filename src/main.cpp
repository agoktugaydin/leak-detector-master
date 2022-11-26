
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

void displayValues(){

rawSum = analogRead(analogIn);
analogReadResolution(12);

// ornekleme 
for (int i = 0; i < 500; i++){         
    rawSum += analogRead(analogIn);
}

RawValue = rawSum/500;
Voltage = (RawValue / 4096.0) * 3300;
Serial.print("Raw Value = " );        // adc value
Serial.print(RawValue);
Serial.print("\t milli volts = ");    // shows the voltage measured
Serial.print(Voltage,0);
delay(300);

}

void setup() {

}

void loop() {

}