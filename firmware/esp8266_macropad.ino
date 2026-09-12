#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
​const char* ssid = "MY_WIFI_SSID";
const char* password = "MY_WIFI_PASSWORD";
​const char* phoneWebhookUrl = "https://trigger.macrodroid.com/6eff990f-9ac6-4e5c-bf67-8758473ddac2/";
​const int buttonPins[6] = {D1, D2, D3, D5, D6, D7};
​const String buttonActions[6] = {"flashlight", "mute", "back", "music_app", "pause", "play"};
​int lastButtonStates[6] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};
​void setup() {
Serial.begin(115200);
pinMode(LED_BUILTIN, OUTPUT);
​for (int i = 0; i < 6; i++) {
pinMode(buttonPins[i], INPUT_PULLUP);
}
​WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("\nWiFi connected!");
Serial.print("ESP8266 IP: ");
Serial.println(WiFi.localIP());
}
​void loop() {
for (int i = 0; i < 6; i++) {
int currentState = digitalRead(buttonPins[i]);
​if (lastButtonStates[i] == HIGH && currentState == LOW) {
Serial.print("Button pressed for action: ");
Serial.println(buttonActions[i]);
​triggerWebhook(buttonActions[i]);
​delay(200);
}
​lastButtonStates[i] = currentState;
}
}
​void triggerWebhook(String action) {
if (WiFi.status() == WL_CONNECTED) {
WiFiClient client;
HTTPClient http;
​String fullUrl = String(phoneWebhookUrl) + "?action=" + action;
​Serial.print("Sending request to: ");
Serial.println(fullUrl);
​http.begin(client, fullUrl);
int httpResponseCode = http.GET();
​if (httpResponseCode > 0) {
Serial.print("HTTP Response code: ");
Serial.println(httpResponseCode);
} else {
Serial.print("Error code: ");
Serial.println(httpResponseCode);
}
​http.end();
} else {
Serial.println("WiFi Disconnected. Cannot send webhook.");
}
}
