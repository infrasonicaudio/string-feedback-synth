#pragma once
#ifndef INFS_FEEDBACKSYNTHMIDIHANDLER_H
#define INFS_FEEDBACKSYNTHMIDIHANDLER_H

#include "FeedbackSynthControls.h"

namespace infrasonic {
namespace FeedbackSynth {

class MIDIHandler {

    public:
        MIDIHandler() = delete;

        static void Init(Parameters* const params)
        {
            params_ = params;
        }

        static void NoteOn(unsigned char channel, unsigned char nn, unsigned char velocity)
        {
            assert(params_ != nullptr);
            params_->Update(Parameter::StringPitch, nn, true);
        }

        static void NoteOff(unsigned char channel, unsigned char nn, unsigned char velocity)
        {
            assert(params_ != nullptr);
        }

        static void ControlChange(unsigned char channel, unsigned char cc, unsigned char value)
        {
            assert(params_ != nullptr);
            const auto value_norm = static_cast<float>(value) / 127.0f;
            switch (CCParam(cc)) {

                case CCParam::FeedbackGain:
                    params_->UpdateNormalized(Parameter::FeedbackGain, value_norm);
                    break; 

                case CCParam::FeedbackDelay:
                    params_->UpdateNormalized(Parameter::FeedbackDelay, value_norm, false, daisysp::Mapping::EXP);
                    break; 

                case CCParam::StringPitch:
                    params_->UpdateNormalized(Parameter::StringPitch, value_norm);
                    break;

                case CCParam::FeedbackLPFCutoff:
                    params_->UpdateNormalized(Parameter::FeedbackLPFCutoff, value_norm, false, daisysp::Mapping::EXP);
                    break;

                case CCParam::FeedbackHPFCutoff:
                    params_->UpdateNormalized(Parameter::FeedbackHPFCutoff, value_norm, false, daisysp::Mapping::EXP);
                    break;

                case CCParam::EchoDelayTime:
                    params_->UpdateNormalized(Parameter::EchoDelayTime, value_norm, false, daisysp::Mapping::EXP);
                    break;

                case CCParam::EchoDelayFeedback:
                    params_->UpdateNormalized(Parameter::EchoDelayFeedback, value_norm);
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
            FeedbackHPFCutoff = 106,
            EchoDelayTime   = 28,
            EchoDelayFeedback = 24
        };

        inline static Parameters *params_ = nullptr;
};

// namespace
}
}

#endif