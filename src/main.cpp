#include <Arduino.h>
#include <Timer1SignalGenerator.h>
#include <Waveform.h>

Timer1SignalGenerator digitalSignalGen{};

unsigned long lastSend = 0;
unsigned long sendInterval = 500;

void setup() {
  Serial.begin(9600);

  digitalSignalGen.initialize();

  //digitalSignalGen.outputSquareWave(1000000);
  //digitalSignalGen.outputPulseWave(100000, .1);

  //delay(1000);
  digitalSignalGen.chirp(1000, 100000, 15);

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

