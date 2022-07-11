#pragma once
#ifndef INFS_FEEDBACKSYNTHCONTROLS_H
#define INFS_FEEDBACKSYNTHCONTROLS_H

#include <daisy.h>
#include <daisy_seed.h>
#include "FeedbackSynthEngine.h"
#include "ParameterRegistry.h"

namespace infrasonic {
namespace FeedbackSynth {

class Controls {

public:

    Controls() = default;
    ~Controls() = default;

    void Init(daisy::DaisySeed &hw, Engine &engine);

    void Update(daisy::DaisySeed &hw);

    void Process() {
        params_.Process();
    }

private:

    const size_t kNumAdcChannels = 1;

    enum PinNumber : uint8_t {
        ADC_CH0 = 15,
        MUX0_ADR0 = 1,
        MUX0_ADR1 = 2,
        MUX0_ADR2 = 3
    };
        
    /// Identifies a parameter of the synth engine
    enum class Parameter {
        StringPitch,
        FeedbackGain,
        FeedbackDelay,
        FeedbackLPFCutoff,
        FeedbackHPFCutoff,
        EchoDelayTime,
        EchoDelayFeedback,
        EchoDelaySend
    };

    using Parameters = ParameterRegistry<Parameter>;

    Parameters params_;

    void initADCs(daisy::DaisySeed &hw);
    void registerParams(Engine &engine);
};


}
}

#endif