/*
 * pinos.h - Mapa de pinos e recursos de hardware da Vending Machine.
 *
 * Para trocar a pinagem da placa, altere SOMENTE este arquivo.
 *
 * GPIO27: LED vermelho (produto 1, 1 moeda)
 * GPIO14: LED verde    (produto 2, 2 moedas)
 * GPIO12: LED azul     (produto 3, 3 moedas)
 * GPIO32: potenciometro (ADC1, canal 4)
 * GPIO34: botao moeda  (ativo em nivel baixo)
 * GPIO35: botao compra (ativo em nivel baixo)
 * GPIO2:  buzzer passivo/piezo (PWM pelo LEDC)
 *
 * ALERTA DE HARDWARE: GPIO34 e GPIO35 NAO possuem pull-up/pull-down interno.
 * O filtro de botoes e apenas paliativo: NAO substitui pull-ups externos.
 * Para uma solucao confiavel, adicionar resistor de ~10 kohms de cada
 * sinal de botao para 3,3 V (com botoes ligando o sinal ao GND).
 * NUNCA aplicar 5 V nos GPIOs da ESP32.
 *
 * GPIO12 e GPIO2 sao pinos de strapping; evitar niveis inadequados no boot.
 */
#ifndef PINOS_H
#define PINOS_H

#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_adc/adc_oneshot.h"

/* ---------------- LEDs ---------------- */
#define LED1_PIN        GPIO_NUM_27   /* vermelho - produto 1 */
#define LED2_PIN        GPIO_NUM_14   /* verde    - produto 2 */
#define LED3_PIN        GPIO_NUM_12   /* azul     - produto 3 */

/* LEDs ativos em nivel baixo: 0 = ligado, 1 = desligado. */
#define LED_ON          0
#define LED_OFF         1

/* ---------------- Botoes ---------------- */
#define BTN_COIN_PIN    GPIO_NUM_34   /* moeda  (ativo em nivel baixo) */
#define BTN_BUY_PIN     GPIO_NUM_35   /* compra (ativo em nivel baixo) */

/* ---------------- Potenciometro (ADC) ---------------- */
/* GPIO32 = ADC1 canal 4 */
#define POT_ADC_UNIT    ADC_UNIT_1
#define POT_ADC_CHAN    ADC_CHANNEL_4
#define POT_ADC_BITS    ADC_BITWIDTH_12
#define POT_ADC_ATTEN   ADC_ATTEN_DB_12

/* ---------------- Buzzer (PWM / LEDC) ---------------- */
#define BUZZER_PIN          GPIO_NUM_2
#define BUZZER_MODE         LEDC_LOW_SPEED_MODE
#define BUZZER_TIMER        LEDC_TIMER_0
#define BUZZER_CHANNEL      LEDC_CHANNEL_0
#define BUZZER_RESOLUTION   LEDC_TIMER_10_BIT
#define BUZZER_FREQ_INICIAL 1000U
#define BUZZER_DUTY         512U  /* 50% em 10 bits */

#endif /* PINOS_H */
