#include <ESP8266WiFi.h>
extern "C" {
  #include <espnow.h>
}

#define LED_PIN 2      // Replace with your LED pin (D4 on NodeMCU)
#define BUZZER_PIN 14   // Replace with your buzzer pin
#define LED1_PIN 12  

#define AIR_QUALITY_THRESHOLD 300 // Define a threshold for poor air quality

// Structure to receive the data
typedef struct struct_message {
    char a[32];
    int airQuality;
} struct_message;

struct_message myData;

// Callback function when data is received
void OnDataRecv(uint8_t *mac_addr, uint8_t *incomingData, uint8_t len) {
    memcpy(&myData, incomingData, sizeof(myData));
    Serial.print("Message received: ");
    Serial.println(myData.a);
    Serial.print("Air Quality: ");
    Serial.println(myData.airQuality);

    // Check air quality against the threshold
    if (myData.airQuality > AIR_QUALITY_THRESHOLD) {
        digitalWrite(LED_PIN, HIGH);
        digitalWrite(BUZZER_PIN, HIGH);
        digitalWrite(LED1_PIN, LOW);
        delay(500); // LED and buzzer on for 500 ms
        digitalWrite(LED_PIN, LOW);
        digitalWrite(BUZZER_PIN, LOW);
        Serial.println("Danger: Poor Air Quality Detected!");
    } else {
        digitalWrite(LED1_PIN, HIGH);
        Serial.println("Danger: Good Air Quality Detected!");
    }
}

void setup() {
    // Initialize Serial Monitor
    Serial.begin(115200);
    
    // Set LED pin and buzzer pin as output
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW);

    pinMode(LED1_PIN, OUTPUT);
    digitalWrite(LED1_PIN, LOW);
    
    // Set device as a Wi-Fi Station
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    
    // Init ESP-NOW
    if (esp_now_init() != 0) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }
    
    // Register the receive callback
    esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
    esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
    // Nothing to do here
}
