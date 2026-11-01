/*
 * vending.h - Logica da Vending Machine (saldo, selecao e compra).
 */
#ifndef VENDING_H
#define VENDING_H

/* Inicializa todo o hardware: LEDs, botoes, ADC e buzzer. */
void vending_init_hardware(void);

/* Imprime a mensagem de boas-vindas e instrucoes no terminal serial. */
void vending_mostrar_boas_vindas(void);

/* Le o produto inicial, acende o LED e mostra o status. */
void vending_iniciar_selecao(void);

/* Verifica se o potenciometro mudou de produto e atualiza LEDs/status. */
void vending_atualizar_selecao(void);

/* Processa os botoes de moeda e compra. */
void vending_processar_botoes(void);

/* Exibe leituras de diagnostico dos botoes (se BTN_DEBUG = 1). */
void vending_debug_botoes(void);

#endif /* VENDING_H */
