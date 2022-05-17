#pragma once
#ifndef IFS_FEEDBACK_SYNTH_ENGINE_H
#define IFS_FEEDBACK_SYNTH_ENGINE_H

#include <daisysp.h>

namespace infrasonic {

class FeedbackSynthEngine {

    public:
    
        enum class ParamName {
            StringFreq
        };

        FeedbackSynthEngine() {};
        ~FeedbackSynthEngine() {};

        FeedbackSynthEngine(const FeedbackSynthEngine &other) = delete;
        FeedbackSynthEngine(FeedbackSynthEngine &&other) = delete;
        FeedbackSynthEngine& operator=(const FeedbackSynthEngine &other) = delete;
        FeedbackSynthEngine& operator=(FeedbackSynthEngine &&other) = delete;

        void Init(const float sample_rate);

        void SetStringFreq(const float freq);

        void Process(float *outL, float *outR);

    private:
        daisysp::WhiteNoise noise_;
        daisysp::String strings_[2];
        daisysp::DelayLine<float, 12000> fb_delay_[2]; // long enough for 250ms at 48kHz
};

}

#endif
