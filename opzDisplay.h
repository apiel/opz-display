#ifndef OPZ_DISPLAY_H
#define OPZ_DISPLAY_H

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "midi_interface.h"
#include "opzMidi.h"
#include "opzDataUtil.h"
#include "opzDef.h"

#define RENDER_SIZE 100
#define LINE_COUNT 3

class OpzDisplay
{
public:
    char line[LINE_COUNT][RENDER_SIZE];

    void set(char *_line1, char *_line2, char *_line3)
    {
        strncpy(line[0], _line1, RENDER_SIZE);
        strncpy(line[1], _line2, RENDER_SIZE);
        strncpy(line[2], _line3, RENDER_SIZE);
    }

    void set(uint8_t linePos, char *_line)
    {
        strncpy(line[linePos % LINE_COUNT], _line, RENDER_SIZE);
    }

    void set(uint8_t linePos, const char *_line)
    {
        strncpy(line[linePos % LINE_COUNT], _line, RENDER_SIZE);
    }
};

OpzDisplay display;

void midiInitSysExOpz()
{
    LOG("Starting MIDI\n");
    sendSysEx((uint8_t *)OPZ_INIT, 4);
}

void sendHeartBeat()
{
    sendSysEx((uint8_t *)OPZ_HEARTBEAT, 9);
}

void handleSysEx(uint8_t *array, uint16_t size)
{
    if (memcmp(OPZ_INIT_RESPONSE, array, sizeof(OPZ_INIT_RESPONSE)) == 0)
    {
        LOG("Got init response\n");
        return;
    }

    // printData(array, size, "Got SysEx");

    if (array[0] != OPZ_VENDOR_ID[0] || array[1] != OPZ_VENDOR_ID[1] || array[2] != OPZ_VENDOR_ID[2])
    {
        LOG("Vendor ID %02X:%02X:%02X is not the expected ID %02X:%02X:%02X\n", array[0], array[1], array[2], OPZ_VENDOR_ID[0], OPZ_VENDOR_ID[1], OPZ_VENDOR_ID[2]);
        return;
    }

    if ((array[3] == 0) || (array[3] > OPZ_MAX_PROTOCOL_VERSION))
    {
        LOG("Unexpected protocol version %02X, was expecting > 0 and <= %02X\n", array[3], OPZ_MAX_PROTOCOL_VERSION);
        return;
    }

    uint8_t parm_id = array[4];

    if (parm_id == 0x01)
    {
        // Universal response https://github.com/hyphz/opzdoc/wiki/MIDI-Protocol#01-universal-response
        // F0 00 20 76 01 01 0C 15 55 2D 0E F7
        //
        // Sent by OP-Z every time a 00 message is received. Significance currently unknown. Bytes 10/11 match bytes 8/9 of 00 message, but with 8th bit cleared if it was set.
        return;
    }

    uint8_t output[MAX_DATA_SIZE];
    uint8_t data[MAX_DATA_SIZE];
    uint16_t dataSize = decode(array + 5, size - 5, data);

    LOG("parm_id %02X (%d)\n", parm_id, parm_id);
    if (parm_id == 0x06)
    {
        // Button states https://github.com/hyphz/opzdoc/wiki/MIDI-Protocol#06-button-states
        // F0 00 20 76 01 06 06 01 40 1F 1F 00 F7
        //
        // 8th byte 1st bit (hi) and 9th byte 7th bit (lo) indicate selected encoder mode:
        // 00 = white, 01 = green, 10 = purple/blue, 11 = orange. 8th byte 2nd bit set = mixer
        // button down. 8th byte 3rd bit set = record button held
        //
        // 10th byte: 5th bit unset = track/step button held, bits 1-4 give number of held track/step.
        // 6th bit set = shift button held. 7th bit set = tempo button held
        //
        // 11th byte: 2nd bit set = screen button held. 3rd bit set = sequencer playing
        //
        // 12th byte: 1st bit set = track select held. 2nd bit set = program button held.
        //
        // Sending these messages can update encoder mode and system modes. Sending messages
        // indicating a button is held can "lock" OPZ into having this button held until another
        // message is sent (pressing and releasing button does not release)

        // printData(data, dataSize, "Button state");
        switch (data[0])
        {
        case 0x00:
            LOG("Encoder mode: white, sound\n");
            break;
        case 0x40:
            LOG("Encoder mode: green, env\n");
            break;
        case 0x80:
            LOG("Encoder mode: purple, lfo or arp\n");
            break;
        case 0xC0:
            LOG("Encoder mode: red, fx and level\n");
            break;

        default:
            break;
        }
    }
    if (parm_id == 0x09)
    {
        // Pattern https://github.com/hyphz/opzdoc/wiki/MIDI-Protocol#09-pattern
        // F0 00 20 76 01 09 00 0A 00 36 00 00 00 78 57 1C 6D 5D 6D 0C 54 57 6E 7D 47 71 7F 3E 41 30 1E 04 42 2E
        // 40 29 61 31 40 6B 58 2F 06 1C 0C 54 59 6B 05 32 5D 5D 09 46 1E 36 06 60 5A 48 4E 62 4A 3B 72 26 45 31
        // 2F 12 79 62 35 09 42 6A 16 3B 12 60 5C 64 5D 12 0A 6C 05 2B 45 16 25 02 4A 79 1E 53 29 35 09 04 10 2F
        // 3F 4C 0B 36 72 0B 64 4D 1C 55 59 15 7C 22 72 54 75 54 46 15 25 10 52 0A 3D 26 35 27 77 5C 27 39 73 73
        // 30 3B 77 1E 76 1E 37 3B 73 3D 12 7A 1D 3D 5D 77 7E 66 1E 39 67 6F 2B 5C 19 1D 5D 3F 27 45 38 64 59 57
        // 1B 48 3D 59 75 79 49 58 1E 7B 0A 1C 53 0A 5A 59 79 43 68 2F 1A 74 5B 5F 68 6A 5C 2B 3B 5F 58 20 5F 09
        // 7A 73 76 5E 3F 5B 56 5A 2E 09 03 7A 07 F7
        //
        // Code indicates that this may be the data for a full pattern, compressed using zlib and sent in packets.

        memcpy(&output, array, 4);
        output[4] = 0x0B;
        uint8_t msg[8] = {0x09, 0x00, 0x00, data[1], data[2], data[3], data[4], 0x00};
        uint16_t len = encode(&msg[0], 8, &output[5]);
        sendSysEx(&output[0], len + 5);
        return;
    }
}

