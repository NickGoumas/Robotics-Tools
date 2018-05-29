/*
  Hardware trigger code for PointGrey Blackfly cameras.
*/

#include <EEPROM.h>

int channel_0 = 10;                 // Digital pin value for chan 0.
int channel_1 = 11;                 // Digital pin value for chan 1.

int address_0 = 1;                  // EEPROM address chan 0.
int address_1 = 5;                  // EEPROM address chan 1.
float trig_freq_0;                  // Trigger frequency chan 0. (Hz)
float trig_freq_1;                  // Trigger frequency chan 1. (Hz)
int cycle_chan_0;                   // Cycle length in milliseconds chan 0.
int cycle_chan_1;                   // Cycle length in milliseconds chan 1.
unsigned long start_time;           // Initialize start time.

void setup() {
  pinMode(channel_0, OUTPUT);       // Initialize pin 10 as a trigger output.
  pinMode(channel_1, OUTPUT);       // Same for pin 11.
  pinMode(LED_BUILTIN, OUTPUT);     // Same for LED_BUILTIN (pin 13) for visual feedback.

  Serial.begin(9600);               // Setup serial line to change freq.

  EEPROM.get(address_0, trig_freq_0);
  EEPROM.get(address_1, trig_freq_1);
  
}

void loop() {
  start_time = millis();
  // Set cycle_millis as fraction of sec.
  cycle_chan_0 = int(1000 / trig_freq_0);
  cycle_chan_1 = int(1000 / trig_freq_1);

  // Set triggers and LED to HIGH.
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(10, HIGH);
  digitalWrite(11, HIGH);
  delay(1);                          // Keep output HIGH for at least 1 milli.

  // Set triggers and LED to LOW.
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);

  while(start_time + cycle_millis > millis()){
    OutputSerial();
    UpdateFreq();
    }
    delay(1);
}


void OutputSerial(){
  // Only update serial output every half-sec at most.
    if(millis() % 500 == 0){
      Serial.print("Current trigger freq: ");
      Serial.println(trigger_freq);
      Serial.println("Send new freq as float to update.");
      //Serial.println(EEPROM.read(0));
      }
}

void UpdateFreq(int address, float freq){
    // Check if serial input is waiting in buffer.
    // If so, parse the first float value and set it
    // as the new trigger freq.
    if(Serial.available() > 0){
      freq = Serial.parseFloat();
      EEPROM.put(address, freq);
      }

      }


