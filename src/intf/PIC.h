#pragma once

#include <stdint.h>
#include <stddef.h>


void initialize_pic();
void unmask_irq(uint8_t irq);
void send_eoi(uint8_t irq);

