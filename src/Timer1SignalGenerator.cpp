#include <Arduino.h>
#include <Timer1SignalGenerator.h>
#include <Waveform.h>

void Timer1SignalGenerator::initialize() {
    pinMode(OC1A_PIN, OUTPUT);
    digitalWrite(OC1A_PIN, LOW);
    outputWaveForm = DigitalWaveForm::None;
    timer1Prescale = 0;

    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
}

void Timer1SignalGenerator::outputSquareWave(float frequency) {
    outputWaveForm = DigitalWaveForm::Square;

    TCCR1A = (1 << COM1A0);
    TCCR1B = (1 << WGM12);
    setPrescaler(64);

    OCR1A = computeOCR1A(frequency); 
}

void Timer1SignalGenerator::stop() {
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;

    outputWaveForm = DigitalWaveForm::None;
}

bool Timer1SignalGenerator::setPrescaler(uint16_t prescale) {
    TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));

    switch (prescale)
    {
    case 1:
        TCCR1B |= (1 << CS10);
        break;
    case 8:
        TCCR1B |= (1 << CS11);
        break;
    case 64:
        TCCR1B |= (1 << CS11) | (1 << CS10);
        break;
    case 256:
        TCCR1B |= (1 << CS12);
        break;
    case 1024:
        TCCR1B |= (1 << CS12) | (1 << CS11) | (1 << CS10);
        break;
    default:
        return false;
        break;
    }

    timer1Prescale = prescale;
    return true;
}

