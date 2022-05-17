#pragma once
#ifndef INFS_FEEDBACKSYNTHCONTROLS_H
#define INFS_FEEDBACKSYNTHCONTROLS_H

#include <functional>
#include "ControlsRegistry.h"
#include "FeedbackSynthEngine.h"

namespace infrasonic {
    
enum class FeedbackSynthParam {
    StringPitch
};

using FeedbackSynthControls = ControlsRegistry<FeedbackSynthParam>;

static void register_feedbacksynth_controls(FeedbackSynthControls &controls, FeedbackSynthEngine &engine) {
    using namespace std::placeholders;
    controls.Register(FeedbackSynthParam::StringPitch, 40.0f, 20.0f, 80.0f, std::bind(&FeedbackSynthEngine::SetStringPitch, &engine, _1));
}

}

#endif