void handleControlChange(uint8_t channel, uint8_t cc, uint8_t value)
{
    // LOG("CC %d: %d\n", cc, value);
    switch (channel)
    {
    case 1:
        display.set(0, "Kick");
        break;

    case 2:
        display.set(0, "Snares");
        break;

    case 3:
        display.set(0, "Perc");
        break;

    case 4:
        display.set(0, "Sample");
        break;

    case 5:
        display.set(0, "Bass");
        break;

    case 6:
        display.set(0, "Lead");
        break;

    case 7:
        display.set(0, "ARP");
        break;

    case 8:
        display.set(0, "Chord");
        break;

    default:
        break;
    }

    switch (cc)
    {
    case 1:
        display.set(1, "P1");
        break;

    case 2:
        display.set(1, "P2");
        break;

    case 3:
        display.set(1, "Cutoff");
        break;

    case 4:
        display.set(1, "Res");
        break;

    case 5:
        display.set(1, "Attack");
        break;

    case 6:
        display.set(1, "Decay");
        break;

    case 7:
        display.set(1, "Substain");
        break;

    case 8:
        display.set(1, "Release");
        break;

    case 9:
        display.set(1, channel == 7 ? "ARP speed" : "LFO Depth");
        break;

    case 10:
        display.set(1, channel == 7 ? "ARP Pattern" : "LFO Rate");
        break;

    case 11:
        display.set(1, channel == 7 ? "ARP Style" : "LFO Dest");
        break;

    case 12:
        display.set(1, channel == 7 ? "ARP Range" : "LFO Shape");
        break;

    case 13:
        display.set(1, "FX Send 1");
        break;

    case 14:
        display.set(1, "FX Send 2");
        break;

    case 15:
        display.set(1, "Pan");
        break;

    case 16:
        display.set(1, "Level");
        break;

    default:
        break;
    }

    if (cc == 12 && channel != 7)
    {
        if (value < 10)
        {
            display.set(2, "SIN");
        }
        else if (value < 21)
        {
            display.set(2, "TRI");
        }
        else if (value < 32)
        {
            display.set(2, "SSQR");
        }
        else if (value < 43)
        {
            display.set(2, "SAW");
        }
        else if (value < 54)
        {
            display.set(2, "RND");
        }
        else if (value < 64)
        {
            display.set(2, "GYRO");
        }
        else if (value < 75)
        {
            display.set(2, "SIN triggered");
        }
        else if (value < 85)
        {
            display.set(2, "TRI triggered");
        }
        else if (value < 96)
        {
            display.set(2, "SSQR triggered");
        }
        else if (value < 107)
        {
            display.set(2, "SAW triggered");
        }
        else if (value < 118)
        {
            display.set(2, "RND triggered");
        }
        else
        {
            display.set(2, "ONCE triggered");
        }
    }
    else if (cc == 11 && channel != 7)
    {
        if (value < 16)
        {
            display.set(2, "P1");
        }
        else if (value < 32)
        {
            display.set(2, "P2");
        }
        else if (value < 48)
        {
            display.set(2, "CUTOFF");
        }
        else if (value < 64)
        {
            display.set(2, "RES");
        }
        else if (value < 80)
        {
            display.set(2, "ATTACK");
        }
        else if (value < 96)
        {
            display.set(2, "PITCH");
        }
        else if (value < 112)
        {
            display.set(2, "PAN");
        }
        else
        {
            display.set(2, "VOLUME");
        }
    }
    else if (cc == 10 && channel != 7)
    {
        if (value < 8)
        {
            display.set(2, "1/64");
        }
        else if (value < 16)
        {
            display.set(2, "1/32");
        }
        else if (value < 24)
        {
            display.set(2, "1/16");
        }
        else if (value < 32)
        {
            display.set(2, "1/8");
        }
        else if (value < 40)
        {
            display.set(2, "1/4");
        }
        else if (value < 48)
        {
            display.set(2, "1/2");
        }
        else if (value < 56)
        {
            display.set(2, "1/1");
        }
        else if (value < 64)
        {
            display.set(2, "2/1");
        }
        else
        {
            snprintf(display.line[2], RENDER_SIZE, "%d Hz", value - 64);
        }
    }
    else if (cc == 9 && channel != 7)
    {
        snprintf(display.line[2], RENDER_SIZE, "%d", value - 64);
    }
    else if (cc == 15)
    {
        if (value == 64)
        {
            display.set(2, "Center");
        }
        else
        {
            snprintf(display.line[2], RENDER_SIZE, "%d %s", abs(value - 64), value < 64 ? "Left" : "Right");
        }
    }
    else if (cc == 9 && channel == 7)
    {
        if (value == 0)
        {
            display.set(2, "Off");
        }
        else
        {
            snprintf(display.line[2], RENDER_SIZE, "%d", (int)(value / 16) + 1);
        }
    }
    else if (cc == 10 && channel == 7)
    {
        if (value < 21)
        {
            display.set(2, "manual");
        }
        else if (value < 43)
        {
            display.set(2, "up");
        }
        else if (value < 64)
        {
            display.set(2, "down");
        }
        else if (value < 85)
        {
            display.set(2, "up/down");
        }
        else if (value < 107)
        {
            display.set(2, "down/up");
        }
        else
        {
            display.set(2, "random");
        }
    }
    else if (cc == 11 && channel == 7)
    {
        snprintf(display.line[2], RENDER_SIZE, "%d", (int)(value / 21) + 1);
    }
    else
    {
        snprintf(display.line[2], RENDER_SIZE, "%d", value);
    }
}
#endif