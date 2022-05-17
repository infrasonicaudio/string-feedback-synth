#pragma once
#ifndef INFS_MIDIPARSER_H
#define INFS_MIDIPARSER_H

// The approach here is based on Pinechettes stmlib
// https://github.com/pichenettes/stmlib/blob/master/midi/midi.h

#include <vector>

namespace infrasonic {

enum class MIDIStatus: unsigned char {
    // TODO: Might need others at some point
    NoteOff       = 0x80,
    NoteOn        = 0x90,
    ControlChange = 0xB0
};

template <class Handler>
class MIDIParser {

    using Message = std::vector<unsigned char>*;

    public:
        MIDIParser() = delete;

        static void Parse(const Message message)
        {
            const unsigned int nBytes = message->size();
            const unsigned char status = message->at(0) & 0xF0; 
            const unsigned char channel = message->at(0) & 0x0F; 

            switch (MIDIStatus(status)) {
                case MIDIStatus::NoteOn:
                if (nBytes == 3)
                    Handler::NoteOn(channel, message->at(1), message->at(2));
                break;

                case MIDIStatus::NoteOff:
                    if (nBytes == 3)
                        Handler::NoteOff(channel, message->at(1), message->at(2));
                    break;

                case MIDIStatus::ControlChange:
                    if (nBytes == 3)
                        Handler::ControlChange(channel, message->at(1), message->at(2));
                    break;

                default:
                    break;
            }
        }
};

}

#endif