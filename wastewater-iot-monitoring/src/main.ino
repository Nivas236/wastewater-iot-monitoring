/*
 * ============================================================
 * Cloud-Based IoT System for Real-Time Monitoring and
 * Filtration of Industrial Wastewater for Multi-Purpose Reuse
 * ============================================================
 * Team:   SK. Kalesha Vali | M. Nivas | CH. R. V. Durga Sai | N. Sanjeeva Reddy
 * Guide:  Mr. G. Vijay Kumar, M.Tech., (Ph.D)
 * Inst:   ALIET, Vijayawada — JNTUK (2022–2026)
 * ============================================================
 */

#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>

// ── Wi-Fi Credentials ─────────────────────────────────────────────────────────
const char* ssid     = "YOUR_WIFI_SSID";       // Replace with your Wi-Fi name
const char* password = "YOUR_WIFI_PASSWORD";   // Replace with your Wi-Fi password

// ── ThingSpeak Configuration ──────────────────────────────────────────────────
unsigned long channelID  = 0;                  // Replace with your Channel ID
const char*   writeAPI   = "YOUR_WRITE_API_KEY"; // Replace with your Write API Key

// ── Pin Definitions ───────────────────────────────────────────────────────────
#define PH_PIN         A0   // pH sensor analog pin
#define TURBIDITY_PIN  A1   // Turbidity sensor analog pin
#define TDS_PIN        A2   // TDS sensor analog pin
#define TEMP_PIN       D5   // DS18B20 data pin
#define PUMP_PIN       D6   // Water pump relay pin
#define BUZZER_PIN     D7   // Buzzer pin

// ── LCD: RS, EN, D4, D5, D6, D7 ──────────────────────────────────────────────
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

// ── Temperature Sensor ────────────────────────────────────────────────────────
OneWire           oneWire(TEMP_PIN);
DallasTemperature tempSensor(&oneWire);

// ── Safe Threshold Values ─────────────────────────────────────────────────────
const float PH_MIN        = 6.5;
const float PH_MAX        = 8.5;
const float TURBIDITY_MAX = 4.0;    // NTU
const float TEMP_MIN      = 10.0;   // °C
const float TEMP_MAX      = 40.0;   // °C
const float TDS_MAX       = 500.0;  // mg/L (ppm)

// ── Upload interval (ThingSpeak free: min 15 sec) ─────────────────────────────
const unsigned long UPLOAD_INTERVAL = 15000;
unsigned long lastUploadTime = 0;

WiFiClient client;

// ─────────────────────────────────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);

  // Pin modes
  pinMode(PUMP_PIN,   OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(PUMP_PIN,   LOW);
  digitalWrite(BUZZER_PIN, LOW);

  // LCD
  lcd.begin(16, 2);
  lcd.print("Initializing...");

  // Temperature sensor
  tempSensor.begin();

  // Wi-Fi
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi Connected!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // ThingSpeak
  ThingSpeak.begin(client);

  lcd.clear();
  lcd.print("System Ready");
  delay(1500);
}

// ─────────────────────────────────────────────────────────────────────────────
void loop() {
  // Read all sensors
  float ph          = readPH();
  float turbidity   = readTurbidity();
  float temperature = readTemperature();
  float tds         = readTDS();

  // Display on LCD
  displayLCD(ph, turbidity, temperature, tds);

  // Print to Serial Monitor
  Serial.println("──────────────────────────");
  Serial.print("pH:          "); Serial.println(ph, 2);
  Serial.print("Turbidity:   "); Serial.print(turbidity, 2); Serial.println(" NTU");
  Serial.print("Temperature: "); Serial.print(temperature, 1); Serial.println(" °C");
  Serial.print("TDS:         "); Serial.print(tds, 1); Serial.println(" mg/L");

  // Check thresholds — activate pump & buzzer if needed
  bool alert = checkThresholds(ph, turbidity, temperature, tds);
  controlFiltration(alert);

  // Upload to ThingSpeak every 15 seconds
  unsigned long now = millis();
  if (now - lastUploadTime >= UPLOAD_INTERVAL) {
    uploadToCloud(ph, turbidity, temperature, tds);
    lastUploadTime = now;
  }

  delay(2000);
}

// ─────────────────────────────────────────────────────────────────────────────
// Sensor Reading Functions
// ─────────────────────────────────────────────────────────────────────────────

