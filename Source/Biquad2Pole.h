#pragma once
#ifndef INFS_BIQUAD2POLE_H
#define INFS_BIQUAD2POLE_H

namespace infrasonic {

// Single precision 2nd order Butterworth biquad filter
class Biquad2Pole {

    public:

        enum class FilterType {
            LowPass,
            HighPass   
        };

        Biquad2Pole() {}
        ~Biquad2Pole() {}

        void Init(const float sample_rate);

        void SetFilterType(const FilterType type);

        void SetCutoff(const float cutoff_hz);

        void SetQ(const float q);

        inline float Process(const float in)
        {
            // TODO: arm accelerated version

            // Transposed direct form 2
            float y = b0_ * in + s1_;
            s1_ = s2_ + in * b1_ - a1_ * y;
            s2_ = b2_ * in - a2_ * y;
            return y;
        }

    private:

        float sample_rate_, Ts_;

        FilterType filter_type_ = FilterType::LowPass;
        float cutoff_hz_, q_;

        // coefs
        float b0_, b1_, b2_, a1_, a2_;

        // state
        float s1_{0}, s2_{0};

        void updateCoefficients();
};

}

#endif