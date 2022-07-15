#ifndef OPZ_DEF_H
#define OPZ_DEF_H

#ifndef LOG
#define LOG(...) printf(__VA_ARGS__)
#endif

// TODO find max data size
#define MAX_DATA_SIZE 2048

const char *trackName[16] = {
    "KICK", "SNARE", "PERC", "SAMPLE", "BASS", "LEAD", "ARP", "CHORD", 
    "FX1", "FX2", "TAPE", "MASTER", "PERFORM", "MODULE", "LIGHT", "MOTION"};

#endif