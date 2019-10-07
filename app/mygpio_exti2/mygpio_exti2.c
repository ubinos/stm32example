#include <ubinos.h>

#if (INCLUDE__APP__mygpio_exti2 == 1)

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>

#include "main.h"

static void EXTILine0_Config(void);
static void EXTILine15_10_Config(void);
static void task1func(void *arg);
static void task2func(void *arg);

int appmain(int argc, char *argv[]) {
	int r;

	HAL_Init();

	BSP_LED_Init(LED1);
	BSP_LED_Init(LED2);

	EXTILine0_Config();

	EXTILine15_10_Config();

	//
	printf("\n\n\r\n");
	printf("================================================================================\r\n");
	printf("mygpio_exti2 (build time: %s %s)\r\n", __TIME__, __DATE__);
	printf("================================================================================\r\n");
	printf("\r\n");
#if (UBINOS__UBICLIB__USE_MALLOC_RETARGETING == 1)
	r = heap_printheapinfo(NULL);
	if (0 == r) {
		printf("\r\n");
		printf("================================================================================\r\n");
		printf("\r\n");
	}
#endif /* (UBINOS__UBICLIB__USE_MALLOC_RETARGETING == 1) */

	srand(time(NULL));

	r = task_create(NULL, task1func, NULL, task_getmiddlepriority(), 0, "task1");
	if (0 != r) {
		logme("fail at task_create\r\n");
	}

	r = task_create(NULL, task2func, NULL, task_getmiddlepriority(), 0, "task2");
	if (0 != r) {
		logme("fail at task_create\r\n");
	}

	ubik_comp_start();

	return 0;
}

static void task1func(void *arg) {
	unsigned int delayms;

	task_sleepms(1000);

	for (unsigned int i = 0;; i++) {
		delayms = (rand() % 10 + 1) * 200;
		printf("1: hello world ! (%u) (delay = %4d ms)\r\n", i, delayms);
		task_sleepms(delayms);
	}
}

static void task2func(void *arg) {
	unsigned int delayms;

	task_sleepms(1000);

	for (unsigned int i = 0;; i++) {
		delayms = (rand() % 10 + 1) * 200;
		printf("2: hello world ! (%u) (delay = %4d ms)\r\n", i, delayms);
		task_sleepms(delayms);
	}
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

#endif /* (INCLUDE__APP__mygpio_exti2 == 1) */

