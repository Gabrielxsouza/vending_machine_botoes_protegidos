/*
 * hardware.h - Configuracao de todos os perifericos (GPIO, ADC, LEDC).
 */
#ifndef HARDWARE_H
#define HARDWARE_H

#include "esp_adc/adc_oneshot.h"

/*
 * Configura LEDs (saida, apagados), botoes (entrada sem pull),
 * ADC do potenciometro e PWM do buzzer (em silencio).
 * Deve ser chamada uma unica vez no boot, antes de usar os modulos.
 */
void hardware_init(void);

/* Handle da unidade ADC do potenciometro (valido apos hardware_init). */
adc_oneshot_unit_handle_t hardware_get_adc(void);

#endif /* HARDWARE_H */
