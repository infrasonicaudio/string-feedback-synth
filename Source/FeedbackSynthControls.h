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
    FeedbackDelay
};

using Controls = ControlsRegistry<ControlParam>;

static void register_controls(Controls &controls, Engine &engine) {
    using namespace std::placeholders;

    // Pitch as nn
    controls.Register(ControlParam::StringPitch, 40.0f, 20.0f, 80.0f, std::bind(&Engine::SetStringPitch, &engine, _1), 0.2f);

    // Feedback Gain in dbFS
    controls.Register(ControlParam::FeedbackGain, -50.0f, -50.0f, 6.0f, std::bind(&Engine::SetFeedbackGain, &engine, _1));

    // Feedback delay in seconds
    controls.Register(ControlParam::FeedbackDelay, 0.001f, 0.001f, 0.2f, std::bind(&Engine::SetFeedbackDelay, &engine, _1));
}

}
}

#endif