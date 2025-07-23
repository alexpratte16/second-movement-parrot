#include "poly_control.h"
#include "oscillators.h"
#include "poly_data.h"

static polyphonic_data_t buffer[NUM_OSCILLATORS];

/// @brief Initialize the polyphonic control.
void polyphonic_control_init() {
    polyphonic_init_buf(buffer, NUM_OSCILLATORS);
}

/// @brief Turn a note on.
/// @param freq Frequency to play.
void polyphonic_control_note_on(uint16_t freq) {
    size_t osc_idx = polyphonic_push_freq(buffer, NUM_OSCILLATORS, freq);
    oscillator_set_freq(osc_idx, freq);
    oscillator_turn_on(osc_idx, true);
}

/// @brief Turn a note off.
/// @param freq Frequency to stop.
void polyphonic_control_note_off(uint16_t freq) {
    size_t osc_idx = polyphonic_remove_freq(buffer, NUM_OSCILLATORS, freq);
    if(osc_idx != NUM_OSCILLATORS+1) {
        oscillator_turn_on(osc_idx, false);
    }
}

