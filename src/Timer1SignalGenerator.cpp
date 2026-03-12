#include <Arduino.h>
#include <Timer1SignalGenerator.h>
#include <Waveform.h>

void Timer1SignalGenerator::initialize() {
    pinMode(OC1A_PIN, OUTPUT);
    digitalWrite(OC1A_PIN, LOW);

    stop();
}

void Timer1SignalGenerator::outputSquareWave(float frequency) {
    stop();
    outputWaveForm = DigitalWaveForm::Square;

    TCCR1A = (1 << COM1A0);
    TCCR1B = (1 << WGM12);
    setPrescaler(64);

    OCR1A = computeOCR1A(frequency); 
}

void Timer1SignalGenerator::outputPulseWave(float frequency, float dutycycle) {
    if (dutycycle <= 0.0 || dutycycle > 1) return;

    stop();
    outputWaveForm = DigitalWaveForm::Pulse;


    TCCR1A |= ((1 << COM1A1) | (1 << WGM11));
    TCCR1B |= ((1 << WGM12) | (1 << WGM13));
    setPrescaler(64);
    ICR1 = computeTOP(frequency);
    OCR1A = (ICR1 + 1) * dutycycle - 1;
    
}

void Timer1SignalGenerator::chirp(float startFrequency, float endFrequency, float time) {
    // TODO
}

void Timer1SignalGenerator::tick() {
    // TODO
}

void Timer1SignalGenerator::stop() {
    resetTimer1();
    timer1Prescale = 0;
    outputWaveForm = DigitalWaveForm::None;
    frequencyMode = FrequencyMode::Constant;
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

uint16_t Timer1SignalGenerator::getPrescaleForFrequency(float frequency) {
    if (outputWaveForm == DigitalWaveForm::Pulse) return 0;
    if (frequency < SQUARE_LOWERBOUND_hz || frequency > SQUARE_UPPERBOUND_hz) return 0;

    // TODO
    return 0;
}

