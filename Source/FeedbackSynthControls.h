#pragma once
#ifndef INFS_FEEDBACKSYNTHCONTROLS_H
#define INFS_FEEDBACKSYNTHCONTROLS_H

#include <functional>
#include "ControlsRegistry.h"
#include "FeedbackSynthEngine.h"

namespace infrasonic {
namespace FeedbackSynth {
    
enum class ControlParam {
    StringPitch
};

using Controls = ControlsRegistry<ControlParam>;

static void register_controls(Controls &controls, Engine &engine) {
    using namespace std::placeholders;
    controls.Register(ControlParam::StringPitch, 40.0f, 20.0f, 80.0f, std::bind(&Engine::SetStringPitch, &engine, _1));
}

}
}

#endif