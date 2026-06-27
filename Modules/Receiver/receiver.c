#include "receiver.h"

#include <stddef.h>

#include "config.h"
#include "system_time.h"

typedef enum {
    WAIT_HEADER1,
    WAIT_HEADER2,
    READ_PAYLOAD
} ReceiverState_t;

static ReceiverState_t state = WAIT_HEADER1;
static uint8_t buffer[PROTO_PACKET_LEN];
static uint8_t buffer_index = 0U;
static uint32_t last_packet_ms = 0U;
static ControlPacket_t last_valid_packet;
static bool packet_ready = false;
static bool failsafe_active = true;

void receiver_feed_byte(uint8_t byte) {
    switch (state) {
        case WAIT_HEADER1:
            if (byte == PROTO_HEADER1) {
                buffer[0] = byte;
                buffer_index = 1U;
                state = WAIT_HEADER2;
            }
            break;

        case WAIT_HEADER2:
            if (byte == PROTO_HEADER2) {
                buffer[1] = byte;
                buffer_index = 2U;
                state = READ_PAYLOAD;
            } else {
                state = WAIT_HEADER1;
                buffer_index = 0U;
            }
            break;

        case READ_PAYLOAD:
            buffer[buffer_index++] = byte;
            if (buffer_index >= PROTO_PACKET_LEN) {
                const ControlPacket_t* packet = (const ControlPacket_t*)buffer;
                if (protocol_validate(packet)) {
                    last_valid_packet = *packet;
                    last_packet_ms = millis();
                    packet_ready = true;
                    failsafe_active = false;
                }
                state = WAIT_HEADER1;
                buffer_index = 0U;
            }
            break;

        default:
            state = WAIT_HEADER1;
            buffer_index = 0U;
            break;
    }
}

void receiver_update(void) {
    if ((millis() - last_packet_ms) > FAILSAFE_TIMEOUT_MS) {
        failsafe_active = true;
    }
}

bool receiver_get_packet(ControlPacket_t* out) {
    if (!packet_ready || out == NULL) {
        return false;
    }

    *out = last_valid_packet;
    packet_ready = false;
    return true;
}

bool receiver_is_failsafe_active(void) {
    return failsafe_active;
}
