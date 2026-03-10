#include <Arduino.h>
#include <Timer1SignalGenerator.h>
#include <Waveform.h>

Timer1SignalGenerator digitalSignalGen{};

void setup() {
  Serial.begin(9600);

  digitalSignalGen.initialize();

  digitalSignalGen.outputPulseWave(1000, .2f);
  Serial.println(digitalSignalGen.getOutputDutycycle(), 4);
}

void loop() {
  
}

