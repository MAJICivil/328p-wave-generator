#include <Arduino.h>
#include <Timer1SignalGenerator.h>
#include <Waveform.h>

Timer1SignalGenerator digitalSignalGen{};

void setup() {
  Serial.begin(9600);

  digitalSignalGen.initialize();

  digitalSignalGen.outputPulseWave(25000, .66);
  Serial.println((int)digitalSignalGen.getOutputWaveForm());
  Serial.println(digitalSignalGen.getTimer1Prescale());
  Serial.println(digitalSignalGen.getOutputFrequency());
  Serial.println(digitalSignalGen.getOutputDutycycle());
  Serial.println(OCR1A);
}

void loop() {
  
}

