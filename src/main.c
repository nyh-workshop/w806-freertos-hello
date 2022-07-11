#include <stdio.h>
#include "wm_hal.h"
#include "FreeRTOS.h"
#include "task.h"

void task1_handle(void *p);
void task2_handle(void *p);

static void GPIO_Init(void);

const HeapRegion_t xHeapRegions[] = 
{ 
    { ( uint8_t * ) 0x20020000UL, 0x5000 },
    { NULL, 0 } /*终止数组 */ 
};

TaskHandle_t htask1 = NULL;
TaskHandle_t htask2 = NULL;

void Error_Handler(void);

int main(void)
{
    SystemClock_Config(CPU_CLK_160M);
    HAL_Init();
    GPIO_Init();

    printf("enter main\r\n");
    
    // Note from the SDK readme:
    // Delay function failed in FreeRTOS in /include/arch/xt804/csi_config.h!
    // Please comment out "#define CONFIG_KERNEL_NONE 1"
    vPortDefineHeapRegions( xHeapRegions );
	xTaskCreate(task1_handle, "task1", 512, NULL, 35, &htask1);
	xTaskCreate(task2_handle, "task2", 512, NULL, 32, &htask2);
	vTaskStartScheduler();

    return 0;
}

void task1_handle(void *p)
{
	while(1)
	{
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
		vTaskDelay(1000);
	}
}

void task2_handle(void *p)
{
	while(1)
	{
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
		vTaskDelay(500);
	}
}

void Error_Handler(void)
{
    while (1)
    {
    }
}

static void GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    __HAL_RCC_GPIO_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    //HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2, GPIO_PIN_SET);
}

void assert_failed(uint8_t *file, uint32_t line)
{
    printf("Wrong parameters value: file %s on line %d\r\n", file, line);
}