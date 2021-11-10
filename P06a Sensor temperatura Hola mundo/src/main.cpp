/*******************************************************************************
  P05a: Hola mundo sensor de temperatura y humedad
  
  Este ejemplo permite leer datos del sensor de temperatura y humedad SHT30
  - Se inicializa la pantalla OLED
  - La placa lee el estado de un sensor SHT30 (temperatura y humedad)
  - Los valores de temperatura y humedad se imprimen por serial y en la pantalla

  Conexiones:
    Sensor de temperatura     Placa Heltec
    VIN                       3.3
    GND                       GND
    SCL                       PIN 22
    SDA                       PIN 21
  
 ******************************************************************************/

#include <Arduino.h>
#include <WEMOS_SHT3X.h>
#include <SSD1306Wire.h>

SHT3X sht30(0x44);

SSD1306Wire display(0x3c, SDA_OLED, SCL_OLED); 

void setup() {
  Serial.begin(115200);

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

  // Actualizamos pantalla
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 10, "  TEMP     HUM");
  char text[32];
  sprintf(text, " %.1f°C    %.1f%%", sht30.cTemp, sht30.humidity);
  display.drawString(0,40, text);
  display.display();

  delay(5000);
}