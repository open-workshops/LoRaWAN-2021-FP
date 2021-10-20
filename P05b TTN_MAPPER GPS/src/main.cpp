/*******************************************************************************
  P05B: Práctica TTN MAPPER GPS
  
  Este ejemplo mapea la cobertura de la red TTN utilizando LoRaWAN y GPS.
  -La placa está siempre leyendo el estado del GPS.
  -En la pantalla se proporciona información en cuanto está disponible.
  -Mientras el GPS no obtiene fix mantiene su led encendido.
  -Cuando el GPS recibe fix correcto, hace parpadear su led.
  -Utiliza tipo de sesión ABP y SF7

  Conexiones:
  GPS       Placa Heltec LoRa32
  VCC ----- Pin 3V3
  GND ----- Pin GND
  TX  ----- PIN 13
  RX  ----- PIN 12
  PPS No conectar
*******************************************************************************/
#include <Arduino.h>
#include <TTN_esp32.h> //https://github.com/rgot-org/TheThingsNetwork_esp32
#include "gps.h"
#include "oled.h"

const char* devAddr = "CHANGE_ME"; // Change to TTN Device Address
const char* nwkSKey = "CHANGE_ME"; // Change to TTN Network Session Key
const char* appSKey = "CHANGE_ME"; // Change to TTN Application Session Key

TTN_esp32 ttn;
uint8_t txBuffer[9];

void setup()
{
  Serial.begin(115200);
  setupOLED();
  setupGPS();
  Serial.println("Starting");
  ttn.begin();
  ttn.personalize(devAddr, nwkSKey, appSKey);
  ttn.setDataRate(6); //configura DataRate a 6 (SF7)
  ttn.showStatus();
}

void loop()
{
  smartGPSdelay(5000);
  if (checkGPSFix())
  {
    buildGPSpacket(txBuffer);
    Serial.println("Packet queued");
    ttn.sendBytes(txBuffer, sizeof(txBuffer));
  }
  printGPSdata();
}