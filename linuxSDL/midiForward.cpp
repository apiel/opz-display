#include "rtmidi/RtMidi.h"

#include <stdio.h>
#include <unistd.h>

RtMidiIn *m_in_opz2pico;
RtMidiOut *m_out_opz2pico;

#define BOTH 1
#ifdef BOTH
RtMidiIn *m_in_pico2opz;
RtMidiOut *m_out_pico2opz;
#endif

void midiProcessMessage(double _deltatime, std::vector<unsigned char> *_message, void *_userData)
{
    // if (_message->at(0) != 0xF8)
    // {
    //     printf("%d\n", _message->at(0));
    // }
    m_out_opz2pico->sendMessage(_message);
}

#ifdef BOTH
void midiProcessMessage2(double _deltatime, std::vector<unsigned char> *_message, void *_userData)
{
    m_out_pico2opz->sendMessage(_message);
}
#endif

int main(int argc, char *args[])
{
    printf(">>>>>>> Midi forward\n");

    unsigned int nPorts;

    m_in_opz2pico = new RtMidiIn();
    m_out_opz2pico = new RtMidiOut();
    
#ifdef BOTH
    m_in_pico2opz = new RtMidiIn();
    m_out_pico2opz = new RtMidiOut();
#endif

    while (true)
    {
        if (!m_out_opz2pico->isPortOpen())
        {
            nPorts = m_out_opz2pico->getPortCount();
            // printf("\nThere are %d input ports available.\n", nPorts);
            for (unsigned int i = 0; i < nPorts; i++)
            {
                std::string name = m_out_opz2pico->getPortName(i);
                if (name.rfind("Pico", 0) == 0)
                {
                    printf("Found Pico MIDI output port: %s\n", name.c_str());
                    try
                    {
                        m_out_opz2pico = new RtMidiOut(RtMidi::Api(0), "opz_dump");
                        m_out_opz2pico->openPort(i, name);
                        break;
                    }
                    catch (RtMidiError &error)
                    {
                        error.printMessage();
                    }
                }
            }
        }

        if (!m_in_opz2pico->isPortOpen())
        {
            nPorts = m_in_opz2pico->getPortCount();
            // printf("\nThere are %d output ports available.\n", nPorts);
            for (unsigned int i = 0; i < nPorts; i++)
            {
                std::string name = m_in_opz2pico->getPortName(i);
                if (name.rfind("OP-Z", 0) == 0)
                {
                    printf("Found OP-Z MIDI input port: %s\n", name.c_str());
                    try
                    {
                        m_in_opz2pico = new RtMidiIn(RtMidi::Api(0), "opz_dump");
                        m_in_opz2pico->openPort(i, name);
                        m_in_opz2pico->ignoreTypes(false, false, true);
                        m_in_opz2pico->setCallback(midiProcessMessage, NULL);
                        break;
                    }
                    catch (RtMidiError &error)
                    {
                        error.printMessage();
                    }
                }
            }
        }

#ifdef BOTH
        if (!m_out_pico2opz->isPortOpen())
        {
            nPorts = m_out_pico2opz->getPortCount();
            // printf("\nThere are %d input ports available.\n", nPorts);
            for (unsigned int i = 0; i < nPorts; i++)
            {
                std::string name = m_out_pico2opz->getPortName(i);
                if (name.rfind("OP-Z", 0) == 0)
                {
                    printf("Found OP-Z MIDI output port: %s\n", name.c_str());
                    try
                    {
                        m_out_pico2opz = new RtMidiOut(RtMidi::Api(0), "pico_dump");
                        m_out_pico2opz->openPort(i, name);
                        break;
                    }
                    catch (RtMidiError &error)
                    {
                        error.printMessage();
                    }
                }
            }
        }

        if (!m_in_pico2opz->isPortOpen())
        {
            nPorts = m_in_pico2opz->getPortCount();
            // printf("\nThere are %d output ports available.\n", nPorts);
            for (unsigned int i = 0; i < nPorts; i++)
            {
                std::string name = m_in_pico2opz->getPortName(i);
                if (name.rfind("Pico", 0) == 0)
                {
                    printf("Found Pico MIDI input port: %s\n", name.c_str());
                    try
                    {
                        m_in_pico2opz = new RtMidiIn(RtMidi::Api(0), "pico_dump");
                        m_in_pico2opz->openPort(i, name);
                        m_in_pico2opz->ignoreTypes(false, false, true);
                        m_in_pico2opz->setCallback(midiProcessMessage2, NULL);
                        break;
                    }
                    catch (RtMidiError &error)
                    {
                        error.printMessage();
                    }
                }
            }
        }
#endif

        sleep(1);
    }

    return 0;
}