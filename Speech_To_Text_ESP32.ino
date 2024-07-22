#include "Audio.h"
#include "CloudSpeechClient.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

String transcribedText; // Declare a global string variable to store the transcribed text

const char* ssid1 = ""; // Your Wi-Fi SSID
const char* password1 = "";     // Your Wi-Fi password

#define BOTtoken "" // Your Telegram bot token
#define CHAT_ID ""                                         // Your Telegram chat ID

const int pushButtonPin = 18; // Define the pin for the push button

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

void setup() {
  Serial.begin(115200);
  pinMode(pushButtonPin, INPUT_PULLUP); // Set push button pin as input with internal pull-up resistor
  delay(500);
  Serial.println("\r\nRecord start!\r\n");
}

void loop() {
  // Check if the push button is pressed
  if (digitalRead(pushButtonPin) == 1) {
    Serial.println("Button pressed");
    
    // Wait for the button to be released to avoid multiple triggers
  } else{
    recordAndSendSpeech();
    Serial.println("off hai");
    delay(2000);
  }
  // delay(2000);
}

void recordAndSendSpeech() {
  Audio* audio = new Audio(ICS43434);
  //Audio* audio = new Audio(M5STACKFIRE);
  audio->Record();
  Serial.println("Recording Completed. Now Processing...");
  CloudSpeechClient* cloudSpeechClient = new CloudSpeechClient(USE_APIKEY);
  transcribedText = cloudSpeechClient->Transcribe(audio); // Store the transcribed text in the variable
  delete cloudSpeechClient;
  delete audio;

  // Now you can use the transcribedText variable as needed
  Serial.println("Transcribed Text: " + transcribedText);

  // Connecting to WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid1, password1);
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Sending the transcribed text to Telegram
  bot.sendMessage(CHAT_ID, transcribedText);
}