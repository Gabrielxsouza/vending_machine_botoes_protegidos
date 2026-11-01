/*
 * seletor.h - Selecao de produto pelo potenciometro (ADC1).
 *
 * O ADC e configurado em hardware_init() (hardware.c).
 */
#ifndef SELETOR_H
#define SELETOR_H

/* Le o potenciometro e retorna o produto selecionado (1..3). */
int read_selected_product(void);

#endif /* SELETOR_H */
