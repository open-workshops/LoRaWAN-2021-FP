/*******************************************************************************
 P1: Práctica de LoRa P2P (Receptor)
 
 Este ejemplo permite recibir las tramas de LoRa enviadas por el transmisor.
 ******************************************************************************/
#include <Arduino.h>
#include <RadioLib.h>

// PINOUT SX1278
// LORA MODULE NSS pin   SS       = 18
// LORA MODULE DIO0 pin  DIO0     = 26
// LORA MODULE RESET pin RST_LoRa = 14
// LORA MODULE DIO1 pin  DIO1     = 35
SX1278 radio = new Module(NSS_PIN, DIO0, RST_LoRa, DIO1);

void setup()
{
  Serial.begin(115200);
  delay(4000); // esperamos a que el monitor serial este listo
  
  // initialize SX1278 with default settings
  Serial.print("Inicializando... ");

  // Freq: 865.0 - 870.0 MHz
  // Bandwidth: 125 MHz
  // Spreading factor: SF7
  int state = radio.begin(868.1, 125, 7);

  if (state == ERR_NONE)
  {
    Serial.println("OK!");
  } 
  else
  {
    Serial.print("Error, codigo: ");
    Serial.println(state);
    while (true);
  }
  Serial.print("Escuchando a la espera de una transmisión ... ");
}

void loop() {
  String str;
  int state = radio.receive(str);

  if (state == ERR_NONE)
  {
    Serial.println("Recibido!");

    Serial.print("Data:\t\t\t");
    Serial.println(str);

    Serial.print("RSSI:\t\t\t");
    Serial.print(radio.getRSSI());
    Serial.println(F(" dBm"));

    Serial.print("SNR:\t\t\t");
    Serial.print(radio.getSNR());
    Serial.println(F(" dB"));

    Serial.print("Escuchando a la espera de una transmisión ... ");
  } 
  else if (state != ERR_RX_TIMEOUT)
  {
    Serial.print("Error, codigo: "));
    Serial.println(state);

    Serial.print("Escuchando a la espera de una transmisión ... ");
  }
}