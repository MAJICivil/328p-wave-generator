#include<Arduino.h>
#include<Timer1SignalGenerator.h>

Timer1SignalGenerator::Timer1SignalGenerator(DigitalWaveForm outputWave) {
    outputWaveForm = outputWave;  
    TCCR1A = 0;
}