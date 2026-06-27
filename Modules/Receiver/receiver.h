#ifndef RECEIVER_H
#define RECEIVER_H

#include <stdbool.h>
#include <stdint.h>

#include "protocol.h"

void receiver_feed_byte(uint8_t byte);
void receiver_update(void);
bool receiver_get_packet(ControlPacket_t* out);
bool receiver_is_failsafe_active(void);

#endif /* RECEIVER_H */
