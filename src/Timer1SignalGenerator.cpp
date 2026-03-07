#include <Arduino.h>
#include <Timer1SignalGenerator.h>
#include <Waveform.h>

void Timer1SignalGenerator::initialize() {
    pinMode(OC1A_PIN, OUTPUT);
    digitalWrite(OC1A_PIN, LOW);
    outputWaveForm = DigitalWaveForm::None;

    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
}

void Timer1SignalGenerator::outputSquareWave(float frequency) {
    outputWaveForm = DigitalWaveForm::Square;

    //TODO: add support for user defined frequency

    TCCR1A = (1 << COM1A0);
    TCCR1B = (1 << WGM12);
    OCR1A = 124; // 1khz square wave

    TCCR1B |= (1 << CS11) | (1 << CS10);
}

void Timer1SignalGenerator::stop() {
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;

    outputWaveForm = DigitalWaveForm::None;
}
