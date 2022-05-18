#pragma once
#ifndef INFS_BIQUAD2POLE_H
#define INFS_BIQUAD2POLE_H

#include <cassert>

namespace infrasonic {

/**
 *  Single precision 2nd order Butterworth biquad filter
 *  with support for mono or stereo sample-by-sample-processing.
 *
 *  TODO: Support for higher order / cascading
 */
class Biquad2Pole {

    public:

        enum class FilterType {
            LowPass,
            HighPass   
        };

        Biquad2Pole() {}
        ~Biquad2Pole() {}

        void Init(const float sample_rate);

        // Individual param update methods will recalculate coefficients every time
        void SetFilterType(const FilterType type);
        void SetCutoff(const float cutoff_hz);
        void SetQ(const float q);

        inline float Process(const float in)
        {
            return process(in, 0);
        }

        // In-place stereo processing
        inline void ProcessStereo(float &sampL, float &sampR)
        {
            sampL = process(sampL, 0);
            sampR = process(sampR, 1);
        }

    private:

        float sample_rate_, Ts_;

        FilterType filter_type_ = FilterType::LowPass;
        float cutoff_hz_, q_;

        // coefs
        float b0_, b1_, b2_, a1_, a2_;

        // state
        float s1_[2] = {0, 0};
        float s2_[2] = {0, 0};

        void updateCoefficients();

        inline float process(const float in, const int channel)
        {
            assert(channel < 2);

            // TODO: arm accelerated version

            // Transposed direct form 2
            float y = b0_ * in + s1_[channel];
            s1_[channel] = s2_[channel] + in * b1_ - a1_ * y;
            s2_[channel] = b2_ * in - a2_ * y;
            return y;
        }
};

}

#endif