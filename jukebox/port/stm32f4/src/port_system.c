/**
 * @file port_system.c
 * @brief File that defines the functions that are related to the access to the specific HW of the microcontroller.
 * @author Sistemas Digitales II
 * @date 2024-01-01
 */

/* Includes ------------------------------------------------------------------*/
#include "port_system.h"

/* Defines -------------------------------------------------------------------*/
#define HSI_VALUE ((uint32_t)16000000) /*!< Value of the Internal oscillator in Hz */

/* GLOBAL VARIABLES */
static volatile uint32_t msTicks = 0; /*!< Variable to store millisecond ticks. @warning **It must be declared volatile!** Just because it is modified in an ISR. **Add it to the definition** after *static*. */

/* These variables are declared extern in CMSIS (system_stm32f4xx.h) */
uint32_t SystemCoreClock = HSI_VALUE;                                               /*!< Frequency of the System clock */
const uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9}; /*!< Prescaler values for AHB bus */
const uint8_t APBPrescTable[8] = {0, 0, 0, 0, 1, 2, 3, 4};                          /*!< Prescaler values for APB bus */

//------------------------------------------------------
// SYSTEM CONFIGURATION
//------------------------------------------------------
/**
 * @brief  Setup the microcontroller system
 *         Initialize the FPU setting, vector table location and External memory
 *         configuration.
 *
 * @note   This function is called at startup by CMSIS in startup_stm32f446xx.s.
 */
void SystemInit(void)
{
/* FPU settings ------------------------------------------------------------*/
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
  SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2)); /* set CP10 and CP11 Full Access */
#endif

#if defined(DATA_IN_ExtSRAM) || defined(DATA_IN_ExtSDRAM)
  SystemInit_ExtMemCtl();
#endif /* DATA_IN_ExtSRAM || DATA_IN_ExtSDRAM */

  /* Configure the Vector Table location -------------------------------------*/
#if defined(USER_VECT_TAB_ADDRESS)
  SCB->VTOR = VECT_TAB_BASE_ADDRESS | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal SRAM */
#endif                                                 /* USER_VECT_TAB_ADDRESS */
}

/**
 * @brief System Clock Configuration
 *
 * @attention This function should NOT be accesible from the outside to avoid configuration problems.
 * @note This function starts a system timer that generates a SysTick every 1 ms.
 * @retval None
 */
void system_clock_config(void)
{
  /** Configure the main internal regulator output voltage */
  /* Power controller (PWR) */
  /* Control the main internal voltage regulator output voltage to achieve a trade-off between performance and power consumption when the device does not operate at the maximum frequency */
  PWR->CR &= ~PWR_CR_VOS; // Clean and set value
  PWR->CR |= (PWR_CR_VOS & (POWER_REGULATOR_VOLTAGE_SCALE3 << PWR_CR_VOS_Pos));

  /* Initializes the RCC Oscillators. */
  /* Adjusts the Internal High Speed oscillator (HSI) calibration value.*/
  RCC->CR &= ~RCC_CR_HSITRIM; // Clean and set value
  RCC->CR |= (RCC_CR_HSITRIM & (RCC_HSI_CALIBRATION_DEFAULT << RCC_CR_HSITRIM_Pos));

  /* RCC Clock Config */
  /* Initializes the CPU, AHB and APB buses clocks */
  /* To correctly read data from FLASH memory, the number of wait states (LATENCY)
      must be correctly programmed according to the frequency of the CPU clock
      (HCLK) and the supply voltage of the device. */

  /* Increasing the number of wait states because of higher CPU frequency */
  FLASH->ACR = FLASH_ACR_LATENCY_2WS; /* Program the new number of wait states to the LATENCY bits in the FLASH_ACR register */

  /* Change in clock source is performed in 16 clock cycles after writing to CFGR */
  RCC->CFGR &= ~RCC_CFGR_SW; // Clean and set value
  RCC->CFGR |= (RCC_CFGR_SW & (RCC_CFGR_SW_HSI << RCC_CFGR_SW_Pos));

  /* Update the SystemCoreClock global variable */
  SystemCoreClock = HSI_VALUE >> AHBPrescTable[(RCC->CFGR & RCC_CFGR_HPRE) >> RCC_CFGR_HPRE_Pos];

  /* Configure the source of time base considering new system clocks settings */
  SysTick_Config(SystemCoreClock / (1000U / TICK_FREQ_1KHZ)); /* Set Systick to 1 ms */
}

size_t port_system_init()
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  /* Configure Flash prefetch, Instruction cache, Data cache */
  /* Instruction cache enable */
  FLASH->ACR |= FLASH_ACR_ICEN;

  /* Data cache enable */
  FLASH->ACR |= FLASH_ACR_DCEN;

  /* Prefetch cache enable */
  FLASH->ACR |= FLASH_ACR_PRFTEN;

  /* Set Interrupt Group Priority */
  NVIC_SetPriorityGrouping(NVIC_PRIORITY_GROUP_4);

  /* Use systick as time base source and configure 1ms tick (default clock after Reset is HSI) */
  /* Configure the SysTick IRQ priority. It must be the highest (lower number: 0)*/
  NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0U, 0U)); /* Tick interrupt priority */

  /* Init the low level hardware */
  /* Reset and clock control (RCC) */
  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; /* Syscfg clock enabling */

  /* Peripheral clock enable register */
  RCC->APB1ENR |= RCC_APB1ENR_PWREN; /* PWREN: Power interface clock enable */

  /* Configure the system clock */
  system_clock_config();

  return 0;
}

