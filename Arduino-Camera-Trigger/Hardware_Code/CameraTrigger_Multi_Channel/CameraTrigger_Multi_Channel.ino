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
unsigned long start_time = 0;       // Start loop time in milliseconds.
unsigned long end_time = 0;         // End loop time in milliseconds.
int loop_time;                      // Elapsed time in loop.

void setup() {
  pinMode(channel_0, OUTPUT);       // Initialize pin 10 as a trigger output.
  pinMode(channel_1, OUTPUT);       // Same for pin 11.
  pinMode(LED_BUILTIN, OUTPUT);     // Same for LED_BUILTIN (pin 13) for visual feedback.

  Serial.begin(9600);               // Setup serial line to change freq.

  EEPROM.get(address_0, trig_freq_0);
  EEPROM.get(address_1, trig_freq_1);

  // Set cycle_millis as fraction of sec.
  cycle_chan_0 = int(1000 / trig_freq_0);
  cycle_chan_1 = int(1000 / trig_freq_1);
  
}

void loop() {
  start_time = millis();

    cycle_chan_0 = cycle_chan_0 - loop_time;
    if(cycle_chan_0 <= 0){
      Trigger(channel_0);
      cycle_chan_0 = int(1000 / trig_freq_0);
      // Serial.println(millis());
    }

    cycle_chan_1 = cycle_chan_1 - loop_time;
    if(cycle_chan_1 <= 0){
      Trigger(channel_1);
      cycle_chan_1 = int(1000 / trig_freq_1);
    }

    OutputSerial(trig_freq_0, trig_freq_1);
    UpdateFreq();
  delay(1);
  end_time = millis();
  loop_time = end_time - start_time;
  

}

void OutputSerial(float freq_0, float freq_1){
  // Only update serial output every half-sec at most.
    if(millis() % 500 == 0){
      Serial.print("Trigger freq chan 0: ");
      Serial.println(freq_0);
      Serial.print("Trigger freq chan 1: ");
      Serial.println(freq_1);
      Serial.println("Send new freq as float to update.");
      Serial.println("----------------");
    }
}

void UpdateFreq(){
    // Check if serial input is waiting in buffer.
    // If so, parse the first float value and set it
    // as the new trigger freq.
    float freq;
    float chan;
    
    if(Serial.available() > 0){
      chan = Serial.parseFloat();
      freq = Serial.parseFloat();
      Serial.println(chan);
      Serial.println(freq);
      if(chan == 0){
        EEPROM.put(address_0, freq);
      }
      if(chan == 1){
        EEPROM.put(address_1, freq);
      }
      EEPROM.get(address_0, trig_freq_0);
      EEPROM.get(address_1, trig_freq_1);
    }
}

void Trigger(int channel){
  digitalWrite(channel, HIGH);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1);
  digitalWrite(channel, LOW);
  digitalWrite(LED_BUILTIN, LOW);
}

