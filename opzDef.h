#ifndef OPZ_DEF_H
#define OPZ_DEF_H

#ifndef LOG
#define LOG(...) printf(__VA_ARGS__)
#endif

// TODO find max data size
#define MAX_DATA_SIZE 2048

#define TRACK_ARP 6

const char *trackName[16] = {
    "KICK", "SNARE", "PERC", "SAMPLE", "BASS", "LEAD", "ARP", "CHORD",
    "FX1", "FX2", "TAPE", "MASTER", "PERFORM", "MODULE", "LIGHT", "MOTION"};

const char *encoderModeName[4] = {"Sound", "Env.", "LFO", "FX/Level"};

// FIXME
        // 8	Selected track number
        // 9	Engine parameter 1
        // 10	Engine parameter 2
        // 11	Attack
        // 12	Decay
        // 13	Sustain
        // 14	Release
        // 15	FX Send level 1
        // 16	FX Send level 2
        // 18	Filter
        // 19	Resonance
        // 20	Pan
        // 21	Level
        // 22	Portamendo
        // 24	LFO Depth / Arp Speed
        // 25	LFO Speed / Arp Pattern
        // 26	LFO Value / Arp Style
        // 27	LFO Shape / Arp Range
const char *soundParamName[16] = {
    "P1",
    "P2",
    "Cutoff",
    "Res",
    "Attack",
    "Decay",
    "Substain",
    "Release",
    "LFO Depth",
    "LFO Rate",
    "LFO Dest",
    "LFO Shape",
    "FX Send 1",
    "FX Send 2",
    "Pan",
    "Level",
};

const char *soundParamArpName[4] = {"Speed", "Pattern", "Style", "Range"};

#endif