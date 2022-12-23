
// oled ve pio icin kutuphaneler dahil edildi
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
//esp-now protokolu icin gerekli kutuphaneler
#include <esp_now.h>
#include <WiFi.h>
// i2c kutuphanesi icin gerekli
#include <Wire.h>

// reset pini kullanilmiyor fakat i2c kutuphanesi icin gerekli
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

// gonderilecek veri icin gerekli struct yapisi
// verici cihazdaki struct yapisi ile eslesmeli
typedef struct struct_message {
    int id; // verici cihaza ait unique id
    int x;
    int y;
} struct_message;

struct_message myData;

// gelen verileri ayri ayri tutmak icin gerekli olan struct yapisi
struct_message board1;
//struct_message board2;
//struct_message board3;

// kart arrayi, kart sayisi arttikca eklenebilir
struct_message boardsStruct[1] = {board1};

// veri geldiginde caliscak callback fonksiyonu
void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
  char macStr[18];
  Serial.print("Packet received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.printf("Board ID %u: %u bytes\n", myData.id, len);
  // struct yapisinin guncellenmesi
  boardsStruct[myData.id-1].x = myData.x;
  boardsStruct[myData.id-1].y = myData.y;
  Serial.printf("x value: %d \n", boardsStruct[myData.id-1].x);
  Serial.printf("y value: %d \n", boardsStruct[myData.id-1].y);
  Serial.println();
}

// pin and constant value assignees
const int analogIn = A0;
int rawValue = 0;
double voltage = 0;
double rawSum = 0;
int limit = 2000;

void displayValues(){

  rawSum = analogRead(analogIn);
  analogReadResolution(12);

  // ornekleme 
  for (int i = 0; i < 500; i++){         
      rawSum += analogRead(analogIn);
  }

  rawValue = rawSum/500;
  voltage = (rawValue / 4096.0) * 3300; // sensoru besleyen kaynagin voltaji onemli
  Serial.print("adc value = " );        // adc value
  Serial.print(rawValue);
  Serial.print("\n");
  Serial.print("mvolts = ");    // shows the voltage measured
  Serial.print(voltage,0);
  Serial.print("\n");
  delay(300);

  display.clearDisplay();         // clear display
  display.setTextColor(WHITE);    //set color
  display.setTextSize(1);         //set font size
  display.setCursor(0,0);         //set cursor coordinates

  if (rawValue<limit){             // limitleme -degerle ilgilenilecek-
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(18, LOW);
    display.print("NORMAL");
    display.setCursor(0,10); 
    display.print("raw value:"); 
    display.print(rawValue);
    display.print("\n");
    display.print("recieved value:");
    display.print(boardsStruct[0].x);
    display.print("\n");
    display.print(boardsStruct[0].y);
    display.print("\n");
  }
  else {
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(18, HIGH);
    display.print("LEAK");
    display.setCursor(0,10); 
    display.print("raw value:"); 
    display.print(rawValue);
    display.print("\n");
    display.print("recieved value:");
    display.print(boardsStruct[0].x);
    display.print("\n");
    display.print("generated value:");
    display.print(boardsStruct[0].y);
    display.print("\n");
  }
}

void setup() {
  pinMode (LED_BUILTIN, OUTPUT);  
  pinMode (18, OUTPUT);
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);   // oled i2c adress 0x3C
  Serial.begin(115200);
  // cihaz wifi station olarak kuruldu
  WiFi.mode(WIFI_STA);      
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
  
  delay(300);                // sensorun stabil hale gelmesi icin delay
}

void loop() {
  displayValues();
  display.display();
  delay(300);
  }
