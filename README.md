# Macro-pad-from-scratch
Making a 6 key macro pad from house hold items like copper wire,foiland carboard due me not having a lap


code

#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Replace with your phone's local IP address and MacroDroid webhook port
const char* phoneWebhookUrl = "http://192.168.1.50:8080/triggerAction";

// Pin mapping for your 6 buttons (wired to GND when pressed)
const int buttonPins[6] = {D1, D2, D3, D5, D6, D7};

// Corresponding action names for each button
const String buttonActions[6] = {"flash", "mute", "vol_up", "vol_down", "pause",
                                 "play"};

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT); // Built-in LED for visual feedback

  // Configure pins with internal pull-ups (reads HIGH normally, LOW when touching GND)
  for (int i = 0; i < 6; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
}

void loop() {
  for (int i = 0; i < 6; i++) {
    if (digitalRead(buttonPins[i]) == LOW) { // Foil contact made to GND
      String action = buttonActions[i];

      Serial.print("Button ");
      Serial.print(i + 1);
      Serial.print(" pressed -> Action: ");
      Serial.println(action);

      flashLED();             // Flash onboard LED
      sendMobileAction(action); // Send action name to phone

      delay(500); // Debounce delay
    }
  }
}

// Function for visual feedback (blinks built-in LED)
void flashLED() {
  digitalWrite(LED_BUILTIN, LOW); // On NodeMCU, LOW turns LED ON
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH); // Turn LED OFF
}

// Function to send the specific command to your phone via Webhook
void sendMobileAction(String actionName) {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    String url = String(phoneWebhookUrl) + "?action=" + actionName;

    http.begin(client, url);
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      Serial.println("Action sent to phone successfully!");
    } else {
      Serial.print("HTTP error code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  }
}

