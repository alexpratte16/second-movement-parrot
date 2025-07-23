#ifndef POLY_DATA_H
#define POLY_DATA_H

#include <stdint.h>
#include <stddef.h>

typedef struct polyphonic_data {
    uint8_t freq;
    size_t age;
} polyphonic_data_t;

extern const polyphonic_data_t EMPTY_NOTE; // = {0xFF, 0xFF};

void polyphonic_init_buf(polyphonic_data_t* buf, size_t len);
uint8_t polyphonic_push_freq(polyphonic_data_t* buf, size_t len, uint8_t freq);
uint8_t polyphonic_remove_freq(polyphonic_data_t* buf, size_t len, uint8_t freq);

#endif // POLY_DATA_H