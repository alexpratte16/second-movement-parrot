#ifndef OSCILLATORS_H
#define OSCILLATORS_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define NUM_OSCILLATORS 4

void oscillator_set_freq(size_t idx, uint16_t freq);

void oscillator_turn_on(size_t idx, bool on);

uint16_t oscillator_get_freq(size_t idx);

bool oscillator_is_on(size_t idx);

#endif // OSCILLATORS_H