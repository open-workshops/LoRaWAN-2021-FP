/*******************************************************************************
  P05b: Práctica de LoRaWAN con OTAA y sensor de temperatura
  
  Este ejemplo permite enviar datos de un sensor con activacion por OTAA
  - Inicalmente se hace un JOIN por OTAA a una red LoRaWAN
  - La placa lee el estado de un sensor SHT30 (temperatura y humedad)
  - Los valores de temperatura y humedad se imprimen por serial
  - Se formatean los valores anteriores en un array para enviarlos por red
  - Se realiza el uplink por LoRaWAN

  Crea un dispositivo en TTN y rellena los campos de abajo con
  las claves correspondientes

  Conexiones:
    Sensor de temperatura     Placa Heltec
    VIN                       3.3
    GND                       GND
    SCL                       PIN 15
    SDA                       PIN 4

    # PAYLOAD FORMATER para TTN #

    function Decoder(bytes, port) {
      return {
        temperature: bytes[0] / 5.0 - 12.0,
        humidity: bytes[1]
      };
    }
  
 ******************************************************************************/

#include <Arduino.h>
#include <WEMOS_SHT3X.h>
#include <TTN_esp32.h>
#include <SSD1306Wire.h>

const char* devEui = "XXXXXXXXXXXXXXXX"; // Rellena con el Device EUI de TTN
const char* appEui = "XXXXXXXXXXXXXXXX"; // Rellena con el Application EUI de TTN
const char* appKey = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"; // Rellena con la Application Key de TTN

TTN_esp32 ttn;
SHT3X sht30(0x44);
SSD1306Wire display(0x3c, SDA_OLED, SCL_OLED); 

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Reseteamos la OLED
  pinMode(RST_OLED,OUTPUT);
  digitalWrite(RST_OLED, LOW);     
  delay(50);
  digitalWrite(RST_OLED, HIGH);   

  // Iniciamos OLED
  Serial.println("Iniciando pantalla...");
  display.init();
  display.flipScreenVertically();
  
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_16);
  display.drawString(128/2, 64/2-16/2, "Iniciando...");
  display.display();
  delay(3000);

  // Iniciamos LoRaWAN
  ttn.begin();
  ttn.join(devEui, appEui, appKey);
  Serial.print("TTN Join");
  while (!ttn.isJoined())
  {
      Serial.print(".");
      delay(500);
  }
  Serial.println("\njoined!");
  ttn.showStatus();
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

  // Enviamos por LoRaWAN
  uint8_t payload[2];
  payload[0] = (uint8_t) ((sht30.cTemp + 12) * 5);
  payload[1] = (uint8_t) sht30.humidity;

  ttn.sendBytes(payload, 2);

  // Actualizamos pantalla
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 10, "  TEMP     HUM");
  char text[32];
  sprintf(text, " %.1f°C    %.1f%%", sht30.cTemp, sht30.humidity);
  display.drawString(0,40, text);
  display.display();

  delay(30000);
}