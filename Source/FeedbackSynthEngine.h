#pragma once
#ifndef IFS_FEEDBACK_SYNTH_ENGINE_H
#define IFS_FEEDBACK_SYNTH_ENGINE_H

#include <daisysp.h>
#ifdef __arm__
#include <dev/sdram.h>
#endif

namespace infrasonic {
namespace FeedbackSynth {

class Engine {

    public:

        // Singleton instance of synthesis engine.
        // Implemented this way so the engine can own + use a static
        // echo delay object with a long delay line, allocated in SDRAM
        static Engine& instance() {
            static Engine engine;
            return engine;
        }

        Engine(const Engine &other) = delete;
        Engine(Engine &&other) = delete;
        Engine& operator=(const Engine &other) = delete;
        Engine& operator=(Engine &&other) = delete;

        void Init(const float sample_rate);

        void SetStringPitch(const float nn);

        void SetFeedbackGain(const float gain_dbfs);

        void SetFeedbackDelay(const float delay_s);

        void SetFeedbackHPFCutoff(const float cutoff_hz);

        // void SetFeedbackLPFCutoff(const float cutoff_hz);

        void Process(float *outL, float *outR);

    private:
        // long enough for 250ms at 48kHz
        static constexpr size_t kMaxFeedbackDelaySamp = 12000;

        float sample_rate_;
        float fb_gain_ = 0.0f;

        float fb_delay_smooth_coef_;
        float fb_delay_samp_ = 64.f;
        float fb_delay_samp_target_ = 64.f;

        daisysp::WhiteNoise noise_;
        daisysp::String strings_[2];
        daisysp::DelayLine<float, kMaxFeedbackDelaySamp> fb_delayline_[2];

        // TODO: Write generic 2-pole biquad filters
        daisysp::ATone fb_hpf_[2];
        // daisysp::Biquad fb_lpf_[2];

        Engine() {};
        ~Engine() {};
};

}
}

#endif
