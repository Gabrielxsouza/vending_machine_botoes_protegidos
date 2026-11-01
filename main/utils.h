/*
 * utils.h - Funcoes auxiliares de tempo.
 */
#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

/* Bloqueia a task atual pelo tempo indicado (em ms). */
void delay_ms(uint32_t ms);

/* Tempo desde o boot, em ms (equivalente ao millis() do Arduino). */
uint32_t millis_now(void);

#endif /* UTILS_H */
