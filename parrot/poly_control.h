#ifndef POLY_CONTROL_H
#define POLY_CONTROL_H

#include <stdint.h>
#include <stddef.h>

void polyphonic_control_init();

void polyphonic_control_note_on(uint16_t freq);

void polyphonic_control_note_off(uint16_t freq);

#endif // POLY_CONTROL_H