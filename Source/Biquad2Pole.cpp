#include "Biquad2Pole.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <daisysp.h>

// #define BQ_COSF(x) cosf(x)
// #define BQ_SINF(x) sinf(x)

using namespace infrasonic;

void Biquad2Pole::Init(float sample_rate)
{
    sample_rate_ = sample_rate;
    Ts_ = 1.0f / sample_rate;
    cutoff_hz_ = sample_rate * 0.49f;
    q_ = 0.70710678118f; // sqrt(2)/2 = "flat"
    updateCoefficients();
}

void Biquad2Pole::SetFilterType(const FilterType type)
{
    filter_type_ = type;
    updateCoefficients();
}

void Biquad2Pole::SetCutoff(const float cutoff_hz)
{
    cutoff_hz_ = daisysp::fclamp(cutoff_hz, 1.f, sample_rate_ * 0.5f);
    updateCoefficients();
}

void Biquad2Pole::SetQ(const float q)
{
    q_ = daisysp::fmax(q, 0.1f);
    updateCoefficients();
}

void Biquad2Pole::updateCoefficients()
{
    float norm;
    const float K = tanf(PI_F * cutoff_hz_ * Ts_);
    const float Ksq = K * K;

    switch (filter_type_) {
        default: // lowpass
            norm = 1.0f / (1.0f + (K / q_) + Ksq);
            b0_ = Ksq * norm;
            b1_ = 2.0f * b0_;
            b2_ = b0_;
            a1_ = 2.0f * (Ksq - 1.0f) * norm;
            a2_ = (1.0f - (K / q_) + Ksq) * norm; 
            break;
    }

    std::cout << std::setprecision(5) 
        << "\nCoefs for f = " << cutoff_hz_ << " and q = " << q_ << "\n"
        << "  b0 = " << b0_ << "\n"
        << "  b1 = " << b1_ << "\n"
        << "  b2 = " << b2_ << "\n"
        << "  a1 = " << a1_ << "\n"
        << "  a2 = " << a2_ << "\n"
        << "\n";
}

