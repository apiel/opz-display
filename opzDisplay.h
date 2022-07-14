#ifndef OPZ_DISPLAY_H
#define OPZ_DISPLAY_H

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "midi_interface.h"

#ifndef LOG
#define LOG(...) printf(__VA_ARGS__)
#endif

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
    uint8_t inArray[4] = {0x7E, 0x7F, 0x06, 0x01};
    sendSysEx(inArray, 4);
}

void handleSysEx(uint8_t * array, uint16_t size)
{
    LOG("SysEx (%d): %s\n", size, array);
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