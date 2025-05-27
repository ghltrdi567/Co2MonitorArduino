// дефайн перед подключением либы - использовать microWire (лёгкая либа для I2C)
//#define USE_MICRO_WIRE

// дефайн перед подключением либы - скорость SPI
//#define OLED_SPI_SPEED 4000000ul

#include <GyverOLED.h>

#include <Arduino.h>
#include <SensirionI2CScd4x.h>
#include <Wire.h>


// примеры:
//GyverOLED<SSD1306_128x32, OLED_BUFFER> oled;
GyverOLED<SSD1306_128x32, OLED_NO_BUFFER> oled;




SensirionI2CScd4x scd4x;




void setup() {

  Serial.begin(115200);
    while (!Serial) {
        delay(100);
    }
  
  oled.init();  // инициализация

  Wire.begin();
  //Wire.setClock(400000); //set clock speed for I2C bus to maximum allowed for HDC1080

  scd4x.begin(Wire);

  uint16_t error;

  error = scd4x.stopPeriodicMeasurement();
    if (error) {
        Serial.print("Error");
    }


    // Start Measurement
    error = scd4x.startPeriodicMeasurement();
    if (error) {
       Serial.print("Error");
    }

  // --------------------------
  oled.clear();   // очистить дисплей (или буфер)
  //oled.update();  // обновить. Только для режима с буфером! OLED_BUFFER

  // --------------------------
  oled.home();            // курсор в 0,0
  oled.print("Загрузка...");   // печатай что угодно: числа, строки, float, как Serial!
  oled.update();
  

  // --------------------------
 
  //oled.update();
}

void loop() {

uint16_t error;
    

    delay(100);

    
    

    // Read Measurement
    uint16_t co2 = 0;
    float temperature = 0.0f;
    float humidity = 0.0f;
    bool isDataReady = false;
    error = scd4x.getDataReadyFlag(isDataReady);
    if (error) {
        //oled.print("Error");
        return;
    }
    if (!isDataReady) {
      //oled.print("Error");
        return;
    }
    error = scd4x.readMeasurement(co2, temperature, humidity);
    oled.clear(); 
    oled.home();
    if (error) {
        oled.print("Error");
    } else if (co2 == 0) {
        oled.print("Error");
    } else {
        oled.print("Co2: ");
        oled.print(co2);
        oled.setCursorXY(0, 9);
        oled.print("Temp: ");
        oled.print(temperature);
        oled.setCursorXY(0, 18);
        oled.print("Hum: ");
        oled.print(humidity);
    }

    oled.update();
}