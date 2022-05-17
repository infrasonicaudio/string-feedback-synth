#include "FeedbackSynthEngine.h"

using namespace infrasonic;
using namespace daisysp;

void FeedbackSynthEngine::Init(const float sample_rate)
{
    noise_.Init();
    noise_.SetAmp(0.1f);
    // noise_.SetAmp(0.000001f); // TODO dbfs to linear helper

    for (unsigned int i=0; i<2; i++) {
        strings_[i].Init(sample_rate);
        fb_delay_[i].Init();

        // TODO: tweak these?
        strings_[i].SetBrightness(0.5f);
        strings_[i].SetNonLinearity(0.0f);
        strings_[i].SetFreq(mtof(40.0f));
        strings_[i].SetDamping(0.5f);
    }
}

void FeedbackSynthEngine::SetStringPitch(const float nn)
{
    const auto freq = mtof(nn);
    strings_[0].SetFreq(freq);
    strings_[1].SetFreq(freq);
}

void FeedbackSynthEngine::Process(float *outL, float *outR)
{
    const float noise_samp = noise_.Process();

    float sampL = strings_[0].Process(noise_samp);
    float sampR = strings_[1].Process(noise_samp);

    *outL = sampL;
    *outR = sampR;
}