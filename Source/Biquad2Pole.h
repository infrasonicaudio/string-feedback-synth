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

        float Process(const float in);

    private:

        float sample_rate_, Ts_;

        FilterType filter_type_ = FilterType::LowPass;
        float cutoff_hz_, q_;

        // coefs
        float b0_, b1_, b2_, a1_, a2_;

        // state
        // float x_1_, y_1_, s1_1_, s2_1_;

        void updateCoefficients();
};

}

#endif