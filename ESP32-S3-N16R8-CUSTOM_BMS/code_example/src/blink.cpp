#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

/* ====== CONFIG ====== */
#define LED_PIN       21
#define LED_COUNT     3
#define BRIGHTNESS    30
#define TEST_LED_PIN  2

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);

/* ====== TIMER VAR ====== */
unsigned long ledMillis     = 0;
unsigned long pixelMillis   = 0;
unsigned long serialMillis  = 0;

/* ====== STATE ====== */
bool ledState = false;
uint8_t colorIndex = 0;

/* ====== COLOR TABLE (RGBW) ====== */
uint32_t colors[] = {
  strip.Color(255,   0,   0,   0), // Red
  strip.Color(0,   255,   0,   0), // Green
  strip.Color(0,     0, 255,   0), // Blue
  strip.Color(0,     0,   0, 255), // White
};
const uint8_t COLOR_COUNT = sizeof(colors) / sizeof(colors[0]);

/* ====== SETUP ====== */
void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("\n=== BOARD TEST START ===");

  pinMode(TEST_LED_PIN, OUTPUT);
  digitalWrite(TEST_LED_PIN, LOW);

  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  strip.clear();
  strip.show();
}

/* ====== LOOP ====== */
void loop() {
  unsigned long now = millis();

  /* ===== GPIO BLINK 1s ===== */
  if (now - ledMillis >= 1000) {
    ledMillis = now;
    ledState = !ledState;
    digitalWrite(TEST_LED_PIN, ledState);
  }

  /* ===== NEOPIXEL EFFECT (NON-BLOCKING) ===== */
  static uint8_t pixelIndex = 0;
  static unsigned long effectMillis = 0;

  if (now - effectMillis >= 100) {   // tốc độ effect
    effectMillis = now;

    strip.clear();
    strip.setPixelColor(pixelIndex, colors[colorIndex]);
    strip.show();

    pixelIndex++;

    if (pixelIndex >= LED_COUNT) {
      pixelIndex = 0;
      colorIndex++;
      if (colorIndex >= COLOR_COUNT) colorIndex = 0;
    }
  }

  /* ===== SERIAL HEARTBEAT ===== */
  if (now - serialMillis >= 2000) {
    serialMillis = now;
  }
}

