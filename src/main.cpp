#include <Arduino.h>
#include <Timer1SignalGenerator.h>
#include <Waveform.h>

Timer1SignalGenerator digitalSignalGen{}; // Operates on OC1A (pin 9)

unsigned long lastSend = 0;
unsigned long sendInterval = 500;

void setup() {
  Serial.begin(9600);

  digitalSignalGen.initialize();

  //digitalSignalGen.outputSquareWave(1000000);
  //digitalSignalGen.outputPulseWave(8267, .1);

  ///digitalSignalGen.linearSweep(100, 150000, 15);
  digitalSignalGen.logSweep(150000, 100, 15);
}

void loop() {
  digitalSignalGen.tick();

  if (millis() - lastSend >= sendInterval) {
    Serial.println(digitalSignalGen.getOutputFrequency());
    Serial.println(digitalSignalGen.getOutputDutycycle());
    Serial.println(toString(digitalSignalGen.getOutputWaveForm()));
    lastSend = millis();
  }
}

