/**
 * @file port_button.h
 * @brief Header for port_button.c file.
 * @author alumno1
 * @author alumno2
 * @date fecha
 */

#ifndef PORT_BUTTON_H_
#define PORT_BUTTON_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include <stdbool.h>
#include "port_system.h"
#define BUTTON_0_ID 0
#define BUTTON_0_GPIO GPIOC
#define BUTTON_0_PIN 13
#define BUTTON_0_DEBOUNCE_TIME_MS 150

/* HW dependent includes */

/* Defines and enums ----------------------------------------------------------*/
/* Defines */

/* Typedefs --------------------------------------------------------------------*/
typedef struct
{
    GPIO_TypeDef *p_port; /**< Puntero al puerto GPIO del botón */
    uint8_t pin; /**< Número de pin del botón */
    bool flag_pressed; /**< Indicador de si el botón está presionado */
} port_button_hw_t;


/* Global variables */
extern port_button_hw_t buttons_arr[]; /**< Array de los botones */
/* Function prototypes and explanation -------------------------------------------------*/
/**
 * @brief Inicializa el botón especificado.
 * @param button_id es el identificador del botón a inicializar.
 */
void 	port_button_init (uint32_t button_id);
/**
 * @brief Verifica si el botón especificado está presionado.
 * @param button_id ID del botón a verificar.
 * @return Verdadero si el botón está presionado, falso en caso contrario.
 */
bool 	port_button_is_pressed (uint32_t button_id);
/**
 * @brief Obtiene el valor del contador de ticks en milisegundos.
 * @return Valor actual del contador de ticks.
 */
uint32_t port_button_get_tick ();
#endif
