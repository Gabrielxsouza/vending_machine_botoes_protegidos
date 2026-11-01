/*
 * botao.h - Detector robusto de acionamento de botao (ativo em nivel baixo).
 *
 * O debounce mede estabilidade do nivel, mas a confirmacao so acontece
 * depois da sequencia: HIGH estavel -> LOW mantido -> HIGH estavel.
 * Isso dificulta acionamentos falsos, mas flutuacao sustentada ainda pode
 * reproduzir a mesma sequencia. Nao ha garantia sem polarizacao eletrica.
 */
#ifndef BOTAO_H
#define BOTAO_H

#include <stdbool.h>
#include <stdint.h>

#include "driver/gpio.h"

typedef enum {
    BTN_AGUARDA_REPOUSO = 0,
    BTN_PRONTO,
    BTN_AGUARDA_PRESSAO_LONGA,
    BTN_AGUARDA_LIBERACAO
} fase_botao_t;

typedef struct {
    gpio_num_t pino;
    int ultima_amostra;          /* -1: ainda nao lido */
    int nivel_estavel;           /* -1: ainda nao estabilizado */
    uint32_t amostra_desde_ms;
    uint32_t estavel_desde_ms;
    uint32_t pressao_desde_ms;
    fase_botao_t fase;
} botao_robusto_t;

/*
 * Inicializa o estado do detector para o pino indicado.
 * O GPIO em si e configurado em hardware_init() (hardware.c).
 */
void botao_init(botao_robusto_t *b, gpio_num_t pino);

/*
 * Retorna true apenas uma vez por sequencia completa:
 * 1) solto HIGH estavel por BTN_ARM_HIGH_MS;
 * 2) pressionado LOW estavel, mantido por BTN_HOLD_LOW_MS;
 * 3) novamente HIGH estavel por BTN_RELEASE_HIGH_MS.
 * Uma tecla solta mas flutuante pode simular os tres passos: nao e garantia.
 */
bool detectar_acionamento(botao_robusto_t *b);

#endif /* BOTAO_H */
