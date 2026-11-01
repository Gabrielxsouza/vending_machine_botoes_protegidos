/*
 * Vending Machine ESP32 - ESP-IDF 5.5.x (linguagem C)
 * Versao com protecao de acionamentos dos botoes GPIO34 e GPIO35.
 *
 * Organizacao dos modulos:
 *   pinos.h           - mapa de pinos e recursos de hardware
 *   hardware.c/.h     - configuracao de GPIO, ADC e PWM (LEDC)
 *   config.h          - parametros de software ajustaveis
 *   utils.c/.h        - delay_ms() e millis_now()
 *   leds.c/.h         - LEDs indicadores de produto
 *   buzzer.c/.h       - buzzer PWM e sons de feedback
 *   seletor.c/.h      - potenciometro (ADC) para escolher o produto
 *   botao.c/.h        - detector robusto de acionamento de botao
 *   produtos.c/.h     - catalogo (preco, LED, nome)
 *   persistencia.c/.h - saldo salvo na flash (NVS)
 *   vending.c/.h      - logica da maquina (saldo, selecao, compra, troco)
 *
 * Consulte pinos.h para a pinagem e os alertas de hardware.
 */

#include "config.h"
#include "utils.h"
#include "vending.h"

void app_main(void)
{
    vending_init_hardware();
    delay_ms(100);

    vending_mostrar_boas_vindas();
    vending_iniciar_selecao();

    while (1) {
        vending_atualizar_selecao();
        vending_processar_botoes();
        vending_debug_botoes();
        delay_ms(LOOP_PERIOD_MS);
    }
}
