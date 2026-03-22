#pragma once

enum class DigitalWaveForm {
    None,
    Square,
    Pulse
};

enum class AnalogWaveForm {
    None,
    Saw,
    Triangle,
    Sine
};

enum class FrequencyMode {
    Constant,
    LinearChirp,
    LogChirp
};

constexpr uint32_t US_PER_SECOND = 1000000;

inline String toString(DigitalWaveForm waveform) {
    switch (waveform)
    {
    case DigitalWaveForm::None:
        return "None";
    case DigitalWaveForm::Square:
        return "Square";
    case DigitalWaveForm::Pulse:
        return "Pulse";
    default:
        return "Unknown";
    }
}