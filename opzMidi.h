#ifndef OPZ_MIDI_H
#define OPZ_MIDI_H

#include <stdint.h>

const uint8_t OPZ_INIT[4] = {0x7E, 0x7F, 0x06, 0x01};
const uint8_t OPZ_INIT_RESPONSE[4] = {0x7E, 0x7F, 0x06, 0x02};
const uint8_t OPZ_VENDOR_ID[3] = {0x00, 0x20, 0x76};
const uint8_t OPZ_MAX_PROTOCOL_VERSION = 0x01;
const uint8_t OPZ_HEARTBEAT[9] = {
    // OPZ_VENDOR_ID[0], OPZ_VENDOR_ID[1], OPZ_VENDOR_ID[2], OPZ_MAX_PROTOCOL_VERSION, 0x00, 0x03, 0x2D, 0x0E, 0x05 // prior to version 1.2.5
    OPZ_VENDOR_ID[0], OPZ_VENDOR_ID[1], OPZ_VENDOR_ID[2], OPZ_MAX_PROTOCOL_VERSION, 0x00, 0x01, 0x4E, 0x2E, 0x06 // version 1.2.5
    // OPZ_VENDOR_ID[0], OPZ_VENDOR_ID[1], OPZ_VENDOR_ID[2], OPZ_MAX_PROTOCOL_VERSION, 0x00, 0x01, 0x7F, 0x75, 0x06 // new version
    // OPZ_VENDOR_ID[0], OPZ_VENDOR_ID[1], OPZ_VENDOR_ID[2], OPZ_MAX_PROTOCOL_VERSION, 0x00, 0x00, 0x2C, 0x54, 0x06 //
    // OPZ_VENDOR_ID[0], OPZ_VENDOR_ID[1], OPZ_VENDOR_ID[2], OPZ_MAX_PROTOCOL_VERSION, 0x00, 0x02, 0x27, 0x3C, 0x06
};

#endif