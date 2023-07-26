
#include "Zanshin_BME680.h"  // Include the BME680 Sensor library
BME680_Class BME680;         // initialize a BME680  object 
int32_t temp, humidity, pressure, gas;

void BMEstart() {
BME680.begin(I2C_STANDARD_MODE);  // Start BME680 using I2C, use first device found

BME680.setOversampling(TemperatureSensor, Oversample16);  // Set enum sensorType Oversampling
BME680.setOversampling(HumiditySensor, Oversample16);     // Set enum sensorType Oversampling
BME680.setOversampling(PressureSensor, Oversample16);    // Set enum sensorType Oversampling
BME680.setIIRFilter(IIR4);                              // Set IIR (Infinite impulse response) Filter
BME680.setGas(320, 150);                                 // Gas heating temperature and time
}
void getBMEdata() {
  turnOnRGB(0xFFFF00, 0);
  Serial.println("start");
  for (int i = 0; i < 25; i++) {

    BME680.getSensorData(temp, humidity, pressure, gas);
  }

  Serial.println("end");
  turnOffRGB();
}