#include <TTN_esp32.h>

/***************************************************************************
 *  Go to your TTN console register a device then the copy fields
 *  and replace the CHANGE_ME strings below
 ****************************************************************************/
const char* devAddr = "CHANGE_ME"; // Change to TTN Device Address
const char* nwkSKey = "CHANGE_ME"; // Change to TTN Network Session Key
const char* appSKey = "CHANGE_ME"; // Change to TTN Application Session Key

TTN_esp32 ttn ;

void message(const uint8_t* payload, size_t size, int rssi)
{
    Serial.println("-- MESSAGE");
    Serial.print("Received " + String(size) + " bytes RSSI= " + String(rssi) + "dB");

    for (int i = 0; i < size; i++)
    {
        Serial.print(" " + String(payload[i]));
    }

    Serial.println();
}

void setup()
{
    Serial.begin(115200);
    Serial.println("Starting");
    ttn.begin();
    ttn.onMessage(message); // declare callback function when is downlink from server
    ttn.personalize(devAddr, nwkSKey, appSKey);
    ttn.showStatus();
}

void loop()
{
    uint8_t* payload = (uint8_t*) "hola";
    ttn.sendBytes(payload, 5);
    Serial.println("Enviando uplink...");
    delay(60000);
}