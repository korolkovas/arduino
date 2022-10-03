// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include "U8glib.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

U8GLIB_SH1106_128X64 u8g(13, 11, 10, 9); // SCK = 13, MOSI = 11, CS = 10, A0 = 9
#define DHTPIN 8     // Digital pin connected to the DHT sensor
#define DHTTYPE    DHT11     // DHT 11
DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS;
int pinOut = 7;
float lastUp = 0;
float lastDown = 0;
float goingUp = 0;
float goingDown = 0;

void draw(void) {
  // graphic commands to redraw the complete screen should be placed here
  u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);
  u8g.drawStr( 0, 15, "Amu Bitu3!");
  u8g.drawStr( 10, 50, "Amu Bitu42!");
}



void setup() {

  Serial.begin(9600);

  // TEMPERATURE AND HUMIDITY SENSOR SETUP
  dht.begin(); // Initialize device.

  Serial.println(F("DHTxx Unified Sensor Example"));
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println(F("Humidity Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
  Serial.println(F("------------------------------------"));
  delayMS = sensor.min_delay / 1000; // Set delay between sensor readings based on sensor details.


  // RELAY OUTPUT SETUP
  pinMode(pinOut, OUTPUT);


  // OLED DISPLAY SETUP
  u8g.setRot180();
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
  u8g.setColorIndex(255); // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
  u8g.setColorIndex(3); // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
  u8g.setColorIndex(1); // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
  u8g.setHiColorByRGB(255,255,255);
  }
}

void loop() {
  // Delay between measurements.
  delay(delayMS);

  // Get temperature event and print its value.
  sensors_event_t event;

  if (isnan(event.temperature || isnan(event.relative_humidity))) {
    Serial.println(F("Error reading sensor!"));
  }
  else {
    dht.temperature().getEvent(&event);
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
    double temp;
    temp = event.temperature;

    dht.humidity().getEvent(&event);
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
    double humi;
    humi = event.relative_humidity;

    if (temp >= 13){
      digitalWrite(pinOut, HIGH);
      lastUp = millis();
      goingUp = (lastUp - lastDown) / 1000 / 60;

    }
    if (temp <= 9){
      digitalWrite(pinOut, LOW);
      lastDown = millis();
      goingDown = (lastDown - lastUp) / 1000 / 60;
    }

    u8g.firstPage();
    do {
      // First Line Temperature
      u8g.setFont(u8g_font_courR14);
      u8g.setPrintPos(1, 14);
      u8g.print("T: ");
      u8g.print(temp);
      u8g.write(0xB0);
      u8g.print("C");

      u8g.setPrintPos(1, 36);
      u8g.print("H: ");
      u8g.print(humi);
      u8g.write(0x25);

      // Third line Deltas
      u8g.setPrintPos(6, 58);
      u8g.setFont(u8g_font_6x12_67_75);
      u8g.write(0x11);
      u8g.setFont(u8g_font_courR10);
      u8g.print(" ");
      u8g.print(goingUp, 2);
      u8g.setPrintPos(70, 58);
      u8g.setFont(u8g_font_6x12_67_75);
      u8g.write(0x13);
      u8g.setFont(u8g_font_courR10);
      u8g.print(" ");
      u8g.print(goingDown, 2);
    } while( u8g.nextPage() );
  }
}
