#include <Arduino.h>
#include <Waveform.h>
#pragma once

/**
 * Models a signal generator that manipulates timer1 to create square and pulse waves
 */
class Timer1SignalGenerator {
    DigitalWaveForm outputWaveForm;

    
    public:

    Timer1SignalGenerator(DigitalWaveForm outputWave);
    inline DigitalWaveForm getOutputWaveForm() const { return outputWaveForm; };
};