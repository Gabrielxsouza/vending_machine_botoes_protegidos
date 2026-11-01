/*
 * seletor.c - Selecao de produto pelo potenciometro (ADC1).
 */
#include "seletor.h"

#include "esp_err.h"
#include "hardware.h"
#include "pinos.h"

/* 0..1364 = produto 1; 1365..2729 = 2; 2730..4095 = 3. */
int read_selected_product(void)
{
    int raw_value = 0;
    ESP_ERROR_CHECK(adc_oneshot_read(hardware_get_adc(), POT_ADC_CHAN, &raw_value));

    if (raw_value < 1365) return 1;
    if (raw_value < 2730) return 2;
    return 3;
}
