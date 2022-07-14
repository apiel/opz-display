#include "rtmidi/RtMidi.h"

#include <stdio.h>

RtMidiIn *m_in;
RtMidiOut *m_out;

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
    printf("Disconnected\n");
}

void midiProcessMessage(double _deltatime, std::vector<unsigned char> *_message, void *_userData)
{
    // if (_message->at(0) != 0xF8)
    // {
    //     printf("%d\n", _message->at(0));
    // }
    m_out->sendMessage(_message);
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
            printf("Found OP-Z MIDI input port: %s\n", name.c_str());
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
            if (name.rfind("Pico", 0) == 0)
            {
                printf("Found Pico MIDI output port: %s\n", name.c_str());
                try
                {
                    m_out = new RtMidiOut(RtMidi::Api(0), "opz_dump");
                    m_out->openPort(i, name);
                    // m_out->sendMessage(opz_init_msg()); // opzInitMsg

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

int main(int argc, char *args[])
{
    printf(">>>>>>> Midi forward\n");

    midiConnect();

    printf("\nReading MIDI input ... press <enter> to quit.\n");
    char input;
    std::cin.get(input);
    return 0;
}