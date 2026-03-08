#include <Arduino.h>
#include <Waveform.h>
#pragma once



/**
 * Models a signal generator that manipulates timer1 to create square and pulse waves
 */
class Timer1SignalGenerator {
    DigitalWaveForm outputWaveForm = DigitalWaveForm::None;
    short x;
    
    public:
    static constexpr uint8_t OC1A_PIN = 9;
    static constexpr uint16_t PRESCALER = 64;
    
    void initialize();

    /* Outputs a square wave with F_out = frequency */
    void outputSquareWave(float frequency);
    void stop();

    inline DigitalWaveForm getOutputWaveForm() const { return outputWaveForm; };

    private:
    inline uint16_t computeOCR1A(float frequency) const { return F_CPU / (2 * PRESCALER * frequency) - 1; };

};