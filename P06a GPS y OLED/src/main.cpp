/*******************************************************************************
  P05A: Práctica GPS y pantalla OLED
  
  Este ejemplo permite obtener datos del GPS y mostrarlos en la pantalla
  -La placa está siempre leyendo el estado del GPS.
  -En la pantalla se proporciona información en cuanto está disponible.
  -Mientras el GPS no obtiene fix mantiene su led encendido.
  -Cuando el GPS recibe fix correcto, hace parpadear su led.

  Conexiones:
  GPS       Placa Heltec LoRa32
  VCC ----- Pin 3V3
  GND ----- Pin GND
  TX  ----- PIN 13
  RX  ----- PIN 12
  PPS No conectar
*******************************************************************************/
#include <Arduino.h>
#include "gps.h"
#include "oled.h"

void setup()
{
  Serial.begin(9600);
  setupOLED();
  setupGPS();
  delay(5000);
  Serial.println();
}

void loop()
{
  smartGPSdelay(1000);
  printGPSdata();
}