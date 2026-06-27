#include "drv8833.h"

#include "stm32f4xx_hal.h"

static TIM_HandleTypeDef htim2;
static bool initialized = false;

static uint32_t throttle_to_ccr(float throttle) {
    if (throttle < 0.0f) {
        throttle = 0.0f;
    }
    if (throttle > 1.0f) {
        throttle = 1.0f;
    }

    return (uint32_t)(throttle * (float)htim2.Init.Period);
}

bool drv8833_init(void) {
    __HAL_RCC_TIM2_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef gpio = {0};
    gpio.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;
    gpio.Mode = GPIO_MODE_AF_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(GPIOA, &gpio);

    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 0U;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 4199U;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (HAL_TIM_PWM_Init(&htim2) != HAL_OK) {
        return false;
    }

    TIM_OC_InitTypeDef oc = {0};
    oc.OCMode = TIM_OCMODE_PWM1;
    oc.Pulse = 0U;
    oc.OCPolarity = TIM_OCPOLARITY_HIGH;
    oc.OCFastMode = TIM_OCFAST_DISABLE;

    if (HAL_TIM_PWM_ConfigChannel(&htim2, &oc, TIM_CHANNEL_1) != HAL_OK) {
        return false;
    }
    if (HAL_TIM_PWM_ConfigChannel(&htim2, &oc, TIM_CHANNEL_2) != HAL_OK) {
        return false;
    }
    if (HAL_TIM_PWM_ConfigChannel(&htim2, &oc, TIM_CHANNEL_3) != HAL_OK) {
        return false;
    }
    if (HAL_TIM_PWM_ConfigChannel(&htim2, &oc, TIM_CHANNEL_4) != HAL_OK) {
        return false;
    }

    if (HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1) != HAL_OK) {
        return false;
    }
    if (HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2) != HAL_OK) {
        return false;
    }
    if (HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3) != HAL_OK) {
        return false;
    }
    if (HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4) != HAL_OK) {
        return false;
    }

    initialized = true;
    drv8833_stop_all();
    return true;
}

void drv8833_set_pwm(uint8_t motor, float throttle) {
    if (!initialized) {
        return;
    }

    const uint32_t ccr = throttle_to_ccr(throttle);

    switch (motor) {
        case 0U:
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, ccr);
            break;
        case 1U:
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, ccr);
            break;
        case 2U:
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, ccr);
            break;
        case 3U:
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, ccr);
            break;
        default:
            break;
    }
}

void drv8833_stop_all(void) {
    for (uint8_t i = 0U; i < 4U; i++) {
        drv8833_set_pwm(i, 0.0f);
    }
}
