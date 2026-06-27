#include "protocol.h"

#include <stddef.h>

uint8_t protocol_calc_checksum(const ControlPacket_t* packet) {
    if (packet == NULL) {
        return 0U;
    }

    const uint8_t* bytes = (const uint8_t*)packet;
    uint8_t checksum = 0U;

    for (uint8_t i = 2U; i < (PROTO_PACKET_LEN - 1U); i++) {
        checksum ^= bytes[i];
    }

    return checksum;
}

bool protocol_validate(const ControlPacket_t* packet) {
    if (packet == NULL) {
        return false;
    }
    if (packet->header1 != PROTO_HEADER1) {
        return false;
    }
    if (packet->header2 != PROTO_HEADER2) {
        return false;
    }
    if (packet->throttle < 1000U || packet->throttle > 2000U) {
        return false;
    }
    if (packet->checksum != protocol_calc_checksum(packet)) {
        return false;
    }

    return true;
}

void protocol_encode(ControlPacket_t* packet) {
    if (packet == NULL) {
        return;
    }

    packet->header1 = PROTO_HEADER1;
    packet->header2 = PROTO_HEADER2;
    packet->checksum = protocol_calc_checksum(packet);
}
