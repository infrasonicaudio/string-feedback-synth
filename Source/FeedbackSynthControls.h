#pragma once
#ifndef INFS_FEEDBACKSYNTHCONTROLS_H
#define INFS_FEEDBACKSYNTHCONTROLS_H

#include "Controls.h"
#include "FeedbackSynthEngine.h"

namespace infrasonic {
    
enum class FeedbackSynthParam {
    StringFreq
};

using FeedbackSynthControls = Controls<FeedbackSynthParam>;

static void register_feedbacksynth_controls(FeedbackSynthControls &controls, FeedbackSynthEngine &engine) {
    controls.Register(FeedbackSynthParam::StringFreq, [&](float value){ engine.SetStringFreq(value); }, daisysp::mtof(40));
}

}

#endif