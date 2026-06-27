#include "system_time.h"

#include "stm32f4xx_hal.h"

static TIM_HandleTypeDef htim5;

void system_time_init(void) {
    __HAL_RCC_TIM5_CLK_ENABLE();

    uint32_t timer_clock = HAL_RCC_GetPCLK1Freq();
    if ((RCC->CFGR & RCC_CFGR_PPRE1) != RCC_HCLK_DIV1) {
        timer_clock *= 2U;
    }

    htim5.Instance = TIM5;
    htim5.Init.Prescaler = (timer_clock / 1000000U) - 1U;
    htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim5.Init.Period = 0xFFFFFFFFU;
    htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

    HAL_TIM_Base_Init(&htim5);
    HAL_TIM_Base_Start(&htim5);
}

uint32_t micros(void) {
    return __HAL_TIM_GET_COUNTER(&htim5);
}

uint32_t millis(void) {
    return micros() / 1000U;
}
