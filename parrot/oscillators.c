#include "oscillators.h"

/// @brief Structure for holding oscillator state.
typedef struct osc_state
{
    bool on;
    uint16_t freq;
} osc_state_t;

/// @brief Oscillator states
static osc_state_t states[NUM_OSCILLATORS];

/// @brief Set the frequency of an oscillator.
/// @param idx Index of the oscillator.
/// @param freq Desired frequency.
void oscillator_set_freq(size_t idx, uint16_t freq) {
    if(idx < NUM_OSCILLATORS) {
        states[idx].freq = freq;
    }
}

/// @brief Enable or disable an oscillator.
/// @param idx Index of the oscillator.
/// @param on Desired state.
void oscillator_turn_on(size_t idx, bool on) {
    if(idx < NUM_OSCILLATORS) {
        states[idx].on = on;
    }
}

/// @brief Get the frequency of an oscillator.
/// @param idx Index of the oscillator.
/// @return Frequency of the oscillator.
uint16_t oscillator_get_freq(size_t idx) {
    if(idx < NUM_OSCILLATORS) {
        return states[idx].freq;
    }
    return 0;
}

/// @brief Get the state of an oscillator.
/// @param idx Index of the oscillator.
/// @return state of the oscillator.
bool oscillator_is_on(size_t idx) {
    if(idx < NUM_OSCILLATORS) {
        return states[idx].on;
    }
    return false;
}