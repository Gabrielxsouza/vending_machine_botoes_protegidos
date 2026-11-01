/*
 * persistencia.h - Armazenamento do saldo na memoria flash (NVS).
 *
 * Garante que as moedas inseridas nao sejam perdidas se a ESP32
 * for desligada ou reiniciada.
 */
#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include <stdbool.h>

/* Inicializa a particao NVS. Deve ser chamada uma vez no boot. */
void persistencia_init(void);

/* Retorna o saldo salvo (0 se nunca foi salvo ou em caso de erro). */
int persistencia_carregar_saldo(void);

/* Grava o saldo na flash. Retorna false em caso de falha. */
bool persistencia_salvar_saldo(int saldo);

#endif /* PERSISTENCIA_H */
