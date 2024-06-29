/**
 * @file fsm_usart.h
 * @brief Header for fsm_usart.c file.
 * @author Carlos Navarro Juan 
 * @author Tomas Del pozo merino
 * @date 21/03/2024
 */

#ifndef FSM_USART_H_
#define FSM_USART_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <fsm.h>
#include "port_usart.h"
/* Standard C includes */

/* Other includes */

/* HW dependent includes */

/* Defines and enums ----------------------------------------------------------*/
/* Enums */
enum FSM_USART
{
    WAIT_DATA = 0, /*!< Estado de espera de datos */
    SEND_DATA     /*!< Estado de envío de datos */
};
/* Typedefs --------------------------------------------------------------------*/
/**
 * Estructura para la Máquina de Estados Finita USART
 */
typedef struct
{
    fsm_t f;   /*!< Estado actual de la Máquina de Estados Finita */

    bool data_received; /*!< Indicador de si se han recibido datos */

    char in_data[USART_INPUT_BUFFER_LENGTH]; /*!< Búfer de datos de entrada */

    char out_data[USART_OUTPUT_BUFFER_LENGTH]; /*!< Búfer de datos de salida */

    uint8_t usart_id;  /*!< Identificador USART */
} fsm_usart_t;

/* Function prototypes and explanation -------------------------------------------------*/
/**
 * @brief Crea una nueva instancia  
 * @param usart_id El identificador USART
 * @return Puntero a la instancia creada de la Máquina de Estados Finita
 */
fsm_t *fsm_usart_new(uint32_t usart_id);
/**
 * @brief Inicializa una instancia  T
 * @param p_this Puntero a la instancia de la Máquina de Estados Finita
 * @param usart_id El identificador USART
 */
void fsm_usart_init(fsm_t *p_this, uint32_t usart_id);
/**
 * @brief Comprueba si se han recibido datos
 * @param p_this Puntero a la instancia  
 * @return Verdadero si se han recibido datos, falso en caso contrario
 */
bool fsm_usart_check_data_received(fsm_t *p_this);

/**
 * @brief Obtiene los datos de entrada  
 * @param p_this Puntero a la instancia de la Máquina de Estados Finita
 * @param p_data Puntero al búfer donde se almacenarán los datos de entrada
 */
void fsm_usart_get_in_data(fsm_t *p_this, char *p_data);
/**
 * @brief Establece los datos de salida  
 * @param p_this Puntero a la instancia de la Máquina de Estados Finita
 * @param p_data Puntero a los datos que se establecerán como salida
 */
void fsm_usart_set_out_data(fsm_t *p_this, char *p_data);
/**
 * @brief Restablece el búfer de datos de entrada  
 * @param p_this Puntero a la instancia de la Máquina de Estados Finita
 */
void fsm_usart_reset_input_data(fsm_t *p_this);
/**
 * @brief  Compruebe si el FSM de USART está activo o no.El USART está activo cuando está en el estado o hay datos para leer (indicados como en el campo).SEND_DATAtruedata_received
 * @param p_this Puntero a la instancia de la Máquina de Estados Finita
 * @return Verdadero si hay actividad USART presente, falso en caso contrario
 */
bool fsm_usart_check_activity(fsm_t *p_this);
/**
 * @brief Deshabilita la interrupción de recepción USART 
 * @param p_this Puntero a la instancia de la Máquina de Estados Finita
 */
void fsm_usart_disable_rx_interrupt(fsm_t *p_this);
/**
 * @brief Deshabilita la interrupción de transmisión USART
 * @param p_this Puntero a la instancia de la Máquina de Estados Finita
 */
void fsm_usart_disable_tx_interrupt(fsm_t *p_this);


/**
 * @brief Habilita la interrupción de recepción USART  
 * @param p_this Puntero a la instancia de la Máquina de Estados Finita
 */

void fsm_usart_enable_rx_interrupt(fsm_t *p_this);
/**
 * @brief Habilita la interrupción de transmisión USART  
 * @param p_this Puntero a la instancia de la Máquina de Estados Finita
 */
void fsm_usart_enable_tx_interrupt(fsm_t *p_this);

#endif /* FSM_USART_H_ */
