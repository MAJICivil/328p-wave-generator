#include <Arduino.h>

constexpr uint8_t signalOut = 9;

ISR(TIMER1_COMPA_vect) {

}

ISR(TIMER1_OVF_vect) {

}

void setup() {
  Serial.begin(9600);

  TCCR1A = 0;
  TCCR1B |= (1 << CS10);

}

void loop() {
  
}