//------------------------------------------------------
// TIMER RELATED FUNCTIONS
//------------------------------------------------------
uint32_t port_system_get_millis()
{
  return msTicks;
}

void port_system_set_millis(uint32_t ms)
{
  msTicks = ms;
}

void port_system_delay_ms(uint32_t ms)
{
  uint32_t tickstart = port_system_get_millis();

  while ((port_system_get_millis() - tickstart) < ms)
  {
  }
}

void port_system_delay_until_ms(uint32_t *p_t, uint32_t ms)
{
  uint32_t until = *p_t + ms;
  uint32_t now = port_system_get_millis();
  if (until > now)
  {
    port_system_delay_ms(until - now);
  }
  *p_t = port_system_get_millis();
}

//------------------------------------------------------
// GPIO RELATED FUNCTIONS
//------------------------------------------------------
void port_system_gpio_config(GPIO_TypeDef *p_port, uint8_t pin, uint8_t mode, uint8_t pupd)
{
  if (p_port == GPIOA)
  {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; /* GPIOA_CLK_ENABLE */
  }
  else if (p_port == GPIOB)
  {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; /* GPIOB_CLK_ENABLE */
  }
  else if (p_port == GPIOC)
  {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; /* GPIOC_CLK_ENABLE */
  }

  /* Clean ( &=~ ) by displacing the base register and set the configuration ( |= ) */
  p_port->MODER &= ~(GPIO_MODER_MODER0 << (pin * 2U));
  p_port->MODER |= (mode << (pin * 2U));

  p_port->PUPDR &= ~(GPIO_PUPDR_PUPD0 << (pin * 2U));
  p_port->PUPDR |= (pupd << (pin * 2U));
}

void port_system_gpio_config_exti(GPIO_TypeDef *p_port, uint8_t pin, uint32_t mode)
{
  uint32_t port_selector = 0;

  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

  /* SYSCFG external interrupt configuration register */
  if (p_port == GPIOA)
  {
    port_selector = 0;
  }
  else if (p_port == GPIOB)
  {
    port_selector = 1;
  }
  else if (p_port == GPIOC)
  {
    port_selector = 2;
  }

  uint32_t base_mask = 0x0FU;
  uint32_t displacement = (pin % 4) * 4;

  SYSCFG->EXTICR[pin / 4] &= ~(base_mask << displacement);
  SYSCFG->EXTICR[pin / 4] |= (port_selector << displacement);

  /* Rising trigger selection register (EXTI_RTSR) */
  EXTI->RTSR &= ~BIT_POS_TO_MASK(pin);
  if (mode & TRIGGER_RISING_EDGE)
  {
    EXTI->RTSR |= BIT_POS_TO_MASK(pin);
  }

  /* Falling trigger selection register (EXTI_FTSR) */
  EXTI->FTSR &= ~BIT_POS_TO_MASK(pin);
  if (mode & TRIGGER_FALLING_EDGE)
  {
    EXTI->FTSR |= BIT_POS_TO_MASK(pin);
  }

  /* Event mask register (EXTI_EMR) */
  EXTI->EMR &= ~BIT_POS_TO_MASK(pin);
  if (mode & TRIGGER_ENABLE_EVENT_REQ)
  {
    EXTI->EMR |= BIT_POS_TO_MASK(pin);
  }

  /* Clear EXTI line configuration */
  EXTI->IMR &= ~BIT_POS_TO_MASK(pin);

  /* Interrupt mask register (EXTI_IMR) */
  if (mode & TRIGGER_ENABLE_INTERR_REQ)
  {
    EXTI->IMR |= BIT_POS_TO_MASK(pin);
  }
}
/**
 * @brief Enable interrupts of a GPIO line (pin)
 *
 * @param pin Pin/line of the GPIO (index from 0 to 15)
 * @param priority Priority level (from highest priority: 0, to lowest priority: 15)
 * @param subpriority Subpriority level (from highest priority: 0, to lowest priority: 15)
 *
 * @retval None
 */
void port_system_gpio_exti_enable(uint8_t pin, uint8_t priority, uint8_t subpriority)
{
  NVIC_SetPriority(GET_PIN_IRQN(pin), NVIC_EncodePriority(NVIC_GetPriorityGrouping(), priority, subpriority));/*!Configura la prioridad de la interrupción EXTI asociada al pin GPIO*/
  NVIC_EnableIRQ(GET_PIN_IRQN(pin));     /*! Habilita la interrupción EXTI en el NVIC para el pin GPIO*/

}
/**
 * @brief Disable interrupts of a GPIO line (pin)
 *
 * @param pin Pin/line of the GPIO (index from 0 to 15)
 *
 * @retval None
 */
