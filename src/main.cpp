#include <Arduino.h>
#include <Timer1SignalGenerator.h>
#include <Waveform.h>

Timer1SignalGenerator digitalSignalGen{};

unsigned long lastSend = 0;
unsigned long sendInterval = 500;

void setup() {
  Serial.begin(9600);

  digitalSignalGen.initialize();

  // digitalSignalGen.outputSquareWave(25000);

  // Serial.println(digitalSignalGen.getTimer1Prescale());
  // Serial.println(digitalSignalGen.getOutputFrequency());
  // Serial.println(digitalSignalGen.getOutputDutycycle());
  delay(1000);

  digitalSignalGen.chirp(25000, 100000, 30);

}

void loop() {
  digitalSignalGen.tick();

  if (millis() - lastSend >= sendInterval) {
    Serial.println(digitalSignalGen.getOutputFrequency());
    Serial.println((int)digitalSignalGen.getOutputWaveForm());
    Serial.println((int)digitalSignalGen.getFrequencyMode());
    lastSend = millis();
  }
}

