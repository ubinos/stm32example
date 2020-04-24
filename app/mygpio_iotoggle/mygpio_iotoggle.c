#include <ubinos.h>

#if (INCLUDE__APP__mygpio_iotoggle == 1)

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>

#include "main.h"

static GPIO_InitTypeDef  GPIO_InitStruct;

static void task1func(void *arg);
static void task2func(void *arg);
static void task3func(void *arg);

int appmain(int argc, char *argv[]) {
	int r;

	HAL_Init();
	ubik_settickhookfunc(HAL_IncTick);

#if (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__STM3221GEVAL)

	/* -1- Enable GPIOG, GPIOC and GPIOI Clock (to be able to program the configuration registers) */
	__HAL_RCC_GPIOG_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOI_CLK_ENABLE();

	/* -2- Configure PG.6, PG.8, PI.9 and PC.7 IOs in output push-pull mode to
		 drive external LEDs */
	GPIO_InitStruct.Pin = (GPIO_PIN_6 | GPIO_PIN_8);
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

	HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

#elif (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__NUCLEOF207ZG)

	/* -1- Enable GPIO Clock (to be able to program the configuration registers) */
	LED1_GPIO_CLK_ENABLE();
	LED2_GPIO_CLK_ENABLE();

	/* -2- Configure IO in output push-pull mode to drive external LEDs */
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

	GPIO_InitStruct.Pin = LED1_PIN;
	HAL_GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = LED2_PIN;
	HAL_GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStruct);

#else
	#error "Unsupported UBINOS__BSP__BOARD_MODEL"
#endif

	//
	printf("\n\n\r\n");
	printf("================================================================================\r\n");
	printf("mygpio_iotoggle (build time: %s %s)\r\n", __TIME__, __DATE__);
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

	r = task_create(NULL, task3func, NULL, task_getmiddlepriority(), 0, "task3");
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

static void task3func(void *arg) {
	/* -3- Toggle IO in an infinite loop */
	while (1)
	{
#if (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__STM3221GEVAL)

		HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_6);
		/* Insert delay 100 ms */
		task_sleepms(100);
		HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_8);
		/* Insert delay 100 ms */
		task_sleepms(100);
		HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_9);
		/* Insert delay 100 ms */
		task_sleepms(100);
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_7);
		/* Insert delay 100 ms */
		task_sleepms(100);

#elif (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__NUCLEOF207ZG)

		HAL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_PIN);
		/* Insert delay 100 ms */
		task_sleepms(100);
		HAL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);
		/* Insert delay 100 ms */
		task_sleepms(100);
#else
	#error "Unsupported UBINOS__BSP__BOARD_MODEL"
#endif
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

#endif /* (INCLUDE__APP__mygpio_iotoggle == 1) */

