#ifndef _FSM_H_
#define _FSM_H_

#include <stdlib.h>
#include <stdint.h>

/**
 * Some pre-canned status codes
 */
typedef enum {
    OK,
    ERROR,
} fsm_status_e;

/**
 * A generic function pointer for lifecycle callbacks.
 */
typedef fsm_status_e (*fsm_lifecycle_callback)(void *);

/**
 * The typedef for FSM events
 */
typedef uint32_t fsm_state_event;

/**
 * A FSM transition 
 */
typedef struct fsm_transition_t
{
    fsm_state_event event;
    struct fsm_state_t *next_state;
} fsm_transition_t;

/**
 * A FSM state, this holds numerous transitions
 */
typedef struct fsm_state_t
{
    const char *name;
    fsm_transition_t *transitions;
    int num_transitions;
    fsm_lifecycle_callback on_enter;
    fsm_lifecycle_callback on_exit;
} fsm_state_t;

/**
 * The core of the FSM.
 */
typedef struct fsm_t
{
    // the current state of the FSM
    fsm_state_t *current_state;

    // the previous state of the FSM
    fsm_state_t *previous_state;

    // the default state if anything goes wrong.
    fsm_state_t *default_state;
} fsm_t;

/**
 * creates and initializes the FSM with the initial state.
 */
fsm_t *fsm_initialize(fsm_state_t *initial_state, fsm_state_t *default_state);

/**
 * Creates a state with the supplied name and maximum number of transitions
 */
fsm_state_t *fsm_create_state(char *name, uint32_t num_transitions, fsm_lifecycle_callback on_enter, fsm_lifecycle_callback on_exit);

/**
 * Adds a transition to the supplied state with the event
 */
fsm_status_e fsm_add_transition(fsm_state_t *src, fsm_state_t *dst, fsm_state_event event);

/**
 * Processes the input state event.
 */
fsm_status_e fsm_process_event(fsm_t *state_machine, fsm_state_event event);

/**
 * Resets the FSM to the default state.
 */
fsm_status_e fsm_reset(fsm_t *state_machine);

#endif