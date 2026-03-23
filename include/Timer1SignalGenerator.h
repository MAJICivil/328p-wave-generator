#include <Arduino.h>
#include <Waveform.h>
#pragma once

/**
 * Models a signal generator that manipulates timer1 (OC1A) to create digital signals
 */
class Timer1SignalGenerator {
    DigitalWaveForm outputWaveForm = DigitalWaveForm::None;
    FrequencyMode frequencyMode = FrequencyMode::Constant;
    uint16_t timer1Prescale = 0;

    float sweepStartFrequency;
    float sweepEndFrequency;
    float sweepDuration;
    uint32_t sweepStartTime_us;
    uint32_t sweepLastUpdate_us;
    
    public:
    static constexpr uint8_t OC1A_PIN = 9;
    static constexpr uint32_t SQUARE_UPPERBOUND_hz = 4000000;
    static constexpr float SQUARE_LOWERBOUND_hz = 0.12;
    static constexpr uint32_t PULSE_UPPERBOUND_hz = 1000000;
    static constexpr float PULSE_LOWERBOUND_hz = 0.24;
    static constexpr uint32_t TOP_UPPERBOUND = (uint32_t)1 << 16;
    static constexpr uint8_t TOP_LOWERBOUND = 2;
    static constexpr uint32_t SWEEP_UPDATE_INTERVAL_us = 50;

    
    void initialize();

    /* Outputs a square wave with F_out = frequency */
    void outputSquareWave(float frequency);
    /* Outputs a pulse wave with F_out = frequency and d = dutycycle */
    void outputPulseWave(float frequency, float dutycycle);
    /* Outputs a square wave linear sweep from startFrequency to endFrequency over time in seconds */
    void linearSweep(float startFrequency, float endFrequency, float time);
    /* Outputs a square wave log sweep (f(t) = f0 * e^(ln(f1/f0) * t * 1/T)) from startFrequency to endFrequency over time in seconds */
    void logSweep(float startFrequency, float endFrequency, float time);
    /* Must be called frequently when using time variable frequencies */
    void tick();
    void stop();

    inline DigitalWaveForm getOutputWaveForm() const { return outputWaveForm; };
    inline FrequencyMode getFrequencyMode() const { return frequencyMode; };
    inline float getOutputFrequency() const { 
        if (timer1Prescale == 0.0) return NAN;
        return outputWaveForm == DigitalWaveForm::Square ? (float)F_CPU / ((uint32_t)2 * timer1Prescale * (1 + OCR1A)) : (float)F_CPU / ((uint32_t)timer1Prescale * (1 + ICR1)); };
    inline float getOutputDutycycle() const { 
        if (outputWaveForm == DigitalWaveForm::None) return NAN;
        return outputWaveForm == DigitalWaveForm::Pulse ?  (OCR1A + 1) / (float)ICR1 : 0.5; };
    inline uint16_t getTimer1Prescale() const { return timer1Prescale; }
    
    private:

    inline uint16_t computeOCR1A(float frequency) const { return round(F_CPU / (2 * timer1Prescale * frequency) - 1); };
    inline uint16_t computeTOP(float frequency) const { return round(F_CPU / (timer1Prescale * frequency) - 1); };
    uint16_t getPrescaleForFrequency(float frequency, DigitalWaveForm waveform);
    bool setPrescaler(uint16_t prescale);
    bool checkAndSetPrescalerForFrequency(float frequency, DigitalWaveForm waveform);
    inline void resetTimer1() { TCCR1A = 0; TCCR1B = 0; TCNT1 = 0; OCR1A = 0; ICR1 = 0; }
    inline float getFrequencyForPrescale(uint16_t prescale, uint32_t top, DigitalWaveForm waveform) const { 
        float base = (float)F_CPU / (prescale * top);
        return waveform == DigitalWaveForm::Pulse ? base : base / 2; };
    void updateSquareWaveFrequency(float frequency);
    void setupSweep(float startFrequency, float endFrequency, float duration);
    float getLinearSweepFrequency(float delta);
    float getLogSweepFrequency(float delta);
};