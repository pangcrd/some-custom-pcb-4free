#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>

U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE); 	      
#define U8LOG_WIDTH 16
#define U8LOG_HEIGHT 8
uint8_t u8log_buffer[U8LOG_WIDTH*U8LOG_HEIGHT];
U8X8LOG u8x8log;

#define LED_PIN 8
#define BUZZER_PIN 7

unsigned long previousBuzzerMillis = 0;
const long buzzerInterval = 2000;   

#define BUZZER_CHANNEL 0
#define BUZZER_FREQ 4000      // 2kHz 
#define BUZZER_RESOLUTION 8   // 8-bit resolution

void handleBuzzer() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousBuzzerMillis >= buzzerInterval) {
    previousBuzzerMillis = currentMillis;
    ledcWrite(BUZZER_CHANNEL, 128);  // Duty cycle 50% (0-255)
    digitalWrite(LED_PIN, HIGH);
    delay(200); 
    ledcWrite(BUZZER_CHANNEL, 0);
    digitalWrite(LED_PIN, LOW);
  }
}

void setup(void)
{	
  
  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  
  u8x8log.begin(u8x8, U8LOG_WIDTH, U8LOG_HEIGHT, u8log_buffer);
  u8x8log.setRedrawMode(1);		// 0: Update screen with newline, 1: Update screen for every char  

  pinMode(LED_PIN, OUTPUT);
  

  ledcSetup(BUZZER_CHANNEL, BUZZER_FREQ, BUZZER_RESOLUTION);
  ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL);
  ledcWrite(BUZZER_CHANNEL, 0);
  
}

unsigned long t = 0;
// print the output of millis() to the terminal every second
void loop(void) {
  if ( t < millis() ) {
    t = millis() + 15000;			// every 15 seconds
    u8x8log.print("\f");			// \f = form feed: clear the screen 
  }
  u8x8log.print("millis=");
  u8x8log.print(millis());
  u8x8log.print("\n");
  handleBuzzer();
  delay(500);
}
