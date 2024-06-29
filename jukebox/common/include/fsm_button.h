/**
 * @file fsm_button.h
 * @brief Header for fsm_button.c file.
 * @author alumno1
 * @author alumno2
 * @date fecha
 */

#ifndef FSM_BUTTON_H_
#define FSM_BUTTON_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include <stdbool.h>

/* Other includes */
#include "fsm.h"

/* Defines and enums ----------------------------------------------------------*/
/* Enums */
enum FSM_BUTTON
{
    BUTTON_RELEASED = 0, /**< Botón liberado */
    BUTTON_RELEASED_WAIT, /**< Espera del botón liberado */
    BUTTON_PRESSED,    /**< Botón presionado */
    BUTTON_PRESSED_WAIT /**< Espera del botón presionado */
};

/* Typedefs --------------------------------------------------------------------*/
typedef struct
{
    fsm_t f; /**< Máquina de estados finita */
    uint32_t debounce_time; /**< Tiempo de rebote */

    uint32_t next_timeout; /**< Próximo tiempo de espera */
 
    uint32_t tick_pressed; /**< Tiempo de inicio de la pulsación */

    uint32_t duration; /**< Duración de la pulsación */

    uint32_t button_id; /**< Identificador del botón */

} fsm_button_t;

/* Function prototypes and explanation -------------------------------------------------*/
/**
 * @brief  se crea un nuevo botón FSM.Este FSM implementa un mecanismo anti-rebote. Los rebotes (o pulsaciones de botones muy rápidas) que duran menos de lo que se filtran.
 * @param debounce_time Tiempo de rebote del botón.
 * @param button_id Identificador del botón.
 * @return Puntero a la nueva instancia de la máquina de estados finita.
 */
fsm_t *fsm_button_new(uint32_t debounce_time, uint32_t button_id);
/**
 * @brief  Inicialice un botón FSM. Esta función inicializa los valores predeterminados de la estructura FSM y
 *  llama a la para inicializar el hardware asociado al ID dado.
 * @param p_this Puntero a la instancia de la máquina de estados finita.
 * @param debounce_time Tiempo de rebote del botón.
 * @param button_id Identificador del botón.
 */
void fsm_button_init(fsm_t *p_this, uint32_t debounce_time, uint32_t button_id);
/**
 * @brief Obtiene la duración de la pulsación del botón.
 * @param p_this Puntero a la instancia de la máquina de estados finita.
 * @return Duración de la pulsación del botón.
 */
uint32_t fsm_button_get_duration(fsm_t *p_this);
/**
 * @brief Reinicia la duración de la pulsación del botón.
 * @param p_this Puntero a la instancia de la máquina de estados finita.
 */
void fsm_button_reset_duration(fsm_t *p_this);
/**
 * @brief Compruebe si el botón FSM está activo o no. El botón está inactivo cuando está en el estado .BUTTON_RELEASED.
 * @param p_this Puntero a la instancia de la máquina de estados finita.
 * @return Verdadero si hay actividad en el botón, falso en caso contrario.
 */
bool fsm_button_check_activity(fsm_t *p_this);

#endif
