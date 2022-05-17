#pragma once
#ifndef INFS_CONTROLS_H
#define INFS_CONTROLS_H

#include <functional>
#include <map>
#include <daisysp.h>

namespace infrasonic {

/**
 * @brief 
 * Generic control parameter registry with built in update smoothing
 * 
 * @tparam ParamId Template parameter specifying the parameter ID type
 */
template<typename ParamId>
class Controls {
    
    public:

        using ParamHandler = std::function<void(const float)>;

        Controls() {};
        ~Controls() {};

        void Init(const float control_rate)
        {
            control_rate_ = control_rate;
        }

        void Register(const ParamId id, const ParamHandler handler, const float initial_value = 0.0f, const float smooth_time = 0.05f)
        {
            ParamState state(initial_value, smooth_coef(smooth_time, control_rate_), handler);
            param_states_.insert({id, state});
        }

        // Called in response to new control input. Updates target, does not apply value.
        void Update(const ParamId id, const float value, const bool immediate = false)
        {
            auto it = param_states_.find(id);
            if (it != param_states_.end()) {
                auto &state = it->second;
                state.target = value;
                if (immediate) {
                    state.current = value;
                }
            }
        }

        // Updates param values with smoothing and applies using handler.
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

                state.handler(state.current);
            }
        }
    
    private:

        static constexpr float kParamSmoothingThresh = 0.001f;

        struct ParamState {
            float current;
            float target;
            
            const float smooth_coef;
            const ParamHandler handler;

            ParamState() = delete;
            ParamState(const float initial, const float smooth_coef, const ParamHandler handler)
                : current(initial)
                , target(initial)
                , smooth_coef(smooth_coef)
                , handler(handler)
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