#include "poly_data.h"
#include <stdbool.h>
#include <assert.h>

const polyphonic_data_t EMPTY_NOTE = {0xFF, 0xFF};

/// @brief Return if 2 polyphonic datas are equal.
/// @param a First data to compare.
/// @param b Second data to compare.
/// @return True if data is equal.
static bool is_eq(polyphonic_data_t a, polyphonic_data_t b) {
    return a.freq == b.freq && a.age == b.age;
}

/// @brief Intialize a polyphonic buffer.
/// @param buf Buffer to Initialize.
/// @param len Length of the buffer.
void polyphonic_init_buf(polyphonic_data_t* buf, size_t len) {
    assert(len > 0 && len < SIZE_MAX);

    for(size_t buffer_idx = 0; buffer_idx < len; buffer_idx++) {
        buf[buffer_idx] = EMPTY_NOTE;
    }
}


/// @brief Push a frequency to the polyphonic buffer. If the buffer is filled kick off the oldest frequency.
/// @param buf Buffer to push the note to.
/// @param len Length of the buffer.
/// @param freq Frequency to push to the buffer.
/// @return The index that the frequency was inserted.
uint8_t polyphonic_push_freq(polyphonic_data_t* buf, size_t len, uint8_t freq) {
    assert(len > 0 && len < SIZE_MAX);

    polyphonic_data_t note = {freq, 0};

    size_t oldest_idx = 0;
    size_t empty_idx = len+1;
    size_t buffer_idx = 0;
    bool found_duplicate = false;

    for(; buffer_idx < len; buffer_idx++) {
        // find the maximum age.
        if(buf[buffer_idx].age > buf[oldest_idx].age) {
            oldest_idx = buffer_idx;
        }

        // Increment age of all elements except EMPTY_NOTES.
        if(is_eq(buf[buffer_idx], EMPTY_NOTE)) {
            empty_idx = buffer_idx;
        } else {
            buf[buffer_idx].age++;
        }

        if(buf[buffer_idx].freq == freq) {
            found_duplicate = true;
            break;
        }
    }

    // If we found a duplicate freq, return ages to original ages.
    if(found_duplicate) {
        for(size_t i = 0; i < buffer_idx; i++) {
            // Decrement age of all elements except EMPTY_NOTES.
            if(is_eq(buf[buffer_idx], EMPTY_NOTE)) {
                empty_idx = buffer_idx;
            } else {
                buf[buffer_idx].age--;
            }
        }
        return buffer_idx;
    }

    // Fill an empty slot if found
    if(empty_idx != len+1) {
        buf[empty_idx] = note;
        return empty_idx;
    }

    buf[oldest_idx] = note;
    return oldest_idx;
}

/// @brief Remove a frequency from the polyphonic buffer.
/// @param buf Buffer to remove the frequency from.
/// @param len Length of the buffer.
/// @param freq Frequency to remove.
/// @return Index of the removed frequency or len+1 if the requested frequency was not found.
uint8_t polyphonic_remove_freq(polyphonic_data_t* buf, size_t len, uint8_t freq) {
    assert(len > 0 && len < SIZE_MAX);

    for(size_t buffer_idx = 0; buffer_idx < len; buffer_idx++) {
        if(buf[buffer_idx].freq == freq) {
            buf[buffer_idx] = EMPTY_NOTE;
            return buffer_idx;
        }
    }

    return len+1;
}