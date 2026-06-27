#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdbool.h>
#include <stdint.h>

#define PROTO_HEADER1    0xAAU
#define PROTO_HEADER2    0x55U
#define PROTO_PACKET_LEN 12U

typedef struct __attribute__((packed)) {
    uint8_t header1;
    uint8_t header2;
    int16_t roll;
    int16_t pitch;
    int16_t yaw;
    uint16_t throttle;
    uint8_t flags;
    uint8_t checksum;
} ControlPacket_t;

uint8_t protocol_calc_checksum(const ControlPacket_t* packet);
bool protocol_validate(const ControlPacket_t* packet);
void protocol_encode(ControlPacket_t* packet);

#endif /* PROTOCOL_H */
