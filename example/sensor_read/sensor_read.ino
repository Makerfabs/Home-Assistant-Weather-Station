#include <Adafruit_AHT10.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>


#define I2C_SDA 4
#define I2C_SCL 5
Adafruit_BMP280 bme; // I2C
Adafruit_AHT10 aht;

void setup() {
  Serial.begin(115200);
  Serial.println("Adafruit AHT10 demo!");
  Serial.println(F("BMP280 test"));
  Wire.begin(I2C_SDA, I2C_SCL);
  
  if (! aht.begin()) {
    Serial.println("Could not find AHT10? Check wiring");
    while (1) delay(10);
  }
    if (!bme.begin()) {  
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }
  Serial.println("AHT10 & BMP280 found");
}


void loop() {
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
  Serial.print("Temperature: "); Serial.print(temp.temperature); Serial.println(" degrees C");
  Serial.print("Humidity: "); Serial.print(humidity.relative_humidity); Serial.println("% rH");
  
//  Serial.print("Temperature = ");
//  Serial.print(bme.readTemperature());
//  Serial.println(" *C");
    
  Serial.print("Pressure = ");
  Serial.print(bme.readPressure());
  Serial.println(" Pa");

   // Serial.print("Approx altitude = ");
   // Serial.print(bme.readAltitude(1013.25)); // this should be adjusted to your local forcase
   // Serial.println(" m");
    
    Serial.println();
    delay(2000);
}
