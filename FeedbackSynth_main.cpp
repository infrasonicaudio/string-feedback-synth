#include <daisy_seed.h> 
#include "FeedbackSynthEngine.h"
#include "FeedbackSynthControls.h"

// TODO: This is just a placeholder file until the engine code is developed

using namespace infrasonic;
using namespace daisy;
using namespace daisysp;

static const auto kSampleRate = SaiHandle::Config::SampleRate::SAI_48KHZ;
static const size_t kBlockSize = 4;

static DaisySeed hw;
static FeedbackSynth::Engine engine;
static FeedbackSynth::Controls controls;

// --- TODO: Move to dedicated class ---

#define NUM_ADC_CONTROLS 2

// pin numbers
enum class ADCControls: uint8_t {
    StringPitch = 15,
    FeedbackGain = 16 
};

void InitHwControls()
{
    AdcChannelConfig config[NUM_ADC_CONTROLS];
    config[0].InitSingle(hw.GetPin(static_cast<uint8_t>(ADCControls::StringPitch)));
    config[1].InitSingle(hw.GetPin(static_cast<uint8_t>(ADCControls::FeedbackGain)));
    hw.adc.Init(config, 2);
    hw.adc.Start();
}

void UpdateHwControls()
{
    controls.UpdateNormalized(FeedbackSynth::ControlParam::StringPitch, hw.adc.GetFloat(0));
    controls.UpdateNormalized(FeedbackSynth::ControlParam::FeedbackGain, hw.adc.GetFloat(1));
}

// --- /TODO ---

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
    UpdateHwControls();
    controls.Process();
    for (size_t i=0; i<size; i++) {
        engine.Process(OUT_L[i], OUT_R[i]);
    }
}

int main(void)
{
    hw.Init();

    InitHwControls();

    hw.SetAudioSampleRate(kSampleRate);
    hw.SetAudioBlockSize(kBlockSize);
    engine.Init(hw.AudioSampleRate());
    controls.Init(hw.AudioSampleRate() / static_cast<float>(kBlockSize));
    FeedbackSynth::register_controls(controls, engine);

    hw.StartAudio(AudioCallback);

    while(1) {
        System::Delay(10);
    }

    return 0;
}
