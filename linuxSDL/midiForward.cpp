#include "rtmidi/RtMidi.h"

#include <stdio.h>
#include <unistd.h>

RtMidiIn *m_in;
RtMidiOut *m_out;

void midiProcessMessage(double _deltatime, std::vector<unsigned char> *_message, void *_userData)
{
    // if (_message->at(0) != 0xF8)
    // {
    //     printf("%d\n", _message->at(0));
    // }
    m_out->sendMessage(_message);
}

int main(int argc, char *args[])
{
    printf(">>>>>>> Midi forward\n");

    unsigned int nPorts;

    m_in = new RtMidiIn();
    m_out = new RtMidiOut();

    while (true)
    {
        if (!m_out->isPortOpen())
        {
            nPorts = m_in->getPortCount();
            printf("There are %d input ports available.\n", nPorts);
            for (unsigned int i = 0; i < nPorts; i++)
            {
                std::string name = m_out->getPortName(i);
                if (name.rfind("Pico", 0) == 0)
                {
                    printf("Found Pico MIDI output port: %s\n", name.c_str());
                    try
                    {
                        m_out = new RtMidiOut(RtMidi::Api(0), "opz_dump");
                        m_out->openPort(i, name);
                        break;
                    }
                    catch (RtMidiError &error)
                    {
                        error.printMessage();
                    }
                }
            }
        }

        if (!m_in->isPortOpen())
        {
            nPorts = m_in->getPortCount();
            printf("There are %d output ports available.\n", nPorts);
            for (unsigned int i = 0; i < nPorts; i++)
            {
                std::string name = m_in->getPortName(i);
                if (name.rfind("OP-Z", 0) == 0)
                {
                    printf("Found OP-Z MIDI input port: %s\n", name.c_str());
                    try
                    {
                        m_in = new RtMidiIn(RtMidi::Api(0), "opz_dump");
                        m_in->openPort(i, name);
                        m_in->ignoreTypes(false, false, true);
                        m_in->setCallback(midiProcessMessage, NULL);
                        break;
                    }
                    catch (RtMidiError &error)
                    {
                        error.printMessage();
                    }
                }
            }
        }

        sleep(1);
    }

    return 0;
}