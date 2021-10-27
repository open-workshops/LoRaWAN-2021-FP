/*******************************************************************************
  P07b: Práctica final del curso de LoRaWAN
  
  Esta práctica consiste en la monitorización del habitat natural del 
  Escribano palustre, una ave que se encuentra en peligro en las zona de
  Galicia debido a la alteración de sus zonas de anidamiento.

  El Escribano palustre anida en zonas de humedales a escasa altura del agua
  (unos 10cm), vamos a monitorizar la altura del nivel del agua en diferentes
  puntos de un humedal y vamos a medir los parametros de temperatura y humedad.

  Vamos a enviar los datos a través de LoRaWAN usando The Things Network y 
  obtendremos los datos mediante una aplicación de nodered y los mostraremos
  en forma de gráficas.

  Este ejemplo permite enviar datos de un sensor con activacion por OTAA
  - Inicalmente se hace un JOIN por OTAA a una red LoRaWAN
  - La placa lee el estado de un sensor SHT30 (temperatura y humedad)
  - La placa lee la distancia del sensor HC-SR04 (ultrasonidos)
  - Los valores de temperatura y humedad se imprimen por serial
  - La distancia obtenida se imprime por serial y en la pantalla
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

    Sensor de temperatura     Placa Heltec
    VCC                       5V
    GND                       GND
    TRIG                      PIN 12
    ECHO                      PIN 13

    # PAYLOAD FORMATER para TTN #

    function Decoder(bytes, port) {
      return {
        temperature: bytes[0] / 5.0 - 12.0,
        humidity: bytes[1],
        distance: bytes[2],
      };
    }
  
 ******************************************************************************/

#include <Arduino.h>
#include <WEMOS_SHT3X.h>
#include <TTN_esp32.h>
#include <SSD1306Wire.h>
#include <Ultrasonic.h>

const char* devEui = "XXXXXXXXXXXXXXXX"; // Rellena con el Device EUI de TTN
const char* appEui = "XXXXXXXXXXXXXXXX"; // Rellena con el Application EUI de TTN
const char* appKey = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"; // Rellena con la Application Key de TTN

TTN_esp32 ttn;
SHT3X sht30(0x44);
SSD1306Wire display(0x3c, SDA_OLED, SCL_OLED); 

// Definimos el sensor con dos pines TRIGER y ECHO
Ultrasonic ultrasonic(12, 13);

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
  int distance = ultrasonic.read(); // Solicitamos la distancia al sensor

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

  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.println(" cm");
  Serial.println();

  // Enviamos por LoRaWAN
  uint8_t payload[3];
  payload[0] = (uint8_t) ((sht30.cTemp + 12) * 5);
  payload[1] = (uint8_t) sht30.humidity;
  if (distance > 255) distance = 255;
  payload[2] = distance;

  ttn.waitForPendingTransactions();
  ttn.sendBytes(payload, 3);

  // Actualizamos pantalla
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_10);
  display.drawString(128/2, 0, "TEMP     HUM");
  char text[32];
  sprintf(text, " %.1f°C    %.1f%%", sht30.cTemp, sht30.humidity);
  display.drawString(128/2, 14, text);
  display.drawString(128/2, 30, "DISTANCIA");
  sprintf(text, " %d cm", distance);
  display.drawString(128/2,44, text);
  display.display();

  delay(30000);
}