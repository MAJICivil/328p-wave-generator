#include <Arduino.h>
#include <Timer1SignalGenerator.h>
#include <Waveform.h>

Timer1SignalGenerator digitalSignalGen(DigitalWaveForm::Square);

void setup() {
  Serial.begin(9600);

  digitalSignalGen.initialize();
}

void loop() {
  
}

