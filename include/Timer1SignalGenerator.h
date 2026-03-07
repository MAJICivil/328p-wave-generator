#include <Arduino.h>
#include <Waveform.h>
#pragma once



/**
 * Models a signal generator that manipulates timer1 to create square and pulse waves
 */
class Timer1SignalGenerator {
    DigitalWaveForm outputWaveForm;
    
    
    public:
    static constexpr uint8_t OC1A_PIN = 9;
    
    Timer1SignalGenerator(DigitalWaveForm outputWave);
    void initialize();
    inline DigitalWaveForm getOutputWaveForm() const { return outputWaveForm; };
};