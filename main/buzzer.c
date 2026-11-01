/*
 * buzzer.c - Buzzer passivo acionado por PWM (LEDC).
 */
#include "buzzer.h"

#include <stdint.h>

#include "config.h"
#include "esp_err.h"
#include "pinos.h"
#include "utils.h"

void beep(int freq, int duration_ms)
{
    ESP_ERROR_CHECK(ledc_set_freq(BUZZER_MODE, BUZZER_TIMER, (uint32_t)freq));
    ESP_ERROR_CHECK(ledc_set_duty(BUZZER_MODE, BUZZER_CHANNEL, BUZZER_DUTY));
    ESP_ERROR_CHECK(ledc_update_duty(BUZZER_MODE, BUZZER_CHANNEL));
    delay_ms((uint32_t)duration_ms);
    ESP_ERROR_CHECK(ledc_set_duty(BUZZER_MODE, BUZZER_CHANNEL, 0));
    ESP_ERROR_CHECK(ledc_update_duty(BUZZER_MODE, BUZZER_CHANNEL));
}

void play_coin_sound(void)
{
    beep(1500, 80);
}

void play_success_sound(void)
{
    beep(1046, 100);
    delay_ms(30);
    beep(1318, 100);
    delay_ms(30);
    beep(1568, 180);
}

void play_error_sound(void)
{
    beep(330, 90);
    delay_ms(80);
    beep(330, 90);
}

void play_change_coin_sound(void)
{
    beep(TROCO_BEEP_FREQ, TROCO_BEEP_MS);
}
