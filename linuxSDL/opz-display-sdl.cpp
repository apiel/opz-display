#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <stdio.h>

// #include <time.h>
// #include <sys/time.h>

#include "color.h"
#include "font.h"

#include "midi_interface.h" // Must be before opzDisplay.h

// #include "../opzDevice.h"
#include "../opzDisplay.h"
#include "../opzDef.h"

#define SCREEN_WIDTH 340
#define SCREEN_HEIGHT 160

double m_last_heartbeat;
double m_last_time;
double m_last_step;

bool m_connected;

timespec time_start;

// Standard MIDI events
enum midi_id
{
    NOTE_OFF = 0x80,
    NOTE_ON = 0x90,
    KEY_PRESSURE = 0xA0,
    CONTROLLER_CHANGE = 0xB0,
    PROGRAM_CHANGE = 0xC0,
    CHANNEL_PRESSURE = 0xD0,
    PITCH_BEND = 0xE0,
    SYSEX_HEAD = 0xF0,
    SONG_POSITION = 0xF2,
    SONG_SELECT = 0xF3,
    TUNE_REQUEST = 0xF6,
    SYSEX_END = 0xF7,
    TIMING_TICK = 0xF8,
    START_SONG = 0xFA,
    CONTINUE_SONG = 0xFB,
    STOP_SONG = 0xFC,
    ACTIVE_SENSING = 0xFE,
    SYSTEM_RESET = 0xFF
};

void midiProcessMessage(double _deltatime, std::vector<unsigned char> *_message, void *_userData)
{
    uint8_t command = _message->at(0);
    uint8_t data[MAX_DATA_SIZE];

    if (command == SYSEX_HEAD)
    {
        // SDL_Log("SYSEX_HEAD");
        memcpy(data, _message->data() + 1, _message->size() - 2);
        handleSysEx(&data[0], _message->size() - 2);
        return;
    }
    if (command != TIMING_TICK)
    {
        // SDL_Log("midiProcessMessage: 0x%02X (%d)", command, command);
    }

    uint8_t status = 0;
    uint8_t channel = 0;
    if ((command & 0xf0) != 0xf0)
    {
        channel = command & 0x0f;
        channel += 1;
        status = command & 0xf0;
    }
    else
    {
        channel = 0;
        status = command;
    }

    switch (status)
    {
    case CONTROLLER_CHANGE:
        // SDL_Log("CONTROLLER_CHANGE");
        handleControlChange(channel, _message->at(1), _message->at(2));
        break;

    default:
        break;
    }
}

void midiDisconnect()
{
    if (m_in)
    {
        m_in->cancelCallback();
        m_in->closePort();
        delete m_in;
        m_in = NULL;
    }

    if (m_out)
    {
        m_out->closePort();
        delete m_out;
        m_out = NULL;
    }

    m_connected = false;
}

bool midiConnect()
{

    m_in = new RtMidiIn();
    unsigned int nPorts = m_in->getPortCount();
    bool in_connected = false;
    for (unsigned int i = 0; i < nPorts; i++)
    {
        std::string name = m_in->getPortName(i);
        if (name.rfind("OP-Z", 0) == 0)
        {
            SDL_Log("Found OP-Z MIDI input port: %s", name.c_str());
            try
            {
                m_in = new RtMidiIn(RtMidi::Api(0), "opz_dump");
                m_in->openPort(i, name);
                m_in->ignoreTypes(false, false, true);
                m_in->setCallback(midiProcessMessage, NULL);
                in_connected = true;
                break;
            }
            catch (RtMidiError &error)
            {
                error.printMessage();
            }
        }
    }

    if (in_connected)
    {
        m_out = new RtMidiOut();
        nPorts = m_out->getPortCount();
        for (unsigned int i = 0; i < nPorts; i++)
        {
            std::string name = m_out->getPortName(i);
            if (name.rfind("OP-Z", 0) == 0)
            {
                SDL_Log("Found OP-Z MIDI output port: %s", name.c_str());
                try
                {
                    clock_gettime(CLOCK_MONOTONIC, &time_start);

                    m_out = new RtMidiOut(RtMidi::Api(0), "opz_dump");
                    m_out->openPort(i, name);
                    midiInitSysExOpz();

                    m_connected = true;
                    return true;
                }
                catch (RtMidiError &error)
                {
                    error.printMessage();
                }
            }
        }
    }

    midiDisconnect();
    return false;
}

bool handleEvent()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_KEYUP:
        case SDL_KEYDOWN:
            if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
            {
                return false;
            }
            break;

        case SDL_QUIT:
            SDL_Log("Event QUIT");
            return false;
        }
    }
    return true;
}

void render(SDL_Surface *screenSurface)
{
    SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, UI_COLOR_BG));
    // SDL_Log("\n%s\n%s\n%s\n", display.line[0], display.line[1], display.line[2]);
    draw_string(screenSurface, display.line[0], 2, 6, 3);
    draw_string(screenSurface, display.line[1], 2, 50, 4);

    if (display.drawPan != -1)
    {
        snprintf(display.line[2], RENDER_SIZE, "L %d", abs(display.drawPan - 100));
        draw_string(screenSurface, display.line[2], 2, 100, 2);
        snprintf(display.line[2], RENDER_SIZE, "R %d", display.drawPan);
        draw_string(screenSurface, display.line[2], 130, 100, 2);

        SDL_Rect r = {5, SCREEN_HEIGHT - 20, SCREEN_WIDTH - 10, 4};
        SDL_FillRect(screenSurface, &r, SDL_MapRGB(screenSurface->format, UI_COLOR_BAR));

        SDL_Rect r1 = {SCREEN_WIDTH * 0.5f - 2, SCREEN_HEIGHT - 20, 4, 4};
        SDL_FillRect(screenSurface, &r1, SDL_MapRGB(screenSurface->format, UI_COLOR_BG));

        SDL_Rect r2 = {5 + ((float)(SCREEN_WIDTH - 10 - 4) / 100.0f * display.drawPan), SCREEN_HEIGHT - 40, 4, 20};
        SDL_FillRect(screenSurface, &r2, SDL_MapRGB(screenSurface->format, UI_COLOR_BAR));
    }
    else
    {
        draw_string(screenSurface, display.line[2], 2, 100, 4);
    }

    if (display.drawValue)
    {
        SDL_Rect r = {5, SCREEN_HEIGHT - 20, (float)display.drawValue / 100.0f * (SCREEN_WIDTH - 10), 4};
        SDL_FillRect(screenSurface, &r, SDL_MapRGB(screenSurface->format, UI_COLOR_BAR));
    }
}

int main(int argc, char *args[])
{
    SDL_Log(">>>>>>> Start OPZ display\n");

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "Could not initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "OPZ display",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);

    if (window == NULL)
    {
        fprintf(stderr, "Could not create window: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Surface *screenSurface = SDL_GetWindowSurface(window);

    midiConnect();

    display.set((char *)"Connect OPZ", (char *)"OP-Z", (char *)"Display");
    unsigned int lastTime = 0;
    while (handleEvent())
    {
        if (SDL_GetTicks() - lastTime > 1000)
        {
            lastTime = SDL_GetTicks();
            sendHeartBeat();
        }
        render(screenSurface);
        SDL_UpdateWindowSurface(window);
        SDL_Delay(100); // ?
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}