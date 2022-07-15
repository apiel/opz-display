#ifndef OPZ_DATA_UTIL_H
#define OPZ_DATA_UTIL_H

#include <stdint.h>

// TODO find max data size
#define MAX_DATA_SIZE 2048

uint16_t encode(const unsigned char* inData, unsigned inLength, unsigned char* outSysEx, bool inFlipHeaderBits = true) {
    uint16_t outLength    = 0;     // Num bytes in output array.
    unsigned char count = 0;     // Num 7bytes in a block.
    outSysEx[0]         = 0;

    for (unsigned i = 0; i < inLength; ++i) {
        const unsigned char data = inData[i];
        const unsigned char msb  = data >> 7;
        const unsigned char body = data & 0x7f;

        outSysEx[0] |= (msb << (inFlipHeaderBits ? count : (6 - count)));
        outSysEx[1 + count] = body;

        if (count++ == 6) {
            outSysEx   += 8;
            outLength  += 8;
            outSysEx[0] = 0;
            count       = 0;
        }
    }
    return outLength + count + (count != 0 ? 1 : 0);
}

uint16_t decode(const unsigned char* inData, uint16_t inLength, unsigned char* outData, bool inFlipHeaderBits = true) {
    uint16_t count  = 0;
    unsigned char msbStorage = 0;
    unsigned char byteIndex  = 0;

    for (uint16_t i = 0; i < inLength; ++i) {
        if ((i % 8) == 0) {
            msbStorage = inData[i];
            byteIndex  = 6;
        }
        else {
            const unsigned char body     = inData[i];
            const unsigned char shift    = inFlipHeaderBits ? 6 - byteIndex : byteIndex;
            const unsigned char msb      = uint8_t(((msbStorage >> shift) & 1) << 7);
            byteIndex--;
            outData[count++] = msb | body;
        }
    }
    return count;
}

#endif