#include "FeedbackSynthControls.h"
#include <functional>

using namespace infrasonic;
using namespace daisy;

void FeedbackSynth::Controls::Init(DaisySeed &hw, Engine &engine) {
    params_.Init(hw.AudioSampleRate() / hw.AudioBlockSize());
    initADCs(hw);
    registerParams(engine);
}

void FeedbackSynth::Controls::Update(DaisySeed &hw) {
    params_.UpdateNormalized(Parameter::StringPitch,        1.0f - hw.adc.GetMuxFloat(0, 0));
    params_.UpdateNormalized(Parameter::FeedbackGain,       1.0f - hw.adc.GetMuxFloat(0, 1));
    params_.UpdateNormalized(Parameter::FeedbackDelay,      1.0f - hw.adc.GetMuxFloat(0, 2));
    params_.UpdateNormalized(Parameter::FeedbackLPFCutoff,  1.0f - hw.adc.GetMuxFloat(0, 3));
    params_.UpdateNormalized(Parameter::FeedbackHPFCutoff,  1.0f - hw.adc.GetMuxFloat(0, 4));
    params_.UpdateNormalized(Parameter::EchoDelayTime,      1.0f - hw.adc.GetMuxFloat(0, 5));
    params_.UpdateNormalized(Parameter::EchoDelayFeedback,  1.0f - hw.adc.GetMuxFloat(0, 6));
    params_.UpdateNormalized(Parameter::EchoDelaySend,      1.0f - hw.adc.GetMuxFloat(0, 7));
}

void FeedbackSynth::Controls::initADCs(DaisySeed &hw) {
    AdcChannelConfig config[kNumAdcChannels];

    // First channel = multiplexed 8x
    config[0].InitMux(hw.GetPin(ADC_CH0), 8, hw.GetPin(MUX0_ADR0), hw.GetPin(MUX0_ADR1), hw.GetPin(MUX0_ADR2));

    hw.adc.Init(config, kNumAdcChannels);
    hw.adc.Start();
}

void FeedbackSynth::Controls::registerParams(Engine &engine) {
    using namespace std::placeholders;

    // Pitch as nn
    params_.Register(Parameter::StringPitch, 40.0f, 16.0f, 72.0f, std::bind(&Engine::SetStringPitch, &engine, _1), 0.2f);

    // Feedback Gain in dbFS
    params_.Register(Parameter::FeedbackGain, -60.0f, -60.0f, 12.0f, std::bind(&Engine::SetFeedbackGain, &engine, _1));

    // Feedback delay in seconds
    params_.Register(Parameter::FeedbackDelay, 0.001f, 0.001f, 0.2f, std::bind(&Engine::SetFeedbackDelay, &engine, _1), 0.2f, daisysp::Mapping::EXP);

    // Feedback filter cutoff in hz
    params_.Register(Parameter::FeedbackLPFCutoff, 18000.0f, 100.0f, 18000.0f, std::bind(&Engine::SetFeedbackLPFCutoff, &engine, _1), 0.05f, daisysp::Mapping::EXP);
    params_.Register(Parameter::FeedbackHPFCutoff, 250.0f, 32.0f, 2000.0f, std::bind(&Engine::SetFeedbackHPFCutoff, &engine, _1), 0.05f, daisysp::Mapping::EXP);

    // Echo Delay time in s
    params_.Register(Parameter::EchoDelayTime, 0.5f, 0.05f, 5.0f, std::bind(&Engine::SetEchoDelayTime, &engine, _1), 0.05f, daisysp::Mapping::EXP);

    // Echo Delay feedback
    params_.Register(Parameter::EchoDelayFeedback, 0.0f, 0.0f, 1.5f, std::bind(&Engine::SetEchoDelayFeedback, &engine, _1));

    // Echo Delay send
    params_.Register(Parameter::EchoDelaySend, 0.0f, 0.0f, 1.0f, std::bind(&Engine::SetEchoDelaySendAmount, &engine, _1), 0.05f, daisysp::Mapping::EXP);
}