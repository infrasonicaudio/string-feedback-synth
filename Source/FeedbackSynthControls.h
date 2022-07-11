#pragma once
#ifndef INFS_FEEDBACKSYNTHCONTROLS_H
#define INFS_FEEDBACKSYNTHCONTROLS_H

#include <functional>
#include "FeedbackSynthEngine.h"
#include "ParameterRegistry.h"

namespace infrasonic {
namespace FeedbackSynth {
    
enum class Parameter {
    StringPitch,
    FeedbackGain,
    FeedbackDelay,
    FeedbackLPFCutoff,
    FeedbackHPFCutoff,
    EchoDelayTime,
    EchoDelayFeedback
};

using Parameters = ParameterRegistry<Parameter>;

static void RegisterParameters(Parameters &params, Engine &engine) {
    using namespace std::placeholders;

    // Pitch as nn
    params.Register(Parameter::StringPitch, 40.0f, 16.0f, 72.0f, std::bind(&Engine::SetStringPitch, &engine, _1), 0.2f);

    // Feedback Gain in dbFS
    params.Register(Parameter::FeedbackGain, -60.0f, -60.0f, 12.0f, std::bind(&Engine::SetFeedbackGain, &engine, _1));

    // Feedback delay in seconds
    params.Register(Parameter::FeedbackDelay, 0.001f, 0.001f, 0.2f, std::bind(&Engine::SetFeedbackDelay, &engine, _1));

    // Feedback filter cutoff in hz
    params.Register(Parameter::FeedbackLPFCutoff, 18000.0f, 100.0f, 18000.0f, std::bind(&Engine::SetFeedbackLPFCutoff, &engine, _1));
    params.Register(Parameter::FeedbackHPFCutoff, 250.0f, 32.0f, 2000.0f, std::bind(&Engine::SetFeedbackHPFCutoff, &engine, _1));

    // Delay time in s
    params.Register(Parameter::EchoDelayTime, 0.5f, 0.05f, 5.0f, std::bind(&Engine::SetEchoDelayTime, &engine, _1));

    // Delay feedback
    params.Register(Parameter::EchoDelayFeedback, 0.0f, 0.0f, 1.5f, std::bind(&Engine::SetEchoDelayFeedback, &engine, _1));
}

}
}

#endif