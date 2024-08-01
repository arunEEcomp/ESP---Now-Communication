#include <ESP8266WiFi.h>
extern "C" {
  #include <espnow.h>
}

// MAC Address of the receiver NodeMCU
uint8_t broadcastAddress[] = {0xEC, 0x64, 0xC9, 0xC4, 0x72, 0xC4}; // Replace with your receiver's MAC address

typedef struct struct_message {
    char a[32];
    int airQuality;
} struct_message;

struct_message myData;

const int sensorPin = A0; // Analog pin for MQ sensor

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
    Serial.print("Last Packet Send Status: ");
    Serial.println(sendStatus == 0 ? "Delivery Success" : "Delivery Fail");
}

void setup() {
    Serial.begin(115200);
    Serial.println();
    
    // Set device as a Wi-Fi Station
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    
    // Init ESP-NOW
    if (esp_now_init() != 0) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }
    
    // Register the send callback
    esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
    esp_now_register_send_cb(OnDataSent);
    
    // Add peer
    esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

void loop() {
    // Read the air quality from the MQ sensor
    int airQuality = analogRead(sensorPin);
    
    // Prepare the message
    strcpy(myData.a, "Air Quality");
    myData.airQuality = airQuality;
    
    // Send the message
    esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    
    // Print air quality to the Serial Monitor
    Serial.print("Air Quality: ");
    Serial.println(airQuality);
    
    delay(5000); // Send data every 5 seconds
}
