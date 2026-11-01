/*
 * botao.c - Detector robusto de acionamento de botao (ativo em nivel baixo).
 */
#include "botao.h"

#include "config.h"
#include "utils.h"

void botao_init(botao_robusto_t *b, gpio_num_t pino)
{
    b->pino = pino;
    b->ultima_amostra = -1;
    b->nivel_estavel = -1;
    b->amostra_desde_ms = 0;
    b->estavel_desde_ms = 0;
    b->pressao_desde_ms = 0;
    b->fase = BTN_AGUARDA_REPOUSO;
}

bool detectar_acionamento(botao_robusto_t *b)
{
    const uint32_t agora = millis_now();
    const int leitura = gpio_get_level(b->pino);

    /* Debounce: so atualiza o estado estavel apos 70 ms sem mudancas. */
    if (leitura != b->ultima_amostra) {
        b->ultima_amostra = leitura;
        b->amostra_desde_ms = agora;
    }
    if (leitura != b->nivel_estavel &&
        (uint32_t)(agora - b->amostra_desde_ms) >= BTN_DEBOUNCE_MS) {
        b->nivel_estavel = leitura;
        b->estavel_desde_ms = agora;
    }

    switch (b->fase) {
        case BTN_AGUARDA_REPOUSO:
            if (b->nivel_estavel == 1 && leitura == 1 &&
                (uint32_t)(agora - b->estavel_desde_ms) >= BTN_ARM_HIGH_MS) {
                b->fase = BTN_PRONTO;
            }
            break;

        case BTN_PRONTO:
            if (b->nivel_estavel == 0) {
                b->pressao_desde_ms = agora;
                b->fase = BTN_AGUARDA_PRESSAO_LONGA;
            }
            break;

        case BTN_AGUARDA_PRESSAO_LONGA:
            if (b->nivel_estavel == 1) {
                /* Pressionamento curto: nao registra nada. */
                b->fase = BTN_AGUARDA_REPOUSO;
            } else if (b->nivel_estavel == 0 &&
                       (uint32_t)(agora - b->pressao_desde_ms) >= BTN_HOLD_LOW_MS) {
                b->fase = BTN_AGUARDA_LIBERACAO;
            }
            break;

        case BTN_AGUARDA_LIBERACAO:
            if (b->nivel_estavel == 1 && leitura == 1 &&
                (uint32_t)(agora - b->estavel_desde_ms) >= BTN_RELEASE_HIGH_MS) {
                /* Aciona somente apos soltar; exige novo repouso para rearmar. */
                b->fase = BTN_AGUARDA_REPOUSO;
                return true;
            }
            break;

        default:
            b->fase = BTN_AGUARDA_REPOUSO;
            break;
    }
    return false;
}
