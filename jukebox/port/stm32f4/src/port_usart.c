/**
 * @file port_usart.c
 * @brief Portable functions to interact with the USART FSM library.
 * @author alumno1
 * @author alumno2
 * @date fecha
 */
/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdlib.h>
#include "port_system.h"
#include "port_usart.h"
/* Standard C libraries */

/* HW dependent libraries */

/* Global variables */
port_usart_hw_t usart_arr[] = { /*! se inicializa el array*/
    [USART_0_ID] = {
        .p_usart = USART_0,
        .p_port_tx = USART_0_GPIO_TX,
        .p_port_rx = USART_0_GPIO_RX,
        .pin_tx = USART_0_PIN_TX,
        .pin_rx = USART_0_PIN_RX,
        .alt_func_tx = USART_0_AF_TX,
        .alt_func_rx = USART_0_AF_RX,
        .i_idx = 0,
        .read_complete = false,
        .o_idx = 0,
        .write_complete = false}};

/* Private functions */

/**
 * @brief  Restablezca un búfer a un valor predeterminado.
 *
 *
 * @param buffer Puntero al búfer de memoria que se reiniciará.
 * @param length Longitud del búfer de memoria.
 */
void _reset_buffer(char *buffer, uint32_t length)
{
    memset(buffer, EMPTY_BUFFER_CONSTANT, length); /*!se usa la función memset*/
};
/* Public functions */
/**
 * @brief  Obtiene el mensaje recibido a través del USART y se guarda en el búfer pasado como argumento.
 * Esta función se llama desde la función del FSM para almacenar el mensaje recibido en el búfer del FSM
 *
 * @param usart_id ID del USART del cual obtener los datos del búfer de entrada.
 * @param p_buffer Puntero al búfer de destino donde se copiarán los datos.
 */
void port_usart_get_from_input_buffer(uint32_t usart_id, char *p_buffer)
{
    memcpy(p_buffer, usart_arr[usart_id].input_buffer, USART_INPUT_BUFFER_LENGTH);
};
/**
 * @brief Copia datos al búfer de salida USART. 
 * @param usart_id Identificador del USART.
 * @param p_data Puntero a los datos a copiar.
 * @param length Longitud de los datos a copiar.
 */
bool port_usart_get_txr_status(uint32_t usart_id)
{
    return usart_arr[usart_id].p_usart->SR & USART_SR_TXE; /*!Se acceder al SR del usart_id y se hace un AND con el valor del registro SR (TXE)*/
}
/**
 * @brief Copia datos al búfer de salida USART.
 * @param usart_id Identificador del USART.
 * @param p_data Puntero a los datos a copiar.
 * @param length Longitud de los datos a copiar.
 */
void port_usart_copy_to_output_buffer(uint32_t usart_id, char *p_data, uint32_t length)
{

    memcpy(usart_arr[usart_id].output_buffer, p_data, length);/*! se utiliza la función memcpy pasando como parámetro la longitud, el dato y el buffer de salida de usart_id*/
}
/**
 * @brief Reinicia el búfer de entrada USART.
 * @param usart_id Identificador del USART.
 */
void port_usart_reset_input_buffer(uint32_t usart_id)
{
    _reset_buffer(usart_arr[usart_id].input_buffer, USART_INPUT_BUFFER_LENGTH); /*! se resetea el buffer de entrada */
    usart_arr[usart_id].read_complete = false; /*! deja haberse leído completamente*/
}
/**
 * @brief Reinicia el búfer de salida USART.
 * @param usart_id Identificador del USART.
 */
void port_usart_reset_output_buffer(uint32_t usart_id)
{
    _reset_buffer(usart_arr[usart_id].output_buffer, USART_OUTPUT_BUFFER_LENGTH); /*! se resetea el buffer de salida */
    usart_arr[usart_id].write_complete = false; /*! deja haberse escrito completamente*/
}
/**
 * @brief Verifica si la recepción USART se ha completado.
 * @param usart_id Identificador del USART a verificar.
 * @return Verdadero si la recepción está completa, falso en caso contrario.
 */
