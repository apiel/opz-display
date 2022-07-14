#ifndef MIDI_INTERFACE_H
#define MIDI_INTERFACE_H

#include <stdint.h>
#include <string.h>

#include "rtmidi/RtMidi.h"

RtMidiIn *m_in;
RtMidiOut *m_out;

void sendSysEx(uint8_t *inArray, uint16_t len)
{
    if (m_out)
    {
        // std::vector<uint8_t> msg(inArray, inArray + len);
        std::vector<uint8_t> msg(len + 2);
        msg[0] = 0xF0;
        memcpy(msg.data() + 1, inArray, len);
        msg[len + 1] = 0xF7;

        // for (int i : msg)
        // {
        //     printf("%d ", i);
        // }
        // printf("\n");

        m_out->sendMessage(&msg);
    }
}

#endif
