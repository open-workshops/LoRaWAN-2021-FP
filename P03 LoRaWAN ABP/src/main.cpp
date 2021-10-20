/***************************************************************************
  P03: Práctica de LoRaWAN con ABP
  
  Este ejemplo permite enviar uplinks con activación por ABP
  - Cada 15 segundos se envía un uplink LoRaWAN con un hola mundo
  - Tras cada uplink se escucha por si llega un downlink

  Crea un dispositivo en TTN y rellena los campos de abajo con
  las claves correspondientes

    # PAYLOAD FORMATER para TTN #

    function Decoder(bytes, port) {
      // Decode plain text; for testing only 
      return {
        myTestValue: String.fromCharCode.apply(null, bytes)
      };
    }

 ****************************************************************************/
#include <Arduino.h>
#include <TTN_esp32.h>

const char *devAddr = "XXXXXXX";                          // Rellena con el Device Address de TTN
const char *nwkSKey = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"; // Rellena con la Network Session Key de TTN
const char *appSKey = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"; // Rellena con la Application Session Key de TTN

TTN_esp32 ttn;

void message(const uint8_t *payload, size_t size, int rssi)
{
  Serial.println("-- NUEVO DOWNLINK");
  Serial.println("Recibidos " + String(size) + " bytes RSSI=" + String(rssi) + "db");

  for (int i = 0; i < size; i++)
  {
    Serial.print(" " + String(payload[i]));
  }

  Serial.println();
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Iniciando");

  ttn.begin();
  ttn.onMessage(message); // declaramos la función de callback para manejar los downlinks desde el servidor
  ttn.personalize(devAddr, nwkSKey, appSKey);
  ttn.showStatus();
}

void loop()
{
  uint8_t *payload = (uint8_t *)"hola";
  ttn.sendBytes(payload, 4);
  Serial.println("Enviando uplink...");
  delay(15000);
}