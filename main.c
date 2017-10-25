#include <stdio.h>
#include "fsm.h"

/**
 * A little helper method to get a SINGLE char.
 */
char get_char()
{
    char input = getchar();
    char junk;
    do
    {
        junk = getchar();
    } while (junk != '\n' && junk != EOF);
    return input;
}

// define a couple of lifecycle callbacks
fsm_status_e on_enter(void *data)
{
    fsm_state_t *state = (fsm_state_t *)data;
    printf("    - Entering state %s\n", state->name);
    return OK;
}

fsm_status_e on_exit(void *data)
{
    fsm_state_t *state = (fsm_state_t *)data;
    printf("    - Exiting state %s\n", state->name);
    return OK;
}

int main(int argc, char *argv[])
{
    // setup states
    fsm_state_t *idle = fsm_create_state("IDLE", 2, &on_enter, &on_exit);
    fsm_state_t *power_off = fsm_create_state("POWER_OFF", 1, &on_enter, &on_exit);
    fsm_state_t *printing = fsm_create_state("PRINTING", 1, &on_enter, &on_exit);

    // idle transitions
    fsm_add_transition(idle, power_off, 'o');
    fsm_add_transition(idle, printing, 'p');

    // power_off transitions
    fsm_add_transition(power_off, idle, 'o');

    // printing transitions
    fsm_add_transition(printing, idle, 'q');

    // create an FSM
    fsm_t *fsm = fsm_initialize(power_off, idle);

    // Let it rip....tater chip
    char input;
    fsm_status_e fsm_status;
    while (1)
    {
        printf("\nEnter event :");
        input = get_char();

        fsm_status = fsm_process_event(fsm, input);

        // check the status of the FSM
        if (fsm_status != OK)
        {
            printf("Something went wrong...error code %d. Resetting the FSM\n", fsm_status);
            fsm_reset(fsm);
        }
    }
    return 0;
}