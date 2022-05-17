#include "Controls.h"
#include <cassert>
#include <daisysp.h>
#include "FeedbackSynthEngine.h"

using namespace infrasonic;
using namespace daisysp;

FeedbackSynthEngine* Controls::pEngine_{nullptr};

Controls::ParamStates Controls::paramStates_{};

float Controls::smooth_coef(float time, float sample_rate)
{
    if (time == 0.0f || sample_rate == 0.0f)
        return 1.0f;

    return 1.0f / (time * sample_rate);
}

void Controls::Init(FeedbackSynthEngine *engine, const float sample_rate, const size_t block_size)
{
    pEngine_ = engine;

    // External controls are updated less frequently than audio rate
    const float control_rate = sample_rate / static_cast<float>(block_size);

    paramStates_.insert({ ParamName::StringFreq, ParamState(mtof(40), smooth_coef(0.1f, control_rate)) });
}

void Controls::Process()
{
    assert(pEngine_ != nullptr);

    for (auto &param : paramStates_) {

        auto &state = param.second;

        // apply smoothing
        if (state.current != state.target) {
           if (fabsf(state.current - state.target) < kParamSmoothingThresh) {
               state.current = state.target;
           } else {
               fonepole(state.current, state.target, state.smooth_coef);
           }
        }

        switch(param.first) {
            case ParamName::StringFreq:
                pEngine_->SetStringFreq(state.current);
                break;
        }
    }
}

#if TARGET_MACOS

void Controls::NoteOn(unsigned char channel, unsigned char nn, unsigned char velocity)
{
    assert(pEngine_ != nullptr);
}

void Controls::NoteOff(unsigned char channel, unsigned char nn, unsigned char velocity)
{
    assert(pEngine_ != nullptr);
}

void Controls::ControlChange(unsigned char channel, unsigned char cc, unsigned char value)
{
    assert(pEngine_ != nullptr);
    
    const float value_norm = static_cast<float>(value) / 127.f;

    switch (cc) {
        case 102: {
            const float nn = fmap(value_norm, 20, 60);
            paramStates_.at(ParamName::StringFreq).target = mtof(nn);
            break;
        }
        
        default:
            break;
    }
}

#endif