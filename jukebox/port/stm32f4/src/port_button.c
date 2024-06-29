/**
 * @file port_button.c
 * @brief File containing functions related to the HW of the button.
 *
 * This file defines an internal struct which contains the HW information of the button.
 *
 * @author alumno1
 * @author alumno2
 * @date fecha
 */

/* Includes ------------------------------------------------------------------*/
#include "port_button.h"

/* Global variables ------------------------------------------------------------*/
port_button_hw_t 	buttons_arr [] = {
     [BUTTON_0_ID]={.p_port=BUTTON_0_GPIO, .pin=BUTTON_0_PIN, .flag_pressed=false}, /*! se inicializa el array*/
};
/**
 * @brief Inicializa el botón especificado.
 * @param button_id es el identificador del botón a inicializar.
 */
void port_button_init(uint32_t button_id)
{
    GPIO_TypeDef *p_port = buttons_arr[button_id].p_port; /*se castea*/
    uint8_t pin = buttons_arr[button_id].pin; /*!se crea la variable pin con button_id*/
    port_system_gpio_config(p_port,pin,GPIO_MODE_IN,GPIO_PUPDR_NOPULL);  /*!  se llama con los argumentos correctos para configurar el botón como entrada y sin conexión pull up ni pull down*/

    port_system_gpio_config_exti(p_port, pin, TRIGGER_BOTH_EDGE | TRIGGER_ENABLE_INTERR_REQ );/*! se Configura la interrupción EXTI asociada al botón externo para que se active en ambos flancos de la señal 
     y se habilita la solicitud de interrupción*/
     /*!se Habilita la interrupción NVIC correspondiente al pin GPIO del botón externo*/
    port_system_gpio_exti_enable (pin,1,0);	
    
    /* TO-DO alumnos */
}
/**
 * @brief Verifica si el botón especificado está presionado.
 * @param button_id ID del botón a verificar.
 * @return Verdadero si el botón está presionado, falso en caso contrario.
 */
bool port_button_is_pressed	(uint32_t button_id){
    return buttons_arr[button_id].flag_pressed; /*!se obtiene el valor del flag del button_id*/

}
/**
 * @brief Obtiene el valor del contador de ticks en milisegundos.
 * @return Valor actual del contador de ticks.
 */
uint32_t port_button_get_tick(){
      return port_system_get_millis(); /*!se obtiene el valor del contador de milisegundos*/
}