/**
 * @file feedback_synth_debug.cpp
 * @author Nick Donaldson
 * @brief Entry point for RtAudio-based debug executable
 * 
 * @copyright Copyright (c) 2022
 */

#include <RtAudio.h>
#include <iostream>
#include <cstdlib>
#include "FeedbackSynthEngine.h"

static const unsigned int kSampleRate = 48000;
static const unsigned int kNumChannels = 2;

static infrasonic::FeedbackSynthEngine engine;

// Two-channel sawtooth wave generator.
int audio_callback( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
                    double streamTime, RtAudioStreamStatus status, void *userData )
{
  unsigned int i, j;
  float outL, outR;
  float *buffer = (float *) outputBuffer;
  if ( status )
    std::cout << "Stream underflow detected!" << std::endl;
  // Write interleaved audio data.
  for ( i=0; i<nBufferFrames; i++ ) {
    engine.Process(&outL, &outR);
    *buffer++ = outL;
    *buffer++ = outR;
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

  engine.Init(static_cast<float>(kSampleRate));

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