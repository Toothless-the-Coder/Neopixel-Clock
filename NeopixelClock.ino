#include <WiFi.h>
#include <time.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_INA219.h>

// ----------------------------------------------------
// Wifi Configuration
// ----------------------------------------------------
const char* ssid     = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Timezone offset in seconds (e.g., -18000 for EST)
const long  gmtOffset_sec = -18000;
// Daylight savings time offset in seconds (e.g., 3600 for 1 hr)
const int   daylightOffset_sec = 3600;

// ----------------------------------------------------
// Pin Configuration
// ----------------------------------------------------
// WS2812B Data In
#define LED_PIN  3

// INA219 I2C Pins (Power Monitor)
#define SDA_PIN  4
#define SCL_PIN  5

// ----------------------------------------------------
// Hardware Setup
// ----------------------------------------------------
#define NUM_LEDS 60
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

Adafruit_INA219 ina219;
bool inaFound = false;

// Time Colors
uint32_t colorHour   = strip.Color(255, 0, 0);   // Red
uint32_t colorMinute = strip.Color(0, 255, 0);   // Green
uint32_t colorSecond = strip.Color(0, 0, 255);   // Blue
uint32_t colorTicks  = strip.Color(10, 10, 10);  // Dim white for hour marks

// Brightness and Power Control
// Limit brightness actively if power draw approaches USB limits
const float MAX_CURRENT_MA = 1500.0; // 1.5A soft limit to avoid dropping USB voltage
uint8_t targetBrightness = 50; 
uint8_t currentBrightness = targetBrightness;

void setup() {
  Serial.begin(115200);
  delay(1000); // Give serial monitor time to open
  Serial.println("\n--- Starting ESP32-C6 NeoPixel Clock ---");

  // 1. Initialize LEDs
  strip.begin();
  strip.setBrightness(currentBrightness);
  strip.clear();
  strip.show();

  // 2. Initialize INA219 via I2C
  // Set explicit I2C pins for ESP32-C6
  Wire.begin(SDA_PIN, SCL_PIN);
  
  if (!ina219.begin()) {
    Serial.println("WARNING: Failed to find INA219 chip. Checking power will be disabled.");
  } else {
    inaFound = true;
    Serial.println("INA219 initialized successfully.");
  }

  // 3. Connect to WiFi
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  int loadingPos = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
    
    // Quick loading animation on ring
    strip.clear();
    strip.setPixelColor(loadingPos % NUM_LEDS, strip.Color(50, 50, 0));
    strip.show();
    loadingPos++;
  }
  
  Serial.println("\nWiFi connected.");
  Serial.printf("IP address: %s\n", WiFi.localIP().toString().c_str());

  // 4. Initialize Network Time Protocol (NTP)
  Serial.println("Synchronizing time...");
  configTime(gmtOffset_sec, daylightOffset_sec, "pool.ntp.org", "time.nist.gov");
  
  struct tm timeinfo;
  while (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time. Retrying...");
    delay(1000);
  }
  Serial.println("Time synchronized successfully.");
}

// Helper to additively blend colors without wiping existing pixel colors
void addPixelColor(int index, uint32_t colorToAdd) {
  uint32_t currentColor = strip.getPixelColor(index);
  
  // Unpack RGB components
  uint8_t oldR = (currentColor >> 16) & 0xFF;
  uint8_t oldG = (currentColor >> 8) & 0xFF;
  uint8_t oldB = currentColor & 0xFF;
  
  uint8_t addR = (colorToAdd >> 16) & 0xFF;
  uint8_t addG = (colorToAdd >> 8) & 0xFF;
  uint8_t addB = colorToAdd & 0xFF;
  
  // Calculate new color with bounds (max 255)
  uint8_t newR = min(255, oldR + addR);
  uint8_t newG = min(255, oldG + addG);
  uint8_t newB = min(255, oldB + addB);
  
  strip.setPixelColor(index, strip.Color(newR, newG, newB));
}

void loop() {
  // --- 1. Track Power Draw ---
  if (inaFound) {
    float voltage_V = ina219.getBusVoltage_V();
    float current_mA = ina219.getCurrent_mA();
    float power_mW = ina219.getPower_mW();
    
    static unsigned long lastPrint = 0;
    if (millis() - lastPrint > 5000) {
      Serial.printf("Power Status | Bus: %.2f V | Current: %.2f mA | Power: %.2f mW\n", 
                    voltage_V, current_mA, power_mW);
      lastPrint = millis();
    }

    // Dynamic brightness adjustment based on power limits
    if (current_mA > MAX_CURRENT_MA && currentBrightness > 5) {
      currentBrightness--;
      strip.setBrightness(currentBrightness);
    } else if (current_mA < (MAX_CURRENT_MA - 150) && currentBrightness < targetBrightness) {
      currentBrightness++;
      strip.setBrightness(currentBrightness);
    }
  }

  // --- 2. Fetch Current Time ---
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Warning: RTC desync, could not get time.");
    delay(100);
    return;
  }

  int h = timeinfo.tm_hour;
  int m = timeinfo.tm_min;
  int s = timeinfo.tm_sec;

  // Map time to 60-LED ring indices
  // Hour hand: smoothly transition between hours depending on minutes
  int hour_12 = h % 12;
  int hour_pos = (hour_12 * 5) + (m / 12);
  hour_pos = hour_pos % NUM_LEDS; // Ensure it stays within bounds
  
  int minute_pos = m % NUM_LEDS;
  int second_pos = s % NUM_LEDS;

  // --- 3. Render Clock Frame ---
  strip.clear();
  
  // Draw base hour marks (12, 1, 2, 3...)
  for (int i = 0; i < NUM_LEDS; i += 5) {
    addPixelColor(i, colorTicks);
  }
  
  // Draw the hands
  // Order matters here! E.g. overlapping hands will blend colors because we use addPixelColor.
  addPixelColor(hour_pos, colorHour);
  addPixelColor(minute_pos, colorMinute);
  
  // To make second hand 'sweep' or pulse, we could add a fade trail, but for now just one pixel
  addPixelColor(second_pos, colorSecond);
  
  strip.show();
  
  // Run loop approx 20 times per second for smooth brightness fading and fast response
  delay(50);
}
