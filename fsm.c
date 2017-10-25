#include "fsm.h"
#include <stdio.h>

// a few private member functions
fsm_transition_t *_fsm_lookup_transition(fsm_state_t *state, fsm_state_event event);
fsm_status_e _fsm_update_current_state(fsm_t *state_machine, fsm_state_t *new_state);

/**
 * See fsm.h
 */
fsm_t *fsm_initialize(fsm_state_t *initial_state, fsm_state_t *default_state)
{
    fsm_t *fsm = malloc(sizeof(fsm_t));
    fsm->current_state = initial_state;
    fsm->default_state = default_state;
    return fsm;
}

/*
 * See fsm.h
 */
fsm_state_t *fsm_create_state(char *name, uint32_t num_transitions, fsm_lifecycle_callback on_enter, fsm_lifecycle_callback on_exit)
{
    fsm_transition_t *transitions = malloc(sizeof(fsm_transition_t) * num_transitions);
    fsm_state_t *state = malloc(sizeof(fsm_state_t));

    state->name = name;
    state->transitions = transitions;
    state->num_transitions = 0;
    state->on_enter = on_enter;
    state->on_exit = on_exit;

    return state;
}

/**
 * See fsm.h
 */
fsm_status_e fsm_add_transition(fsm_state_t *src, fsm_state_t *dst, fsm_state_event event)
{
    fsm_transition_t t = {
        .event = event,
        .next_state = dst};
    src->transitions[src->num_transitions++] = t;
    return OK;
}

/**
 * See fsm.h
 */
fsm_status_e fsm_process_event(fsm_t *state_machine, fsm_state_event event)
{
    fsm_state_t *state = state_machine->current_state;
    fsm_transition_t *transition = _fsm_lookup_transition(state, event);

    if (transition)
    {
        _fsm_update_current_state(state_machine, transition->next_state);
        return OK;
    }
    return ERROR;
}

/**
 * See fsm.h
 */
fsm_status_e fsm_reset(fsm_t *state_machine)
{
    return _fsm_update_current_state(state_machine, state_machine->default_state);
}

/**
 * Returns the transition from the state with the corresponding event
 */
fsm_transition_t *_fsm_lookup_transition(fsm_state_t *state, fsm_state_event event)
{
    // perhaps a dictionary would be better :)
    fsm_transition_t *trans;
    int i;
    for (i = 0; i < state->num_transitions; ++i)
    {
        trans = &state->transitions[i];
        if (trans->event == event)
        {
            return trans;
        }
    }
    return NULL;
}

/**
 * Update the current state of the of state machine
 */
fsm_status_e _fsm_update_current_state(fsm_t *state_machine, fsm_state_t *new_state)
{
    if (!new_state)
    {
        return ERROR;
    }
    if (new_state == state_machine->current_state)
    {
        return OK;
    }
    // leaving this state
    if (state_machine->current_state)
    {
        state_machine->current_state->on_exit(state_machine->current_state);
    }
    state_machine->previous_state = state_machine->current_state;
    state_machine->current_state = new_state;

    // entering the new state
    state_machine->current_state->on_enter(state_machine->current_state);
    return OK;
}