#include <Arduino.h>
/*
  P1: Práctica de LoRa P2P (Receptor)
  
  Este ejemplo permite recibir ñas tramas de LoRa enviadas por el transmisor.
*/

#include <RadioLib.h>

// SX1278 has the following connections:
// NSS pin:   SS = 18
// DIO0 pin:  DIO0 = 26
// RESET pin: RST_LoRa = 14
// DIO1 pin:  DIO1 = 35
SX1278 radio = new Module(SS, DIO0, RST_LoRa, DIO1);

void setup()
{
  Serial.begin(115200);
  delay(4000); // esperamos a que el minitor serial este listo
  
  // initialize SX1278 with default settings
  Serial.print("Inicializando... ");

  // Freq: 434.0
  // Bandwidth: 125
  // Spreading factor: 7
  int state = radio.begin(434.0, 125, 7);

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

  
  Serial.print(F("Escuchando a la espera de una transmisión ... "));
}

void loop() {
  

  String str;
  int state = radio.receive(str);

  if (state == ERR_NONE)
  {
    Serial.println(F("Recibido!"));

    Serial.print(F("Data:\t\t\t"));
    Serial.println(str);

    Serial.print(F("RSSI:\t\t\t"));
    Serial.print(radio.getRSSI());
    Serial.println(F(" dBm"));

    Serial.print(F("SNR:\t\t\t"));
    Serial.print(radio.getSNR());
    Serial.println(F(" dB"));

    Serial.print(F("Escuchando a la espera de una transmisión ... "));
  } 
  else if (state != ERR_RX_TIMEOUT)
  {
    Serial.print(F("Error, codigo: "));
    Serial.println(state);

    Serial.print(F("Escuchando a la espera de una transmisión ... "));
  }
}