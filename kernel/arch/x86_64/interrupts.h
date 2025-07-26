#pragma once

#include <stdint.h>

void interrupts_init(void);

void interrupts_set_callback(uint8_t index, void (*callback)(uint8_t));