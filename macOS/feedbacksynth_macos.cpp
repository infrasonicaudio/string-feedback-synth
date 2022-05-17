/**
 * @file feedback_synth_debug.cpp
 * @author Nick Donaldson
 * @brief Entry point for RtAudio-based debug executable
 * 
 * @copyright Copyright (c) 2022
 */

#include <iostream>
#include <cstdlib>
#include <memory>
#include <RtAudio.h>
#include <RtMidi.h>
#include "FeedbackSynthEngine.h"
#include "FeedbackSynthControls.h"
#include "FeedbackSynthMIDIHandler.h"
#include "MIDIParser.h"

static const unsigned int kSampleRate = 48000;
static const unsigned int kNumChannels = 2;
static unsigned int kBlockSize = 64;

using namespace infrasonic;

struct AudioCallbackData {
  FeedbackSynthEngine &engine;
  FeedbackSynthControls &controls;
};

void midi_callback(double deltatime, std::vector< unsigned char > *message, void *userData)
{
  MIDIParser<FeedbackSynthMIDIHandler>::Parse(message);
}

// Two-channel sawtooth wave generator.
int audio_callback(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
                   double streamTime, RtAudioStreamStatus status, void *userData)
{
  unsigned int i, j;
  float outL, outR;
  float *buffer = (float *) outputBuffer;
  auto *data = static_cast<AudioCallbackData*>(userData);

  data->controls.Process();

  if ( status )
    std::cout << "Stream underflow detected!" << std::endl;
  // Write interleaved audio data.
  for ( i=0; i<nBufferFrames; i++ ) {
    data->engine.Process(&outL, &outR);
    *buffer++ = outL;
    *buffer++ = outR;
  }
  return 0;
}

void startMIDI(std::unique_ptr<RtMidiIn> &midiin) {

  try {
    midiin = std::make_unique<RtMidiIn>();
  }
  catch ( RtMidiError &error ) {
    error.printMessage();
    exit( EXIT_FAILURE );
  }

  // Check inputs.
  unsigned int nPorts = midiin->getPortCount();
  std::cout << "\nThere are " << nPorts << " MIDI input sources available:\n";
  std::string portName;
  for ( unsigned int i=0; i<nPorts; i++ ) {
    try {
      portName = midiin->getPortName(i);
    }
    catch ( RtMidiError &error ) {
      error.printMessage();
      return;
    }
    std::cout << "  " << i+1 << ") " << portName << '\n';
  }
  
  bool opened = false;
  char input_num[16];
  unsigned int portNum;

  while (!opened) {
    std::cout << "\nPlease choose a MIDI source number for input\n>> ";
    std::cin.get(input_num, 16);
    std::cin.ignore(16, '\n');
    
    portNum = std::stoi(input_num);

    if (portNum > 0 && portNum <= midiin->getPortCount()) {
      try {
        midiin->openPort(portNum-1);
        midiin->setCallback(midi_callback, nullptr);
        midiin->ignoreTypes();
        opened = true;
      }
      catch ( RtMidiError &error ) {
        error.printMessage();
      }
    } else {
      std::cout << "\nERROR: Invalid selection!\n";
    }
  }
}

void setupDAC(RtAudio &dac, AudioCallbackData &callbackData) {

  if ( dac.getDeviceCount() < 1 ) {
    std::cout << "\nNo audio devices found!\n";
    exit( 0 );
  }

  RtAudio::StreamParameters parameters;
  parameters.deviceId = dac.getDefaultOutputDevice();
  parameters.nChannels = kNumChannels;
  parameters.firstChannel = 0;
  std::cout << "\nOutputting to default device: " << dac.getDeviceInfo(parameters.deviceId).name << "\n";

  try {
    dac.openStream( &parameters, NULL, RTAUDIO_FLOAT32,
                    kSampleRate, &kBlockSize, &audio_callback, static_cast<void*>(&callbackData));
  }
  catch ( RtAudioError& e ) {
    e.printMessage();
    exit( 0 );
  }
}

int main()
{

  RtAudio dac;
  std::unique_ptr<RtMidiIn> midiin;

  FeedbackSynthEngine engine;
  FeedbackSynthControls controls;

  AudioCallbackData callbackData{engine, controls};

  // Init DSP classes
  engine.Init(static_cast<float>(kSampleRate));

  // Start MIDI
  startMIDI(midiin);

  // Open DAC for Audio Output
  setupDAC(dac, callbackData);

  // Initialize controls
  // This should be done AFTER opening DAC but before starting stream
  // so that we are using the system-updated block size
  controls.Init(static_cast<float>(kSampleRate) / static_cast<float>(kBlockSize));
  register_feedbacksynth_controls(controls, engine);
  FeedbackSynthMIDIHandler::Init(&controls);

  // Start DAC output stream
  try {
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