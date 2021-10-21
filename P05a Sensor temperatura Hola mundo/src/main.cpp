/*******************************************************************************
  P05a: Hola mundo sensor de temperatura y humedad
  
  Este ejemplo permite leer datos del sensor de temperatura y humedad SHT30
  - La placa lee el estado de un sensor SHT30 (temperatura y humedad)
  - Los valores de temperatura y humedad se imprimen por serial

  Conexiones:
    Sensor de temperatura     Placa Heltec
    VIN                       3.3
    GND                       GND
    SCL                       PIN 22
    SDA                       PIN 21
  
 ******************************************************************************/

#include <Arduino.h>
#include <WEMOS_SHT3X.h>

SHT3X sht30(0x44);

void setup() {
  Serial.begin(115200);
  delay(1000);
}

void loop() {
  int errorCode = sht30.get(); // solicitamos medición de temperatura y humedad al sensor

  if (errorCode) // si ha habido un error, no continuamos
  {
    Serial.println("Error leyendo el sensor");
    delay(100000);
    return;
  }

  Serial.print("Temperatura: ");
  Serial.print(sht30.cTemp);
  Serial.println("º C");

  Serial.print("Humedad: ");
  Serial.print(sht30.humidity);
  Serial.println(" %");
  
  Serial.println();

  delay(5000);
}