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

#define LFO_SHAPE_COUNT 12
const char *lfoShapeName[LFO_SHAPE_COUNT] = {
    "SINE",
    "TRIANGLE",
    "SQUARE",
    "SAW",
    "RANDOM",
    "GYRO",
    "SIN triggered",
    "TRI triggered",
    "SQR triggered",
    "SAW triggered",
    "RND triggered",
    "ONCE triggered",
};

enum
{
    SOUND_PARAM_P1,
    SOUND_PARAM_P2,
    SOUND_PARAM_ATTACK,
    SOUND_PARAM_DECAY,
    SOUND_PARAM_SUBSTAIN,
    SOUND_PARAM_RELEASE,
    SOUND_PARAM_FX1,
    SOUND_PARAM_FX2,
    SOUND_PARAM_CUTOFF,
    SOUND_PARAM_RES,
    SOUND_PARAM_PAN,
    SOUND_PARAM_LEVEL,
    SOUND_PARAM_PORTAMENTO,
    SOUND_PARAM_LFO_DEPTH,
    SOUND_PARAM_LFO_RATE,
    SOUND_PARAM_LFO_DEST,
    SOUND_PARAM_LFO_SHAPE,
    SOUND_PARAM_TBD,
    SOUND_PARAM_COUNT
};

enum
{
    SOUND_PARAM_ARP_SPEED = SOUND_PARAM_LFO_DEPTH,
    SOUND_PARAM_ARP_PATTERN,
    SOUND_PARAM_ARP_STYLE,
    SOUND_PARAM_ARP_RANGE,
};

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

const char *soundParamArpName[5] = {"Speed", "Pattern", "Style", "Range", "TBD2."};

#endif