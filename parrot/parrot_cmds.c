#include "parrot_cmds.h"

#if ENABLE_PARROT_CMDS

#include "poly_control.h"
#include "oscillators.h"
#include <stdlib.h>
#include <stdio.h>

/// @brief Command for turning on a note
/// @param argc Argument count
/// @param argv Argument list
/// @return -1 on failure, 0 on success
int parrot_on_cmd(int argc, char *argv[]) {
    if (argc != 2) {
        return -1;
    }
    uint16_t freq = (uint16_t)atoi(argv[1]);
    polyphonic_control_note_on(freq);

    return 0;
}

/// @brief Command for turning off a note
/// @param argc Argument count
/// @param argv Argument list
/// @return -1 on failure, 0 on success
int parrot_off_cmd(int argc, char *argv[]) {
    if (argc != 2) {
        return -1;
    }
    uint16_t freq = (uint16_t)atoi(argv[1]);
    polyphonic_control_note_off(freq);

    return 0;
}

/// @brief Command for getting oscillator state
/// @param argc Argument count
/// @param argv Argument list
/// @return -1 on failure, 0 on success
int parrot_state_cmd(int argc, char *argv[]) {
    (void) argv;

    if (argc != 1) {
        return -1;
    }

    for(size_t osc_idx = 0; osc_idx < NUM_OSCILLATORS; osc_idx++) {
        printf("%d: is_on: %d, freq: %d\r\n", osc_idx, oscillator_is_on(osc_idx), oscillator_get_freq(osc_idx));
    }

    return 0;
}

#endif // ENABLE_PARROT_CMDS