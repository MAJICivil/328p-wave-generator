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
    TCCR1A = (1 << COM1A0);
    TCCR1B = (1 << WGM12);
    outputWaveForm = DigitalWaveForm::Square;

    updateSquareWaveFrequency(frequency);
}

void Timer1SignalGenerator::updateSquareWaveFrequency(float frequency) {
    if (outputWaveForm != DigitalWaveForm::Square) return;
    if (!checkAndSetPrescalerForFrequency(frequency, DigitalWaveForm::Square)) return;

    OCR1A = computeOCR1A(frequency); 
}

void Timer1SignalGenerator::outputPulseWave(float frequency, float dutycycle) {
    if (dutycycle <= 0.0 || dutycycle > 1) return;
    stop();

    TCCR1A |= ((1 << COM1A1) | (1 << WGM11));
    TCCR1B |= ((1 << WGM12) | (1 << WGM13));
    outputWaveForm = DigitalWaveForm::Pulse;

    if (!checkAndSetPrescalerForFrequency(frequency, DigitalWaveForm::Pulse)) return;

    ICR1 = computeTOP(frequency);
    OCR1A = round((ICR1 + 1) * dutycycle - 1);
}

void Timer1SignalGenerator::linearSweep(float startFrequency, float endFrequency, float time) {
    setupSweep(startFrequency, endFrequency, time);
    frequencyMode = FrequencyMode::LinearSweep;
}

void Timer1SignalGenerator::logSweep(float startFrequency, float endFrequency, float time) {
    setupSweep(startFrequency, endFrequency, time);
    frequencyMode = FrequencyMode::LogSweep;
}

void Timer1SignalGenerator::setupSweep(float startFrequency, float endFrequency, float duration) {
    outputSquareWave(startFrequency);
    if (outputWaveForm != DigitalWaveForm::Square) return;

    uint32_t now = micros();
    sweepStartFrequency = startFrequency;
    sweepEndFrequency = endFrequency;
    sweepDuration = duration;
    sweepStartTime_us = now;
    sweepLastUpdate_us = now;
}

void Timer1SignalGenerator::tick() {
    if (frequencyMode == FrequencyMode::Constant) return;

    uint32_t now = micros();
    if (now - sweepLastUpdate_us < SWEEP_UPDATE_INTERVAL_us) return;

    float delta = now - sweepStartTime_us;
    if (delta >= (sweepDuration * US_PER_SECOND)) { frequencyMode = FrequencyMode::Constant; stop(); return; }


    float nextFrequency = frequencyMode == FrequencyMode::LinearSweep ? getLinearSweepFrequency(delta) : getLogSweepFrequency(delta);
    
    updateSquareWaveFrequency(nextFrequency);
    sweepLastUpdate_us = now;

}

float Timer1SignalGenerator::getLinearSweepFrequency(float delta) {
    float nextFrequency = (sweepEndFrequency - sweepStartFrequency) / (sweepDuration);
    nextFrequency *= delta;
    nextFrequency /= US_PER_SECOND;
    nextFrequency += sweepStartFrequency;
    return nextFrequency;
}

float Timer1SignalGenerator::getLogSweepFrequency(float delta) {
    return sweepStartFrequency * exp(log(sweepEndFrequency / sweepStartFrequency) * delta / (sweepDuration * US_PER_SECOND));
}

void Timer1SignalGenerator::stop() {
    resetTimer1();
    timer1Prescale = 0;
    outputWaveForm = DigitalWaveForm::None;
    frequencyMode = FrequencyMode::Constant;
}

bool Timer1SignalGenerator::setPrescaler(uint16_t prescale) {
    TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
    timer1Prescale = 0;

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

bool Timer1SignalGenerator::checkAndSetPrescalerForFrequency(float frequency, DigitalWaveForm waveform) {
    uint16_t updatedPrescale = getPrescaleForFrequency(frequency, waveform);
    if (updatedPrescale == 0) { 
        Serial.print("Invalid Frequency: ");
        Serial.print(frequency, 2); 
        Serial.print(" for waveform: ");
        Serial.println(toString(waveform));
        stop(); 
        return false; 
    }

    if (timer1Prescale != updatedPrescale) {
        if (!setPrescaler(updatedPrescale)) { 
            Serial.print("Failed to set prescale to ");
            Serial.println(updatedPrescale);
            stop(); 
            return false; 
        } 
    }

    return true;
}

uint16_t Timer1SignalGenerator::getPrescaleForFrequency(float frequency, DigitalWaveForm waveForm) {
    if (waveForm == DigitalWaveForm::None) return 0;
    if (waveForm == DigitalWaveForm::Square && (frequency < SQUARE_LOWERBOUND_hz || frequency > SQUARE_UPPERBOUND_hz)) return 0;
    if (waveForm == DigitalWaveForm::Pulse && (frequency < PULSE_LOWERBOUND_hz || frequency > PULSE_UPPERBOUND_hz)) return 0;

    if (frequency >= getFrequencyForPrescale(1, TOP_UPPERBOUND, waveForm) && frequency <= getFrequencyForPrescale(1, TOP_LOWERBOUND, waveForm)) return 1;
    if (frequency >= getFrequencyForPrescale(8, TOP_UPPERBOUND, waveForm) && frequency <= getFrequencyForPrescale(8, TOP_LOWERBOUND, waveForm)) return 8;
    if (frequency >= getFrequencyForPrescale(64, TOP_UPPERBOUND, waveForm) && frequency <= getFrequencyForPrescale(64, TOP_LOWERBOUND, waveForm)) return 64;
    if (frequency >= getFrequencyForPrescale(256, TOP_UPPERBOUND, waveForm) && frequency <= getFrequencyForPrescale(256, TOP_LOWERBOUND, waveForm)) return 256;
    if (frequency >= getFrequencyForPrescale(1024, TOP_UPPERBOUND, waveForm) && frequency <= getFrequencyForPrescale(1024, TOP_LOWERBOUND, waveForm)) return 1024;

    return 0;
}

