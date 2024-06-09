#include <WiFi.h>
#include <WiFiUdp.h>

// Network credentials
const char *ssid = "Laptop-2407";
const char *password = "Suprit@internet24";

// Destination IP and port (Arduino Uno server)
const char *remoteIp = "192.168.0.101"; // Replace with the IP address of the Arduino Uno
const int remotePort = 8888; // Replace with the port number the Arduino Uno is listening on

// Create UDP object
WiFiUDP udp;

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected");

  // Begin UDP communication
  udp.begin(8888); // local port
}

void loop() {
  // Send a message to the Arduino Uno server
  if (Serial.available() > 0) { // Check if data is available to read from serial
    String input = Serial.readStringUntil('\n'); // Read the input until newline character ('\n') is encountered
    char charArray[input.length() + 1];
    input.toCharArray(charArray, input.length() + 1);
    sendMessage(charArray);
  }
  int packetSize = udp.parsePacket();
  if (packetSize) {
    char packetBuffer[255]; // buffer to hold incoming packet
    int len = udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = 0;
      Serial.printf("Cherish: %s\n", packetBuffer);
    }
  }
  delay(1000); // Wait before sending the next message
}

void sendMessage(const char *message) {
  // Send the message to the Arduino Uno server
  udp.beginPacket(remoteIp, remotePort);
  udp.println(message);
  udp.endPacket();

  Serial.print("you: ");
  Serial.println(message);
}