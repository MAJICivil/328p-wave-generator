#include <Arduino.h>
#include <Waveform.h>
#pragma once



/**
 * Models a signal generator that manipulates timer1 to create digital signals
 */
class Timer1SignalGenerator {
    DigitalWaveForm outputWaveForm = DigitalWaveForm::None;
    FrequencyMode FrequencyMode = FrequencyMode::Constant;
    uint16_t timer1Prescale = 0;
    
    public:
    static constexpr uint8_t OC1A_PIN = 9;
    
    void initialize();

    // TODO: make signal outputs select prescale based on frequency

    /* Outputs a square wave with F_out = frequency */
    void outputSquareWave(float frequency);
    /* Outputs a pulse wave with F_out = frequency and d = dutycycle */
    void outputPulseWave(float frequency, float dutycycle);
    /* Outputs a square wave frequency sweep from startFrequency to endFrequency over time */
    void chirp(float startFrequency, float endFrequency, float time);
    /* Must be called frequently when using time variable frequencies */
    void tick();
    void stop();

    inline DigitalWaveForm getOutputWaveForm() const { return outputWaveForm; };
    inline float getOutputFrequency() const { return F_CPU / (2 * timer1Prescale * (1 + OCR1A)); };
    inline float getOutputDutycycle() const { return outputWaveForm == DigitalWaveForm::Pulse ?  (OCR1A + 1) / (float)ICR1 : NAN; };
 
    private:
    inline uint16_t computeOCR1A(float frequency) const { return F_CPU / (2 * timer1Prescale * frequency) - 1; };
    inline uint16_t computeTOP(float frequency) const { return F_CPU / (timer1Prescale * frequency) - 1; };
    bool setPrescaler(uint16_t prescale);
    inline void resetTimer1() { TCCR1A = 0; TCCR1B = 0; TCNT1 = 0; }

};