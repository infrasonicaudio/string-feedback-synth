#include "FeedbackSynthEngine.h"
#include "DSPUtils.h"

using namespace infrasonic::FeedbackSynth;
using namespace daisysp;

void Engine::Init(const float sample_rate)
{
    sample_rate_ = sample_rate;
    fb_delay_smooth_coef_ = onepole_coef(0.2f, sample_rate);

    noise_.Init();
    noise_.SetAmp(dbfs2lin(-90.0f));

    for (unsigned int i=0; i<2; i++) {

        strings_[i].Init(sample_rate);
        strings_[i].SetBrightness(0.95f);
        strings_[i].SetNonLinearity(0.0f);
        strings_[i].SetFreq(mtof(40.0f));
        strings_[i].SetDamping(0.5f);

        fb_delayline_[i].Init();

        fb_hpf_[i].Init(sample_rate);

        float hpcutoff = 250.0f;
        fb_hpf_[i].SetFreq(hpcutoff);
    }
}

void Engine::SetStringPitch(const float nn)
{
    const auto freq = mtof(nn);
    strings_[0].SetFreq(freq);
    strings_[1].SetFreq(freq);
}

void Engine::SetFeedbackGain(const float gain_db)
{
    fb_gain_ = dbfs2lin(gain_db);
}

void Engine::SetFeedbackDelay(const float delay_s)
{
    fb_delay_samp_target_ = DSY_CLAMP(delay_s * sample_rate_, 1.0f, static_cast<float>(kMaxFeedbackDelaySamp - 1));
}

void Engine::Process(float *outL, float *outR)
{
    // --- Update audio-rate-smoothed control params ---

    fonepole(fb_delay_samp_, fb_delay_samp_target_, fb_delay_smooth_coef_);

    // --- Process Samples ---

    const float noise_samp = noise_.Process();

    // Get noise + feedback output
    float inL = fb_delayline_[0].Read(fb_delay_samp_) + noise_samp; 
    float inR = fb_delayline_[1].Read(daisysp::fmax(1.0f, fb_delay_samp_ - 16.f)) + noise_samp;

    // Process through KS resonator
    float sampL = strings_[0].Process(inL);
    float sampR = strings_[1].Process(inR);

    // Distort + Clip
    // TODO: Oversample this? Another distortion algo maybe?
    sampL = SoftClip(sampL * 4.0f);
    sampR = SoftClip(sampR * 4.0f);

    // TODO: HP/LP Filter
    sampL = fb_hpf_[0].Process(sampL);
    sampR = fb_hpf_[1].Process(sampR);

    // Tap to output 
    *outL = sampL * 0.5f;
    *outR = sampR * 0.5f;

    // TODO: Allpass

    // Write back into delay with attenuation
    fb_delayline_[0].Write(sampL * fb_gain_);
    fb_delayline_[1].Write(sampR * fb_gain_);
}