void port_system_gpio_exti_disable(uint8_t pin)
{
  NVIC_DisableIRQ(GET_PIN_IRQN(pin)); /*! Deshabilita la interrupción EXTI en el NVIC para el pin GPIO*/
}
/**
 * @brief Configure the alternate function of a GPIO
 *
 * > 1. **Create a 4-bit base mask**. \n
 * > 2. Shift left the mask depending on the value of the given **`pin` modulo 8.** \n
 * > &nbsp;&nbsp;&nbsp;&nbsp;💡 The value of `pin` ranges from 0 to 15. The registers GPIOx_AFRH and GPIOx_AFRL implement 8 groups of 4 bits each. In order to use the value of `pin` as index to select the corresponding group of bits, we can use the remainder of the division by 8. \n
 * > 3. Clean and set the bits **as shown in the tutorial document**. \n
 * > &nbsp;&nbsp;&nbsp;&nbsp;💡 Clean the corresponding bit on element `0` or `1` of the AFR array (*e.g*, `GPIOA->AFR[0]` for GPIOx_AFRL) \n
 * > &nbsp;&nbsp;&nbsp;&nbsp;💡 Set the given value (`alternate`) of the alternate function, using bit shifting, for example. \n
 * \n
 * > 💡 **You can define your own masks for each alternate function (not recommended), or you can use the macro `BASE_MASK_TO_POS(m, p)` to get the mask of a base mask. Example:** \n
 * > &nbsp;&nbsp;&nbsp;&nbsp;A base mask `m` equals `0x03` (`0b 0000 0011` in binary) can be shifted `p` equals `8` positions `BASE_MASK_TO_POS(0x03, 8)` resulting in `0x300` (`0b 0011 0000 0000` in binary). \n
 *
 * @note The AFR register is a 2-element array representing GPIO alternate function high an low registers (GPIOx_AFRH and GPIOx_AFRL) \n
 * AFRLy: Alternate function selection for port x pin y (y = 0..7) \n
 * AFRHy: Alternate function selection for port x pin y (y = 8..15)
 *
 * @param p_port Port of the GPIO (CMSIS struct like)
 * @param pin Pin/line of the GPIO (index from 0 to 15)
 * @param alternate Alternate function number (values from 0 to 15) according to table of the datasheet: "Table 11. Alternate function".
 *
 * @retval None
 */
void port_system_gpio_config_alternate(GPIO_TypeDef *p_port, uint8_t pin, uint8_t alternate)
{
  uint32_t base_mask = 0x0FU;  /*!se hace la Máscara base para la configuración del pin GPIO*/
  uint32_t displacement = (pin % 8) * 4;  /*! se desplaza dentro del registro AFR*/

  p_port->AFR[(uint8_t)(pin / 8)] &= ~(base_mask << displacement); /*! Limpia los bits correspondientes al pin en el registro AFR */
  p_port->AFR[(uint8_t)(pin / 8)] |= (alternate << displacement); /*! Establece los bits correspondientes al pin con la función alternativa deseada en el registro AFR*/
}
/**
 * @brief Lee el estado de un pin GPIO.
 *
 * @param p_port Puntero al puerto GPIO del pin.
 * @param pin Número de pin que se va a leer.
 * @return Estado del pin GPIO. Verdadero si está alto (1), falso si está bajo (0).
 */
bool port_system_gpio_read(GPIO_TypeDef *p_port, uint8_t pin)
{
  bool value = (bool)(GPIOA->IDR & BIT_POS_TO_MASK(pin)); /* Leemos el valor */
  return value;
}
/**
 * @brief Escribe un valor en un pin GPIO.
 *
 *
 * @param p_port Puntero al puerto GPIO del pin.
 * @param pin Número de pin en el puerto GPIO donde se va a escribir el valor.
 * @param value Valor a escribir en el pin GPIO. Verdadero para establecer en alto (1), falso para establecer en bajo (0).
 */
void port_system_gpio_write(GPIO_TypeDef *p_port, int8_t pin, bool value)
{

  if (value == true) /*!se Comprueba si se debe establecer el pin en alto*/

  {
    p_port->BSRR |= BIT_POS_TO_MASK(pin);  /*! se Establece el pin en alto utilizando el registro BSRR*/
  }
  else
  {
    p_port->BSRR |= BIT_POS_TO_MASK(pin) << 16; /*! Establece el pin en bajo utilizando el registro BSRR (se desplaza 16 bits para borrar el bit correspondiente)*/
  }
}
/**
 * @brief Alterna el estado de un pin GPIO.
 *
 * @param p_port Puntero al puerto GPIO del pin.
 * @param pin Número de pin que se va a alterar.
 */
void port_system_gpio_toggle(GPIO_TypeDef *p_port, uint8_t pin){

  port_system_gpio_write(p_port, pin, !port_system_gpio_read(p_port,pin));/*!Se llama a la función write para altenar el estado del pin*/
}