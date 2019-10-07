#include <ubinos.h>

#if (INCLUDE__APP__mygpio_exti == 1)

#include "main.h"

static void EXTILine0_Config(void);
static void EXTILine15_10_Config(void);

int appmain(int argc, char *argv[]) {
	HAL_Init();

	BSP_LED_Init(LED1);
	BSP_LED_Init(LED2);

	EXTILine0_Config();

	EXTILine15_10_Config();

	while (1) {
	}

	return 0;
}

/**
 * @brief  Configures EXTI Line0 (connected to PA0 pin) in interrupt mode
 * @param  None
 * @retval None
 */
static void EXTILine0_Config(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIOA clock */
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/* Configure PA0 pin as input floating */
	GPIO_InitStructure.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Pin = GPIO_PIN_0;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Enable and set EXTI Line0 Interrupt to the lowest priority */
	HAL_NVIC_SetPriority(EXTI0_IRQn, 2, 0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

/**
 * @brief  Configures EXTI Line15 (connected to PG15 pin) in interrupt mode
 * @param  None
 * @retval None
 */
static void EXTILine15_10_Config(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIOG clock */
	__HAL_RCC_GPIOG_CLK_ENABLE();

	/* Configure PG15 pin as input floating */
	GPIO_InitStructure.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Pin = GPIO_PIN_15;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);

	/* Enable and set EXTI15_10 Interrupt to the lowest priority */
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 3, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

/**
 * @brief EXTI line detection callbacks
 * @param GPIO_Pin: Specifies the pins connected EXTI line
 * @retval None
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == KEY_BUTTON_PIN) {
		/* Toggle LED2 */
		BSP_LED_Toggle(LED2);
	}

	if (GPIO_Pin == WAKEUP_BUTTON_PIN) {
		/* Toggle LED1 */
		BSP_LED_Toggle(LED1);
	}
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
 * @brief  This function handles SysTick Handler.
 * @param  None
 * @retval None
 */
void SysTick_Handler(void) {
}

/**
 * @brief  This function handles External line 0 interrupt request.
 * @param  None
 * @retval None
 */
void EXTI0_IRQHandler(void) {
	HAL_GPIO_EXTI_IRQHandler(WAKEUP_BUTTON_PIN);
}

/**
 * @brief  This function handles External lines 15 to 10 interrupt request.
 * @param  None
 * @retval None
 */
void EXTI15_10_IRQHandler(void) {
	HAL_GPIO_EXTI_IRQHandler(KEY_BUTTON_PIN);
}

#endif /* (INCLUDE__APP__mygpio_exti == 1) */

