#pragma once
#ifndef INFS_FEEDBACKSYNTHCONTROLS_H
#define INFS_FEEDBACKSYNTHCONTROLS_H

#include <functional>
#include "ControlsRegistry.h"
#include "FeedbackSynthEngine.h"

namespace infrasonic {
namespace FeedbackSynth {
    
enum class ControlParam {
    StringPitch,
    FeedbackGain,
    FeedbackDelay,
    FeedbackLPFCutoff,
    FeedbackHPFCutoff,
    EchoDelayTime,
    EchoDelayFeedback
};

using Controls = ControlsRegistry<ControlParam>;

static void register_controls(Controls &controls, Engine &engine) {
    using namespace std::placeholders;

    // Pitch as nn
    controls.Register(ControlParam::StringPitch, 40.0f, 24.0f, 80.0f, std::bind(&Engine::SetStringPitch, &engine, _1), 0.2f);

    // Feedback Gain in dbFS
    controls.Register(ControlParam::FeedbackGain, -50.0f, -50.0f, 6.0f, std::bind(&Engine::SetFeedbackGain, &engine, _1));

    // Feedback delay in seconds
    controls.Register(ControlParam::FeedbackDelay, 0.001f, 0.001f, 0.2f, std::bind(&Engine::SetFeedbackDelay, &engine, _1));

    // Feedback filter cutoff in hz
    controls.Register(ControlParam::FeedbackLPFCutoff, 18000.0f, 100.0f, 18000.0f, std::bind(&Engine::SetFeedbackLPFCutoff, &engine, _1));
    controls.Register(ControlParam::FeedbackHPFCutoff, 250.0f, 32.0f, 8000.0f, std::bind(&Engine::SetFeedbackHPFCutoff, &engine, _1));

    // Delay time in s
    controls.Register(ControlParam::EchoDelayTime, 0.5f, 0.05f, 5.0f, std::bind(&Engine::SetEchoDelayTime, &engine, _1));

    // Delay feedback
    controls.Register(ControlParam::EchoDelayFeedback, 0.0f, 0.0f, 1.2f, std::bind(&Engine::SetEchoDelayFeedback, &engine, _1));
}

}
}

#endif