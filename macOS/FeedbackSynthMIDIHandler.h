#pragma once
#ifndef INFS_FEEDBACKSYNTHMIDIHANDLER_H
#define INFS_FEEDBACKSYNTHMIDIHANDLER_H

#include "FeedbackSynthControls.h"

namespace infrasonic {
namespace FeedbackSynth {

class MIDIHandler {

    public:
        MIDIHandler() = delete;

        static void Init(Controls * const controls)
        {
            controls_ = controls;
        }

        static void NoteOn(unsigned char channel, unsigned char nn, unsigned char velocity)
        {
            assert(controls_ != nullptr);
            controls_->Update(ControlParam::StringPitch, nn, true);
        }

        static void NoteOff(unsigned char channel, unsigned char nn, unsigned char velocity)
        {
            assert(controls_ != nullptr);
        }

        static void ControlChange(unsigned char channel, unsigned char cc, unsigned char value)
        {
            assert(controls_ != nullptr);
            const auto value_norm = static_cast<float>(value) / 127.0f;
            switch (CCParam(cc)) {

                case CCParam::FeedbackGain:
                    controls_->UpdateNormalized(ControlParam::FeedbackGain, value_norm);
                    break; 

                case CCParam::FeedbackDelay:
                    controls_->UpdateNormalized(ControlParam::FeedbackDelay, value_norm, false, daisysp::Mapping::EXP);
                    break; 

                case CCParam::StringPitch:
                    controls_->UpdateNormalized(ControlParam::StringPitch, value_norm);
                    break;

                case CCParam::FeedbackLPFCutoff:
                    controls_->UpdateNormalized(ControlParam::FeedbackLPFCutoff, value_norm, false, daisysp::Mapping::EXP);
                    break;

                case CCParam::FeedbackHPFCutoff:
                    controls_->UpdateNormalized(ControlParam::FeedbackHPFCutoff, value_norm, false, daisysp::Mapping::EXP);
                    break;
                
                default:
                    break;
            }
        }

    private:

        enum class CCParam: unsigned char {
            StringPitch     = 102,
            FeedbackGain    = 26,
            FeedbackDelay   = 29,
            FeedbackLPFCutoff = 105,
            FeedbackHPFCutoff = 106
        };

        inline static Controls *controls_ = nullptr;
};

// namespace
}
}

#endif