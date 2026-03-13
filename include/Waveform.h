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
    Chirp
};

constexpr uint32_t US_PER_SECOND = 1000000;