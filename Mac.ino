#include <ESP8266WiFi.h>

void setup() {
  Serial.begin(115200); // Initialize Serial communication at 115200 baud rate
  Serial.println();     // Add a newline character for better formatting
  
  // Print the MAC address of the ESP8266 board
  Serial.print("ESP Board MAC Address: ");
  Serial.println(WiFi.macAddress());
}

void loop() {
  // No operations in the loop for this example
}
