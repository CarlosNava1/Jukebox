/**
 * @file interr.c
 * @brief Interrupt service routines for the STM32F4 platform.
 * @author Sistemas Digitales II
 * @date 2024-01-01
 */
// Include HW dependencies:
#include "port_system.h"
#include "port_button.h"
#include "port_usart.h"
#define Recepcion 0x20
#define Transmision 0x80
// Include headers of different port elements:

//------------------------------------------------------
// INTERRUPT SERVICE ROUTINES
//------------------------------------------------------
/**
 * @brief Interrupt service routine for the System tick timer (SysTick).
 *
 * @note This ISR is called when the SysTick timer generates an interrupt.
 * The program flow jumps to this ISR and increments the tick counter by one millisecond.
 *
 * > **TO-DO alumnos:**
 * >
 * > ✅ 1. **Increment the System tick counter `msTicks` in 1 count.** To do so, use the function `port_system_get_millis()` and `port_system_get_millis()`.
 *
 * @warning **The variable `msTicks` must be declared volatile!** Just because it is modified by a call of an ISR, in order to avoid [*race conditions*](https://en.wikipedia.org/wiki/Race_condition). **Added to the definition** after *static*.
 *
 */
void SysTick_Handler(void)
{
    uint32_t var = port_system_get_millis();
    var++;
    port_system_set_millis(var);
}
/**
 * @brief Esta función maneja las interrupciones globales Px10-Px15.
 *  esta función identifica la línea/pin que ha elevado la interrupción.
 * 
 * por último,antes de salir, se limpia el registro pendiente de interrupción.
 */
void EXTI15_10_IRQHandler(void)
{
    /* ISR user button */
    if (EXTI->PR & BIT_POS_TO_MASK(buttons_arr[BUTTON_0_ID].pin))/*!Se verifica si la bandera correspondiente al botón de usuario está activada en el registro EXTI_PR.*/
    {
        buttons_arr[BUTTON_0_ID].flag_pressed=false; /*!Si está activada, se establece la bandera de "botón presionado" en falso en el array de botones.*/
    }else{
      buttons_arr[BUTTON_0_ID].flag_pressed=true;/*! Si no está activada, se establece la bandera de "botón presionado" en verdadero en el array de botones.*/
      
    }
    EXTI->PR &= BIT_POS_TO_MASK(buttons_arr[BUTTON_0_ID].pin); /*!Finalmente, se limpia la bandera correspondiente al botón de usuario en el registro EXTI_PR*/
}
/**
 * @brief  Esta función maneja la interrupción global USART3.
 y antes de salir,  se limpia el registro pendiente de interrupción
 */
void USART3_IRQHandler	(	void 		)	{
    if(((USART3->SR &Recepcion)!=0) &&((USART3->CR1&Recepcion)!=0) ){ /*!se está haciendo un and entre el SR y CR1 Y 0x20 d se compruebe que el indicador RXNE está activado y que el bit RXNEIE está habilitado*/
        port_usart_store_data(USART_0_ID);
    }
    if(((USART3->SR &Transmision)!=0) &&((USART3->CR1&Transmision)!=0) ){ /*!se está haciendo un and entre el SR y CR1 Y 0x80 de CR1 compruebe que el indicador TXE está activado y que el bit TXEIE está habilitado*/  
         port_usart_write_data(USART_0_ID);
    }
}

