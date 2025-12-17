#define BLYNK_TEMPLATE_ID "TMPL3gjNwrFVT"   // Replace with your Blynk Template ID
#define BLYNK_TEMPLATE_NAME "Plant Watering System" // Replace with your Blynk Template Name
  
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// Blynk credentials
char auth[] = "P3Vz7X7wVr1w6XJeiveQnM1J2dKgr6jC";   // Replace with your Blynk auth token
char ssid[] = "motorola edge 40_8468";    // Replace with your Wi-Fi SSID
char pass[] = "Sonam@9022"; // Replace with your Wi-Fi password

// Define control pins for the motor
#define IN1 18
#define IN2 19
#define ENA 21

// Define the moisture sensor pin
#define MOISTURE_SENSOR_PIN 34 // Use GPIO34

// Threshold for low moisture percentage
const float MOISTURE_THRESHOLD = 10.0;

// Flag to avoid multiple notifications for the same condition
bool moistureLowNotified = false;

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT); 
  pinMode(ENA, OUTPUT);
  pinMode(MOISTURE_SENSOR_PIN, INPUT);

  Serial.begin(115200);
  Serial.println("Smart Plant Watering System Initialized");

  Blynk.begin(auth, ssid, pass);
}

void loop() {
  Blynk.run();

  int moistureValue = analogRead(MOISTURE_SENSOR_PIN);
  float moisturePercent = map(moistureValue, 0, 4095, 100, 0);

  Serial.print("Moisture Level: ");
  Serial.print(moisturePercent);
  Serial.println("%");

  Blynk.virtualWrite(V0, moisturePercent);

  if (moisturePercent < MOISTURE_THRESHOLD) {
    Serial.println("Moisture low. Turning on the motor...");

    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, 255);

    if (!moistureLowNotified) {
      Blynk.logEvent("low_moisture", "Hey baau, taniko khabar ba! tohar tulsiya jhoora jata ho!"); // Ensure this matches web dashboard
      moistureLowNotified = true;
    }
  } else {
    Serial.println("Moisture sufficient. Turning off the motor.");

    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, 0);

    if (moistureLowNotified) {
      Blynk.logEvent("moisture_normal", "Hey bahini, kaha dheyaan ba ho! ketna paani dalbu tulsiya me!");
      moistureLowNotified = false;
    }
  }

  delay(2000);
}