float readPH() {
  int raw = analogRead(PH_PIN);
  // Convert ADC value to pH (calibrate offset for your sensor)
  float voltage = raw * (5.0 / 1023.0);
  float ph = 3.5 * voltage + 0.00;   // Adjust calibration constants as needed
  return constrain(ph, 0.0, 14.0);
}

float readTurbidity() {
  int raw = analogRead(TURBIDITY_PIN);
  float voltage = raw * (5.0 / 1023.0);
  // Higher voltage = clearer water (inverse relationship for most sensors)
  float turbidity = -1120.4 * voltage * voltage + 5742.3 * voltage - 4352.9;
  return max(0.0f, turbidity);
}

float readTemperature() {
  tempSensor.requestTemperatures();
  return tempSensor.getTempCByIndex(0);
}

float readTDS() {
  int raw = analogRead(TDS_PIN);
  float voltage = raw * (5.0 / 1023.0);
  // Standard TDS conversion (adjust EC factor for your probe)
  float tds = (133.42 * voltage * voltage * voltage
             - 255.86 * voltage * voltage
             + 857.39 * voltage) * 0.5;
  return max(0.0f, tds);
}

// ─────────────────────────────────────────────────────────────────────────────
// Display on LCD (alternates between two screens)
// ─────────────────────────────────────────────────────────────────────────────
void displayLCD(float ph, float turbidity, float temperature, float tds) {
  // Screen 1: pH and Turbidity
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("pH:");
  lcd.print(ph, 1);
  lcd.setCursor(8, 0);
  lcd.print("Turb:");
  lcd.print(turbidity, 1);

  lcd.setCursor(0, 1);
  lcd.print("Temp:");
  lcd.print(temperature, 1);
  lcd.print((char)223); // degree symbol
  lcd.print("C");
  lcd.setCursor(9, 1);
  lcd.print("TDS:");
  lcd.print((int)tds);

  delay(3000);
}

// ─────────────────────────────────────────────────────────────────────────────
// Threshold Check — returns true if any parameter is unsafe
// ─────────────────────────────────────────────────────────────────────────────
bool checkThresholds(float ph, float turbidity, float temperature, float tds) {
  bool unsafe = false;

  if (ph < PH_MIN || ph > PH_MAX) {
    Serial.println("⚠️  ALERT: pH out of range!");
    unsafe = true;
  }
  if (turbidity > TURBIDITY_MAX) {
    Serial.println("⚠️  ALERT: Turbidity too high!");
    unsafe = true;
  }
  if (temperature < TEMP_MIN || temperature > TEMP_MAX) {
    Serial.println("⚠️  ALERT: Temperature out of range!");
    unsafe = true;
  }
  if (tds > TDS_MAX) {
    Serial.println("⚠️  ALERT: TDS too high!");
    unsafe = true;
  }

  return unsafe;
}

// ─────────────────────────────────────────────────────────────────────────────
// Control Filtration Pump and Buzzer
// ─────────────────────────────────────────────────────────────────────────────
void controlFiltration(bool activate) {
  if (activate) {
    digitalWrite(PUMP_PIN,   HIGH);  // Start pump
    digitalWrite(BUZZER_PIN, HIGH);  // Sound buzzer
    lcd.setCursor(0, 1);
    lcd.print("FILTRATION ON   ");
    delay(1000);
    digitalWrite(BUZZER_PIN, LOW);   // Buzzer off after 1 sec
  } else {
    digitalWrite(PUMP_PIN, LOW);     // Pump off — water is safe
  }
}

// ─────────────────────────────────────────────────────────────────────────────
// Upload Sensor Data to ThingSpeak Cloud
// ─────────────────────────────────────────────────────────────────────────────
void uploadToCloud(float ph, float turbidity, float temperature, float tds) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi disconnected. Skipping upload.");
    return;
  }

  ThingSpeak.setField(1, ph);
  ThingSpeak.setField(2, turbidity);
  ThingSpeak.setField(3, temperature);
  ThingSpeak.setField(4, tds);

  int status = ThingSpeak.writeFields(channelID, writeAPI);
  if (status == 200) {
    Serial.println("✅  Cloud upload successful!");
  } else {
    Serial.print("❌  Cloud upload failed. HTTP code: ");
    Serial.println(status);
  }
}
