/**
 * @file port_usart.h
 * @brief Header for port_usart.c file.
 * @author alumno1
 * @author alumno2
 * @date fecha
 */
#ifndef PORT_USART_H_
#define PORT_USART_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx.h"
/* Standard C includes */

/* HW dependent includes */

/* Defines and enums ----------------------------------------------------------*/
/* Defines */
#define USART_0_ID 0
#define USART_0 USART3
#define USART_0_GPIO_TX GPIOB
#define USART_0_GPIO_RX GPIOC
#define USART_0_PIN_TX 10
#define USART_0_PIN_RX 11
#define USART_0_AF_TX 7
#define USART_0_AF_RX 7
#define USART_INPUT_BUFFER_LENGTH 10
#define USART_OUTPUT_BUFFER_LENGTH 100
#define EMPTY_BUFFER_CONSTANT 0x0
#define END_CHAR_CONSTANT 0xA
/* Typedefs --------------------------------------------------------------------*/
;
/**
 * @brief Estructura para el hardware USART
 */
typedef struct
{
    USART_TypeDef *p_usart; /*!< Puntero al periférico USART */
    GPIO_TypeDef *p_port_tx;/*!< Puntero al puerto GPIO del pin de transmisión */
    GPIO_TypeDef *p_port_rx;/*!< Puntero al puerto GPIO del pin de recepción */
    uint8_t pin_tx; /*!< Número de pin de transmisión */
    uint8_t pin_rx; /*!< Número de pin de recepción */
    uint8_t alt_func_tx; /*!< Función alternativa del pin de transmisión */
    uint8_t alt_func_rx; /*!< Función alternativa del pin de rececpción */
    char input_buffer[USART_INPUT_BUFFER_LENGTH]; /*!< Búfer de entrada USART */
    uint8_t i_idx; /*!< Índice de escritura en el búfer de entrada */
    bool read_complete; /*!< Indicador de lectura completa */
    char output_buffer[USART_OUTPUT_BUFFER_LENGTH]; /*!< Búfer de salida USART */
    uint8_t o_idx; /*!< Índice de lectura en el búfer de salida */
    bool write_complete; /**< Indicador de si se ha hecho la escritura completa */

} port_usart_hw_t;

/* Global variables */
extern port_usart_hw_t usart_arr[]; /**< Array de USART */
/* Function prototypes and explanation -------------------------------------------------*/
/**
 * @brief Inicializa el USART especificado.
 * @param usart_id ID del USART a inicializar.
 */
void port_usart_init(uint32_t usart_id);
/**
 * @brief Verifica si la transmisión USART se ha completado.
 * @param usart_id Identificador del USART a verificar.
 * @return Verdadero si la transmisión está completa, falso en caso contrario.
 */
bool port_usart_tx_done(uint32_t usart_id);
/**
 * @brief Verifica si la recepción USART se ha completado.
 * @param usart_id Identificador del USART a verificar.
 * @return Verdadero si la recepción está completa, falso en caso contrario.
 */
bool port_usart_rx_done(uint32_t usart_id);
/**
 * @brief Obtiene datos del búfer de entrada USART.
 * @param usart_id Identificador del USART.
 * @param p_buffer Puntero al búfer donde se almacenarán los datos.
 */
void port_usart_get_from_input_buffer(uint32_t usart_id, char *p_buffer);
/**
 * @brief Obtiene el estado del registro de transmisión USART.
 * @param usart_id Identificador del USART.
 * @return Verdadero si el registro de transmisión está listo, falso en caso contrario.
 */
bool port_usart_get_txr_status(uint32_t usart_id);
/**
 * @brief Copia datos al búfer de salida USART.
 * @param usart_id Identificador del USART.
 * @param p_data Puntero a los datos a copiar.
 * @param length Longitud de los datos a copiar.
 */
void port_usart_copy_to_output_buffer(uint32_t usart_id, char *p_data, uint32_t length);
/**
 * @brief Reinicia el búfer de entrada USART.
 * @param usart_id Identificador del USART.
 */
void port_usart_reset_input_buffer(uint32_t usart_id);
/**
 * @brief Reinicia el búfer de salida USART.
 * @param usart_id Identificador del USART.
 */
void port_usart_reset_output_buffer(uint32_t usart_id);
/**
 * @brief Almacena los datos en el búfer de entrada USART.
 * @param usart_id Identificador del USART.
 */
void port_usart_store_data(uint32_t usart_id);
/**
 * @brief Escribe los datos desde el búfer de salida USART.
 * @param usart_id Identificador del USART.
 */
void port_usart_write_data(uint32_t usart_id);
/**
 * @brief Deshabilita la interrupción de recepción USART.
 * @param usart_id Identificador del USART.
 */
void port_usart_disable_rx_interrupt(uint32_t usart_id);
/**
 * @brief Deshabilita la interrupción de transmisión USART.
 * @param usart_id Identificador del USART.
 */
void port_usart_disable_tx_interrupt(uint32_t usart_id);
/**
 * @brief habilita la interrupción de recepción USART.
 * @param usart_id Identificador del USART.
 */
void port_usart_enable_rx_interrupt(uint32_t usart_id);
/**
 * @brief habilita la interrupción de transmisión USART.
 * @param usart_id Identificadro del USART.
 */
void port_usart_enable_tx_interrupt(uint32_t usart_id);

#endif