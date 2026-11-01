/*
 * config.h - Parametros de software ajustaveis da Vending Machine.
 *
 * A pinagem e os recursos de hardware ficam em pinos.h.
 */
#ifndef CONFIG_H
#define CONFIG_H

/* ---------------- Botoes ---------------- */
/* Protecao por software: ajuste de acordo com testes na placa. */
#define BTN_DEBOUNCE_MS       70U   /* troca de nivel precisa estabilizar */
#define BTN_ARM_HIGH_MS     800U   /* deve estar solto antes de armar */
#define BTN_HOLD_LOW_MS     500U   /* manter pressionado por 0,5 s */
#define BTN_RELEASE_HIGH_MS 250U   /* soltar de forma estavel p/ confirmar */
#define BTN_DEBUG             0    /* 1: exibe leituras periodicas */

/* ---------------- Troco (simulado) ---------------- */
/* Cada moeda devolvida = 1 piscada do LED + 1 bipe. */
#define TROCO_PISCA_MS      150U
#define TROCO_BEEP_FREQ     1200
#define TROCO_BEEP_MS         60

/* ---------------- Persistencia (NVS) ---------------- */
#define NVS_NAMESPACE   "vending"
#define NVS_KEY_SALDO   "saldo"

/* ---------------- Loop principal ---------------- */
#define LOOP_PERIOD_MS        10U
#define DEBUG_PERIOD_MS      500U

#endif /* CONFIG_H */