bool port_usart_rx_done(uint32_t usart_id)
{
    return usart_arr[usart_id].read_complete; /*!se devuelve si se ha leído completamente*/
}
/**
 * @brief Verifica si la transmisión USART se ha completado.
 * @param usart_id Identificador del USART a verificar.
 * @return Verdadero si la transmisión está completa, falso en caso contrario.
 */
bool port_usart_tx_done(uint32_t usart_id)
{
    return usart_arr[usart_id].write_complete; /*!se devuelve si se ha escrito completamente*/
}
/**
 * @brief Almacena los datos en el búfer de entrada USART.
 * @param usart_id Identificador del USART.
 */
void port_usart_store_data(uint32_t usart_id)
{
    char dato = usart_arr[usart_id].p_usart->DR;  /*! obtiene el valor de dato del registro DR con usart_id*/
    if (dato != END_CHAR_CONSTANT)
    {
        if (usart_arr[usart_id].i_idx >= USART_INPUT_BUFFER_LENGTH)
        {
            usart_arr[usart_id].i_idx = 0; /*! resetea el valor del índice del buffer de entrada*/
            usart_arr[usart_id].input_buffer[usart_arr[usart_id].i_idx] = dato; /*!carga el valor de dato en el buffer de entrada*/
            usart_arr[usart_id].i_idx = usart_arr[usart_id].i_idx + 1;/*!actualiza el valor del ínidce del buffer de entrada*/
        }
        else
        {
            usart_arr[usart_id].input_buffer[usart_arr[usart_id].i_idx] = dato; /*!carga el valor de dato en el buffer de entrada*/
            usart_arr[usart_id].i_idx = usart_arr[usart_id].i_idx + 1; /*!actualiza el valor del ínidce del buffer de entrada*/
        }
    }
    else
    {
        usart_arr[usart_id].read_complete = true; /*! se ha leído completamente*/
         usart_arr[usart_id].i_idx = 0; /*! resetea el valor del índice del buffer de entrada*/
    }
}
/**
 * @brief Escribe los datos desde el búfer de salida USART.
 * @param usart_id Identificador del USART.
 */
void port_usart_write_data(uint32_t usart_id)
{
    uint8_t indice = usart_arr[usart_id].o_idx; /*! obtiene el valor del índice del usart_id*/
    char dato = usart_arr[usart_id].output_buffer[indice]; /*! obtiene el valor de índice del buffer de salida*/
    if ((indice == USART_OUTPUT_BUFFER_LENGTH - 1) || (dato == END_CHAR_CONSTANT))
    {
        usart_arr[usart_id].p_usart->DR = dato;  /*!carga en el registro DR el valor del dato*/
        port_usart_disable_tx_interrupt(usart_id); /*!se llama a esta función para deshabilitar la interrupción de transmisión*/
        usart_arr[usart_id].o_idx = 0; /*!se resetea el valor del índice del buffer de salida*/
        usart_arr[usart_id].write_complete = true; /*!se confirma que se ha leído completamente*/
    }
    else
    {
        if (dato != EMPTY_BUFFER_CONSTANT) /*!se mira si dato coincide o no con el valor del buffer vacío*/
        {
            usart_arr[usart_id].p_usart->DR = dato; /*!carga en el registro DR el valor del dato*/
            usart_arr[usart_id].o_idx = usart_arr[usart_id].o_idx + 1; /*!actualiza el valor del ínidce del buffer de salida*/
        }
    }
};
/**
 * @brief habilita la interrupción de transmisión USART.
 * @param usart_id Identificador del USART.
 */
void port_usart_enable_tx_interrupt(uint32_t usart_id)
{
    USART3->CR1 |= USART_CR1_TXEIE; /*! se hace un or del valor del TXEIE(transmisión) del registro CR1 para habilitarlo (este valor viene dado en un define)*/
};
/**
 * @brief habilita la interrupción de recepción USART.
 * @param usart_id Identificador del USART.
 */
