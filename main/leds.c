/*
 * leds.c - Controle dos LEDs indicadores de produto (ativos em nivel baixo).
 */
#include "leds.h"

#include "config.h"
#include "esp_err.h"
#include "pinos.h"
#include "utils.h"

void leds_apagar_todos(void)
{
    ESP_ERROR_CHECK(gpio_set_level(LED1_PIN, LED_OFF));
    ESP_ERROR_CHECK(gpio_set_level(LED2_PIN, LED_OFF));
    ESP_ERROR_CHECK(gpio_set_level(LED3_PIN, LED_OFF));
}

void leds_mostrar_produto(int product)
{
    ESP_ERROR_CHECK(gpio_set_level(LED1_PIN, product == 1 ? LED_ON : LED_OFF));
    ESP_ERROR_CHECK(gpio_set_level(LED2_PIN, product == 2 ? LED_ON : LED_OFF));
    ESP_ERROR_CHECK(gpio_set_level(LED3_PIN, product == 3 ? LED_ON : LED_OFF));
}

void leds_piscar(gpio_num_t pino, int vezes, uint32_t intervalo_ms)
{
    for (int i = 0; i < vezes; i++) {
        ESP_ERROR_CHECK(gpio_set_level(pino, LED_ON));
        delay_ms(intervalo_ms);
        ESP_ERROR_CHECK(gpio_set_level(pino, LED_OFF));
        delay_ms(intervalo_ms);
    }
}
