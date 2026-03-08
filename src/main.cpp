#include <Arduino.h>
#include <Timer1SignalGenerator.h>
#include <Waveform.h>

Timer1SignalGenerator digitalSignalGen{};

void setup() {
  Serial.begin(9600);

  digitalSignalGen.initialize();

  digitalSignalGen.outputSquareWave(100);
}

void loop() {
  
}

