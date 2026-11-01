/*
 * utils.c - Funcoes auxiliares de tempo.
 */
#include "utils.h"

#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void delay_ms(uint32_t ms)
{
    vTaskDelay(pdMS_TO_TICKS(ms));
}

uint32_t millis_now(void)
{
    return (uint32_t)(esp_timer_get_time() / 1000);
}
