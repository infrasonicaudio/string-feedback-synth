#include "FeedbackSynthEngine.h"

using namespace infrasonic;
using namespace daisysp;

void FeedbackSynthEngine::Init(const float sample_rate)
{
    noise_.Init();
    noise_.SetAmp(0.1f);
    // noise_.SetAmp(0.000001f); // TODO dbfs to linear helper

    strings_[0].Init(sample_rate);
    strings_[1].Init(sample_rate);

    fb_delay_[0].Init();
    fb_delay_[1].Init();

    // TEMP
    strings_[0].SetBrightness(0.5f);
    strings_[0].SetNonLinearity(0.0f);
    strings_[0].SetFreq(mtof(40.0f));
    strings_[0].SetDamping(0.5f);
}

void FeedbackSynthEngine::Process(float *outL, float *outR)
{
    float samp = strings_[0].Process(noise_.Process());
    *outL = *outR = samp;
}