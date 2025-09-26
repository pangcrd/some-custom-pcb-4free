/** 
Wiring the ADS1115 Module to an ESP32 C3 Super MINi
  VDD        5V
  GND        GND
  SCL        6 (or SCL)
  SDA        5 (or SDA)
  ADDR       TO SDA
*/

/** 
 * TẤT CẢ NHŨNG PHẦN MÔ TẢ BÊN DƯỚI ĐỀU CÓ TRONG DATASHEET <page 25-26> -> https://www.ti.com/lit/ds/symlink/ads1115.pdf?ts=1756774427639
 * ADS1115 có độ phân giải 16-bit signed → giá trị dao động -32768 … +32767
 * Nếu ae đang đọc single-ended (A0,A1... vs GND) thì giá trị nằm từ 0 … 32767.
 * Mình đang set ADS1115_PGA_6P144 → nó đo được tới ±6.144V
 * Code hiện tại đang set PGA = ±6.144V. → tức là full-scale (FSR) = 6.144V
 * Cách tính điện áp từ ADC -> Vin ​= (ADC/32767)​×FSR
 * 
 */
#include <Arduino.h>
#include "ADS1115.h"

/** Set module i2c address */
ADS1115 adc0(ADS1115_ADDRESS_ADDR_SDA);

void setup() {    
    //I2Cdev::begin();  // join I2C bus
    Wire.begin();
    Serial.begin(115200); // initialize serial communication 
    
    Serial.println("Testing device connections...");
    Serial.println(adc0.testConnection() ? "ADS1115 connection successful" : "ADS1115 connection failed");
    
    adc0.initialize(); // initialize ADS1115 16 bit A/D chip

    // We're going to do single shot sampling
    adc0.setMode(ADS1115_MODE_SINGLESHOT);
    
    // Slow things down so that we can see that the "poll for conversion" code works
    adc0.setRate(ADS1115_RATE_32);   
    // Set the gain (PGA) +/- 6.144v
    // Set ADS1115_PGA_4P096 if u use 3.3v
    // Note that any analog input must be higher than –0.3V and less than VDD +0.3
    adc0.setGain(ADS1115_PGA_6P144);

    adc0.setConversionReadyPinMode();
}

void loop() {
       
    adc0.setMultiplexer(ADS1115_MUX_P1_NG); //select multiplexer (ie which signal to feed into ADC for comparison).
    adc0.triggerConversion();
    int16_t raw1 = adc0.getConversion();  

    adc0.setMultiplexer(ADS1115_MUX_P2_NG);
    adc0.triggerConversion();
    int16_t raw2 = adc0.getConversion();  

    Serial.print("ADC A1: ");
    Serial.print(raw1);
    Serial.print("\t");  

    Serial.print("ADC A2: ");
    Serial.print(raw2);
    Serial.println();   

    delay(500);
}
  

  

