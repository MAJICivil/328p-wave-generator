#include <Arduino.h>
#include <Timer1SignalGenerator.h>
#include <Waveform.h>

Timer1SignalGenerator digitalSignalGen{};

void setup() {
  Serial.begin(9600);

  digitalSignalGen.initialize();

  digitalSignalGen.outputSquareWave(25000);

  Serial.println(digitalSignalGen.getTimer1Prescale());
  Serial.println(digitalSignalGen.getOutputFrequency());
  Serial.println(digitalSignalGen.getOutputDutycycle());

}

void loop() {
  
}

