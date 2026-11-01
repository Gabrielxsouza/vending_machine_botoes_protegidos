/*
 * leds.h - Controle dos LEDs indicadores de produto (ativos em nivel baixo).
 */
#ifndef LEDS_H
#define LEDS_H

#include <stdint.h>

#include "driver/gpio.h"

/* Os GPIOs dos LEDs sao configurados em hardware_init() (hardware.c). */

/* Apaga todos os LEDs. */
void leds_apagar_todos(void);

/* Acende apenas o LED correspondente ao produto (1..3). */
void leds_mostrar_produto(int product);

/* Pisca um LED 'vezes' vezes, com 'intervalo_ms' ligado e desligado. */
void leds_piscar(gpio_num_t pino, int vezes, uint32_t intervalo_ms);

#endif /* LEDS_H */
