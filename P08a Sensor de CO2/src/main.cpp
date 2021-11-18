/*******************************************************************************
  P08A: Práctica Sensor de CO2 MHZ19C
  
  Este ejemplo permite obtener datos del sensor de CO2 y mostrarlos por serial

  Conexiones:
  MHZ19C    Placa Heltec LoRa32
  VIN ----- Pin 5V
  GND ----- Pin GND
  TX  ----- PIN 23
  RX  ----- PIN 17

*******************************************************************************/

#include <Arduino.h>
#include "MHZ19.h"

#define RX_PIN 23           // Rx del ESP32 al que está conectado el TX del MHZ19
#define TX_PIN 17           // Tx del ESP32 al que está conectado el RX del MHZ19
#define BAUDRATE_MHZ19 9600 // Serial baudrate del MH-Z19 (no debe modificarse)
#define BAUDRATE_USB 115200 // Serial baudrate del USB

MHZ19 myMHZ19;

void setup()
{
  Serial.begin(BAUDRATE_USB);
  Serial2.begin(BAUDRATE_MHZ19, SERIAL_8N1, RX_PIN, TX_PIN); // Inicializamos un nuvo puerto serie para el MHZ19
  myMHZ19.begin(Serial2);                                    // Le indicamos a la librería que puerto usar
  myMHZ19.autoCalibration();                                 // Turn auto calibration ON (OFF autoCalibration(false))

  // Deberiamos esperar un minuto a que el sensor se caliente antes de leer
  // Serial.println("Precalentando sensor (60s)");
  // delay(60000); // precalentando sensor (60s)
}

void loop()
{
  int CO2;

  CO2 = myMHZ19.getCO2(); // Solicitamos CO2

  Serial.print("CO2 (ppm): ");
  Serial.println(CO2);

  int8_t Temp;
  Temp = myMHZ19.getTemperature(); // Solicitamos temperatura
  Serial.print("Temperatura (C): ");
  Serial.println(Temp);
  
  delay(5000);
}