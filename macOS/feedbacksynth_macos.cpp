/**
 * @file feedback_synth_debug.cpp
 * @author Nick Donaldson
 * @brief Entry point for RtAudio-based debug executable
 * 
 * @copyright Copyright (c) 2022
 */

#include <RtAudio.h>
#include <daisysp.h>
#include <iostream>
#include <cstdlib>

static const unsigned int kSampleRate = 48000;
static const unsigned int kNumChannels = 2;

static daisysp::Oscillator osc;

// Two-channel sawtooth wave generator.
int audio_callback( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
                    double streamTime, RtAudioStreamStatus status, void *userData )
{
  unsigned int i, j;
  float *buffer = (float *) outputBuffer;
  if ( status )
    std::cout << "Stream underflow detected!" << std::endl;
  // Write interleaved audio data.
  for ( i=0; i<nBufferFrames; i++ ) {
    float samp = osc.Process();
    for ( j=0; j<kNumChannels; j++ ) {
      *buffer++ = samp;
    }
  }
  return 0;
}

int main()
{
  RtAudio dac;
  if ( dac.getDeviceCount() < 1 ) {
    std::cout << "\nNo audio devices found!\n";
    exit( 0 );
  }

  osc.Init(static_cast<float>(kSampleRate));
  osc.SetAmp(0.5);
  osc.SetFreq(440.0);
  osc.SetWaveform(daisysp::Oscillator::WAVE_POLYBLEP_TRI);

  RtAudio::StreamParameters parameters;
  parameters.deviceId = dac.getDefaultOutputDevice();
  parameters.nChannels = kNumChannels;
  parameters.firstChannel = 0;
  std::cout << "Outputting to " << dac.getDeviceInfo(parameters.deviceId).name << "\n";

  unsigned int bufferFrames = 128;
  try {
    dac.openStream( &parameters, NULL, RTAUDIO_FLOAT32,
                    kSampleRate, &bufferFrames, &audio_callback, nullptr);
    dac.startStream();
  }
  catch ( RtAudioError& e ) {
    e.printMessage();
    exit( 0 );
  }
  
  char input;
  std::cout << "\nPlaying ... press <enter> to quit.\n";
  std::cin.get( input );
  try {
    // Stop the stream
    dac.stopStream();
  }
  catch (RtAudioError& e) {
    e.printMessage();
  }
  if ( dac.isStreamOpen() ) dac.closeStream();
  return 0;
}