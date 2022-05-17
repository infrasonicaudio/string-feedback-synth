#pragma once
#ifndef INFS_CONTROLS_H
#define INFS_CONTROLS_H

#include <map>
#if !TARGET_MACOS
#include <per/adc.h>
#endif

namespace infrasonic {

class FeedbackSynthEngine;

class Controls {
    
    public:
        Controls() = delete;

        static void Init(FeedbackSynthEngine *engine, const float sample_rate, const size_t block_size);
        static void Process();

        #if TARGET_MACOS

        // MIDI Handler
        static void NoteOn(unsigned char channel, unsigned char nn, unsigned char velocity);
        static void NoteOff(unsigned char channel, unsigned char nn, unsigned char velocity);
        static void ControlChange(unsigned char channel, unsigned char cc, unsigned char value);

        #else // Daisy Seed

        // static void InitializeValues(daisy::AdcHandle &adc);
        // static void UpdateValues(daisy::AdcHandle &adc);

        #endif
    
    private:

        static constexpr float kParamSmoothingThresh = 0.001f;
    
        enum class ParamName {
            StringFreq
        };

        struct ParamState {
            float current;
            float target;
            float smooth_coef;

            ParamState() = delete;
            ParamState(float initial, float smooth_coef)
                : current(initial)
                , target(initial)
                , smooth_coef(smooth_coef)
            {}
        };

        using ParamStates = std::map<ParamName, ParamState>;

        static FeedbackSynthEngine *pEngine_;
        static ParamStates paramStates_;

        // TODO: could be in a generalized utility file
        static float smooth_coef(float time, float sample_rate);
};

}

#endif