/**
 * @file fsm_button.c
 * @brief Button FSM main file.
 * @author alumno1
 * @author alumno2
 * @date fecha
 */

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include "fsm_button.h"
#include "port_button.h"

/* State machine input or transition functions */

/* State machine output or action functions */

/* Other auxiliary functions */

fsm_t *fsm_button_new(uint32_t debounce_time, uint32_t button_id)
{
    fsm_t *p_fsm = malloc(sizeof(fsm_button_t)); /* Do malloc to reserve memory of all other FSM elements, although it is interpreted as fsm_t (the first element of the structure) */
    fsm_button_init(p_fsm, debounce_time, button_id);
    return p_fsm;
}


static bool check_button_pressed(fsm_t *p_this)
{
    fsm_button_t *p_fsm = (fsm_button_t *)(p_this);
    return port_button_is_pressed(p_fsm->button_id);
}
static bool check_button_released(fsm_t *p_this)
{
    fsm_button_t *p_fsm = (fsm_button_t *)(p_this);
    return !port_button_is_pressed(p_fsm->button_id);
}
static bool check_timeout(fsm_t *p_this)
{
    fsm_button_t *p_fsm = (fsm_button_t *)(p_this);
    uint32_t tick = port_button_get_tick();
    if (tick > p_fsm->next_timeout)
    {
        return true;
    }
    else
    {
        return false;
    }
}
static void do_store_tick_pressed(fsm_t *p_this)
{
    fsm_button_t *p_fsm = (fsm_button_t *)(p_this);
    uint32_t tick = port_button_get_tick();
    p_fsm->tick_pressed = tick;
    p_fsm->next_timeout = tick + p_fsm->debounce_time;
}

static void do_set_duration(fsm_t *p_this)
{
fsm_button_t *p_fsm = (fsm_button_t *)(p_this);
    uint32_t tick = port_button_get_tick();
    p_fsm->duration=tick-p_fsm->tick_pressed;
    p_fsm->next_timeout = tick + p_fsm->debounce_time;

}

static fsm_trans_t fsm_trans_button[] = {{BUTTON_RELEASED, check_button_pressed, BUTTON_PRESSED_WAIT, do_store_tick_pressed},
 {BUTTON_PRESSED_WAIT, check_timeout, BUTTON_PRESSED, NULL},
 {BUTTON_PRESSED,check_button_released,BUTTON_RELEASED_WAIT,do_set_duration},
 {BUTTON_RELEASED_WAIT,check_timeout,BUTTON_RELEASED,NULL},
 {-1, NULL, -1, NULL}};
 
uint32_t fsm_button_get_duration(fsm_t *p_this)
{
     fsm_button_t *p_fsm = (fsm_button_t *)(p_this);
     return p_fsm->duration;
}
void fsm_button_reset_duration(fsm_t *p_this)
{
     fsm_button_t *p_fsm = (fsm_button_t *)(p_this);
     p_fsm->duration=0;
}

 void fsm_button_init(fsm_t *p_this, uint32_t debounce_time, uint32_t button_id)
{
    fsm_button_t *p_fsm = (fsm_button_t *)(p_this);
    fsm_init(p_this, fsm_trans_button);

    /* TO-DO alumnos: */
    p_fsm->duration = 0;
    p_fsm->tick_pressed = 0;
    p_fsm->debounce_time = debounce_time;
    p_fsm->button_id= button_id;
    port_button_init(button_id);
}