void port_usart_enable_rx_interrupt(uint32_t usart_id)
{
    USART3->CR1 |= USART_CR1_RXNEIE; /*! se hace un or del valor del RXNEIE(recepción) del registro CR1 para habilitarlo (este valor viene dado en un define)*/
};
/**
 * @brief Deshabilita la interrupción de recepción USART.
 * @param usart_id Identificador del USART.
 */
void port_usart_disable_rx_interrupt(uint32_t usart_id)
{
    USART3->CR1 &= ~USART_CR1_RXNEIE; /*! se hace un and negado entre el RXNEIE del registro CR1(este valor viene dado en un define) y el CR1 de la usart */
};
/**
 * @brief Deshabilita la interrupción de transmisión USART.
 * @param usart_id Identificador del USART.
 */
void port_usart_disable_tx_interrupt(uint32_t usart_id)
{
    USART3->CR1 &= ~USART_CR1_TXEIE;/*! se hace un and negado entre el TXEIE del registro CR1(este valor viene dado en un define) y el CR1 de la usart */
};
/**
 * @brief Inicializa el USART especificado.
 * @param usart_id ID del USART a inicializar.
 */
void port_usart_init(uint32_t usart_id)
{
    USART_TypeDef *p_usart = usart_arr[usart_id].p_usart;
    GPIO_TypeDef *p_port_tx = usart_arr[usart_id].p_port_tx;
    GPIO_TypeDef *p_port_rx = usart_arr[usart_id].p_port_rx;
    uint8_t pin_tx = usart_arr[usart_id].pin_tx;
    uint8_t pin_rx = usart_arr[usart_id].pin_rx;
    uint8_t alt_func_tx = usart_arr[usart_id].alt_func_tx;
    uint8_t alt_func_rx = usart_arr[usart_id].alt_func_rx;

    /* TO-DO alumnos: */
    port_system_gpio_config(p_port_tx, pin_tx, GPIO_MODE_ALTERNATE, GPIO_PUPDR_PUP);
    port_system_gpio_config(p_port_rx, pin_rx, GPIO_MODE_ALTERNATE, GPIO_PUPDR_PUP);
    port_system_gpio_config_alternate(p_port_tx, pin_tx, alt_func_tx);
    port_system_gpio_config_alternate(p_port_rx, pin_rx, alt_func_rx);

     
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN;/*!Habilita el reloj del USART*/  
    USART3->CR1 &= ~USART_CR1_UE;/*!Deshabilita el USART*/
    p_usart->BRR = 0x0683; /*!establece la velocidad de transmisión*/
    p_usart->CR1 &= USART_CR1_M; 
      /*!Configura tamaño de marco de datos, bit de parada y paridad*/
    p_usart->CR2 &= USART_CR2_STOP;
    p_usart->CR1 &= USART_CR1_PCE;
    USART3->CR1 &= ~USART_CR1_OVER8;
   
    USART3->CR1 |= USART_CR1_TE | USART_CR1_RE; /*!Habilit transmisión y recepción*/
     /*!Deshabilita interrupciones de transmisión y recepción*/
    USART3->CR1 &= ~USART_CR1_TXEIE;
    USART3->CR1 &= ~USART_CR1_RXNEIE;
    /*! Limpia la bandera RXNE*/
    USART3->CR1 &= ~USART_SR_RXNE;
    // Enable USART interrupts globally
    if (p_usart == USART3)
    {
        NVIC_SetPriority(USART3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 2, 0));
        NVIC_EnableIRQ(USART3_IRQn);
    }

     /*! Habilita el USART*/
    USART3->CR1 |= USART_CR1_UE;
     /*!Reiniciar los buffer de entrada y salida*/
    _reset_buffer(usart_arr[usart_id].input_buffer, USART_INPUT_BUFFER_LENGTH);
    _reset_buffer(usart_arr[usart_id].output_buffer, USART_OUTPUT_BUFFER_LENGTH);
}
