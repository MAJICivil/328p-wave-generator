#include <Arduino.h>
#include <Timer1SignalGenerator.h>
#include <Waveform.h>

Timer1SignalGenerator digitalSignalGen{};

void setup() {
  Serial.begin(9600);

  digitalSignalGen.initialize();

  delay(5000);
  digitalSignalGen.outputSquareWave(1000);
}

void loop() {
  
}

