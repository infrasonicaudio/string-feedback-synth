#pragma once
#ifndef INFS_FEEDBACKSYNTHMIDIHANDLER_H
#define INFS_FEEDBACKSYNTHMIDIHANDLER_H

namespace infrasonic {

class FeedbackSynthMIDIHandler {

    public:
        FeedbackSynthMIDIHandler() = delete;

        static void Init(FeedbackSynthControls * const controls)
        {
            controls_ = controls;
        }

        static void NoteOn(unsigned char channel, unsigned char nn, unsigned char velocity)
        {
            assert(controls_ != nullptr);
        }

        static void NoteOff(unsigned char channel, unsigned char nn, unsigned char velocity)
        {
            assert(controls_ != nullptr);
        }

        static void ControlChange(unsigned char channel, unsigned char cc, unsigned char value)
        {
            assert(controls_ != nullptr);
            const auto value_norm = static_cast<float>(value) / 127.0f;
            switch (cc) {
                case 102: {
                    float nn = daisysp::fmap(value_norm, 20.0f, 60.0f);
                    controls_->Update(FeedbackSynthParam::StringFreq, daisysp::mtof(nn));
                    break;
                }
                
                default:
                    break;
            }
        }

    private:

        inline static FeedbackSynthControls *controls_ = nullptr;
};

}

#endif