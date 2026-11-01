/*
 * vending.c - Logica da Vending Machine (saldo, selecao e compra).
 */
#include "vending.h"

#include <stdint.h>
#include <stdio.h>

#include "botao.h"
#include "buzzer.h"
#include "config.h"
#include "hardware.h"
#include "leds.h"
#include "persistencia.h"
#include "pinos.h"
#include "produtos.h"
#include "seletor.h"
#include "utils.h"

static int balance = 0;
static int current_product = -1;
static int previous_product = -1;

static botao_robusto_t botao_moeda;
static botao_robusto_t botao_compra;

static void show_current_status(void)
{
    printf("[i] Selecionado: %s\n", get_product_name(current_product));
    printf("    Preco: %d moeda(s) | Quantidade de Moedas: %d moeda(s)\n\n",
           get_product_price(current_product), balance);
}

/*
 * Altera o saldo e grava imediatamente na flash, para que as moedas
 * nao sejam perdidas em caso de queda de energia.
 */
static void set_balance(int novo_saldo)
{
    balance = novo_saldo;
    persistencia_salvar_saldo(balance);
}

/*
 * Devolve o troco (simulado): 1 piscada do LED + 1 bipe por moeda.
 * O saldo e decrementado e salvo a cada moeda devolvida; se a energia
 * cair no meio da devolucao, so as moedas ainda nao devolvidas
 * continuam como credito no proximo boot.
 */
static void return_change(gpio_num_t led)
{
    if (balance <= 0) {
        return;
    }

    printf("[$] DEVOLVENDO TROCO: %d moeda(s)\n", balance);
    while (balance > 0) {
        set_balance(balance - 1);
        printf("    Moeda devolvida. Restam: %d\n", balance);
        play_change_coin_sound();
        leds_piscar(led, 1, TROCO_PISCA_MS);
    }
    printf("[$] Troco devolvido. Retire suas moedas.\n\n");
}

static void execute_purchase(void)
{
    gpio_num_t selected_led = get_led_pin_for_product(current_product);
    int price = get_product_price(current_product);

    printf("\n****************************************\n");
    printf("[*] Tentativa de compra: %s\n", get_product_name(current_product));
    printf("    Preco: %d moeda(s) | Moedas disponiveis: %d moeda(s)\n", price, balance);

    if (balance >= price) {
        /* Debita e salva ANTES de dispensar o produto. */
        set_balance(balance - price);
        printf("[OK] COMPRA REALIZADA COM SUCESSO!\n");
        printf("     Valor pago: %d moeda(s)\n", price);
        printf("     Troco: %d moeda(s)\n", balance);
        printf("     Dispensando o produto...\n");
        printf("****************************************\n\n");

        leds_apagar_todos();
        leds_piscar(selected_led, 3, 200);

        play_success_sound();
        return_change(selected_led);
        leds_mostrar_produto(current_product);
        show_current_status();
    } else {
        int missing = price - balance;
        printf("[ERRO] SALDO INSUFICIENTE!\n");
        printf("       Preco do item: %d moeda(s)\n", price);
        printf("       Quantidade atual: %d moeda(s)\n", balance);
        printf("       Faltam: %d moeda(s) para completar esta compra.\n", missing);
        printf("       Por favor, insira mais moedas.\n");
        printf("****************************************\n\n");
        play_error_sound();
    }
}

static void handle_coin_button(void)
{
    if (detectar_acionamento(&botao_moeda)) {
        set_balance(balance + 1);
        printf("----------------------------------------\n");
        printf("[+] MOEDA INSERIDA!\n");
        printf("    Quantidade de moedas: %d moeda(s)\n", balance);
        printf("    Item selecionado: %s (Preco: %d)\n",
               get_product_name(current_product), get_product_price(current_product));
        printf("----------------------------------------\n");
        play_coin_sound();
    }
}

static void handle_buy_button(void)
{
    if (detectar_acionamento(&botao_compra)) {
        execute_purchase();
    }
}

void vending_init_hardware(void)
{
    persistencia_init();
    hardware_init();
    botao_init(&botao_moeda, BTN_COIN_PIN);
    botao_init(&botao_compra, BTN_BUY_PIN);
}

void vending_mostrar_boas_vindas(void)
{
    printf("\n========================================================\n");
    printf("       BEM-VINDO A VENDING MACHINE (ESP32)\n");
    printf("========================================================\n");
    printf(" [Tabela de Produtos e Precos]\n");
    printf("   * Produto 1 (LED Vermelho - IO27): 1 moeda  (Refrigerante)\n");
    printf("   * Produto 2 (LED Verde    - IO14): 2 moedas (Snack / Batatas)\n");
    printf("   * Produto 3 (LED Azul     - IO12): 3 moedas (Agua Mineral)\n");
    printf("--------------------------------------------------------\n");
    printf(" Instrucoes:\n");
    printf("   - Gire o potenciometro (IO32) para escolher o produto.\n");
    printf("   - Segure o Botao 1 (IO34) por 0,5 s e SOLTE para inserir moeda.\n");
    printf("   - Segure o Botao 2 (IO35) por 0,5 s e SOLTE para comprar.\n");
    printf("   - Moedas a mais sao devolvidas como troco apos a compra.\n");
    printf("   - O saldo fica salvo mesmo se a energia acabar.\n");
    printf("   - Aguarde o botao estabilizar antes de pressionar novamente.\n");
    printf("========================================================\n\n");
    printf("[AVISO] GPIO34/35 sem pull-up externo: filtro por software NAO garante estabilidade!\n\n");
}

void vending_iniciar_selecao(void)
{
    balance = persistencia_carregar_saldo();
    if (balance > 0) {
        printf("[i] Saldo restaurado da memoria: %d moeda(s)\n", balance);
    }

    current_product = read_selected_product();
    previous_product = current_product;
    leds_mostrar_produto(current_product);
    show_current_status();
}

void vending_atualizar_selecao(void)
{
    current_product = read_selected_product();
    if (current_product != previous_product) {
        previous_product = current_product;
        leds_mostrar_produto(current_product);
        show_current_status();
    }
}

void vending_processar_botoes(void)
{
    handle_coin_button();
    handle_buy_button();
}

void vending_debug_botoes(void)
{
#if BTN_DEBUG
    static uint32_t ultimo_debug_ms = 0;
    if ((uint32_t)(millis_now() - ultimo_debug_ms) >= DEBUG_PERIOD_MS) {
        ultimo_debug_ms = millis_now();
        printf("[DEBUG BOTOES] GPIO34=%d | GPIO35=%d | fase34=%d | fase35=%d\n",
               gpio_get_level(BTN_COIN_PIN), gpio_get_level(BTN_BUY_PIN),
               (int)botao_moeda.fase, (int)botao_compra.fase);
    }
#endif
}
