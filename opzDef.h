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

#define SOUND_PARAM_COUNT 18

const char *soundParamName[SOUND_PARAM_COUNT] = {
    "P1",
    "P2",
    "Attack",
    "Decay",
    "Substain",
    "Release",
    "FX Send 1",
    "FX Send 2",
    "Cutoff",
    "Res",
    "Pan",
    "Level",
    "Portamento",
    "LFO Depth",
    "LFO Rate",
    "LFO Dest",
    "LFO Shape",
    "TBD.",
};

const char *soundParamArpName[4] = {"Speed", "Pattern", "Style", "Range"};

#endif