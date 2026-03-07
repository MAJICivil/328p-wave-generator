#include<Arduino.h>
#include<Timer1SignalGenerator.h>

Timer1SignalGenerator::Timer1SignalGenerator(DigitalWaveForm outputWave) {
    outputWaveForm = outputWave;  
}

void Timer1SignalGenerator::initialize() {
    pinMode(OC1A_PIN, OUTPUT);
    digitalWrite(OC1A_PIN, LOW);

    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;

    TCCR1A = (1 << COM1A0);
    TCCR1B = (1 << WGM12);
    OCR1A = 124; // 1khz square wave

    TCCR1B |= (1 << CS11) | (1 << CS10);

}
