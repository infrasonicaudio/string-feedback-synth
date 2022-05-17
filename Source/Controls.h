#pragma once
#ifndef INFS_CONTROLS_H
#define INFS_CONTROLS_H

#include <functional>
#include <map>
#include <daisysp.h>

#if !TARGET_MACOS
#include <per/adc.h>
#endif

namespace infrasonic {

template<typename ParamId>
class Controls {
    
    public:

        using ParamCallback = std::function<void(const float)>;

        Controls() {};
        ~Controls() {};

        void Init(const float sample_rate, const size_t block_size)
        {
            control_rate_ = sample_rate / static_cast<float>(block_size);
        }

        void Register(const ParamId id, const ParamCallback callback, const float initial_value = 0.0f, const float smooth_time = 0.05f)
        {
            ParamState state(initial_value, smooth_coef(smooth_time, control_rate_), callback);
            param_states_.insert({id, state});
        }

        void Update(const ParamID id, const float value, const bool immediate = false)
        {

        }

        void Process()
        {
            for (auto &param : param_states_) {

                auto &state = param.second;

                // apply smoothing
                if (state.current != state.target) {
                    if (fabsf(state.current - state.target) < kParamSmoothingThresh) {
                        state.current = state.target;
                    } else {
                        daisysp::fonepole(state.current, state.target, state.smooth_coef);
                    }
                }

                state.callback(state.current);
            }
        }

        #if TARGET_MACOS

        // MIDI Handler
        static void NoteOn(unsigned char channel, unsigned char nn, unsigned char velocity) {};
        static void NoteOff(unsigned char channel, unsigned char nn, unsigned char velocity) {};
        static void ControlChange(unsigned char channel, unsigned char cc, unsigned char value) {};

        #else // Daisy Seed

        // static void InitializeValues(daisy::AdcHandle &adc);
        // static void UpdateValues(daisy::AdcHandle &adc);

        #endif
    
    private:

        static constexpr float kParamSmoothingThresh = 0.001f;

        struct ParamState {
            float current;
            float target;
            float smooth_coef;
            ParamCallback callback;

            ParamState() = delete;
            ParamState(const float initial, const float smooth_coef, const ParamCallback callback)
                : current(initial)
                , target(initial)
                , smooth_coef(smooth_coef)
                , callback(callback)
            {}
        };

        using ParamStates = std::map<ParamId, ParamState>;

        float control_rate_{};
        ParamStates param_states_{};

        // TODO: could be in a generalized utility file
        static float smooth_coef(float time, float sample_rate)
        {
            if (time == 0.0f || sample_rate == 0.0f)
                return 1.0f;

            return 1.0f / (time * sample_rate);
        };
};

}

#endif