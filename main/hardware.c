/*
 * hardware.c - Configuracao de todos os perifericos (GPIO, ADC, LEDC).
 *
 * Os demais modulos (leds, botao, seletor, buzzer) apenas USAM os
 * perifericos; toda a configuracao fica concentrada aqui.
 */
#include "hardware.h"

#include "esp_err.h"
#include "pinos.h"

static adc_oneshot_unit_handle_t adc_handle;

static void configurar_leds(void)
{
    gpio_config_t cfg = {
        .pin_bit_mask = (1ULL << LED1_PIN) | (1ULL << LED2_PIN) | (1ULL << LED3_PIN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    ESP_ERROR_CHECK(gpio_config(&cfg));
    ESP_ERROR_CHECK(gpio_set_level(LED1_PIN, LED_OFF));
    ESP_ERROR_CHECK(gpio_set_level(LED2_PIN, LED_OFF));
    ESP_ERROR_CHECK(gpio_set_level(LED3_PIN, LED_OFF));
}

static void configurar_botoes(void)
{
    /* Os GPIOs 34/35 nao possuem polarizacao interna: nao habilitar pull-up ficticio. */
    gpio_config_t cfg = {
        .pin_bit_mask = (1ULL << BTN_COIN_PIN) | (1ULL << BTN_BUY_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    ESP_ERROR_CHECK(gpio_config(&cfg));
}

static void configurar_adc(void)
{
    adc_oneshot_unit_init_cfg_t unit_cfg = {
        .unit_id = POT_ADC_UNIT,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&unit_cfg, &adc_handle));

    adc_oneshot_chan_cfg_t chan_cfg = {
        .bitwidth = POT_ADC_BITS,
        .atten = POT_ADC_ATTEN,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_handle, POT_ADC_CHAN, &chan_cfg));
}

static void configurar_buzzer(void)
{
    /* PWM para buzzer passivo, reproduzindo tone() do Arduino. */
    ledc_timer_config_t timer_cfg = {
        .speed_mode = BUZZER_MODE,
        .duty_resolution = BUZZER_RESOLUTION,
        .timer_num = BUZZER_TIMER,
        .freq_hz = BUZZER_FREQ_INICIAL,
        .clk_cfg = LEDC_AUTO_CLK,
    };
    ESP_ERROR_CHECK(ledc_timer_config(&timer_cfg));

    ledc_channel_config_t channel_cfg = {
        .gpio_num = BUZZER_PIN,
        .speed_mode = BUZZER_MODE,
        .channel = BUZZER_CHANNEL,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = BUZZER_TIMER,
        .duty = 0,
        .hpoint = 0,
    };
    ESP_ERROR_CHECK(ledc_channel_config(&channel_cfg));
}

void hardware_init(void)
{
    configurar_leds();
    configurar_botoes();
    configurar_adc();
    configurar_buzzer();
}

adc_oneshot_unit_handle_t hardware_get_adc(void)
{
    return adc_handle;
}
