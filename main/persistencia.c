/*
 * persistencia.c - Armazenamento do saldo na memoria flash (NVS).
 *
 * O NVS da ESP-IDF distribui as gravacoes pela particao (wear leveling)
 * e e tolerante a quedas de energia durante a escrita: ou o valor novo
 * e gravado por completo, ou o valor anterior permanece valido.
 */
#include "persistencia.h"

#include <stdint.h>
#include <stdio.h>

#include "config.h"
#include "esp_err.h"
#include "nvs.h"
#include "nvs_flash.h"

void persistencia_init(void)
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        /* Particao cheia ou de versao antiga: apaga e reinicializa. */
        printf("[AVISO] NVS incompativel/cheia, apagando particao...\n");
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}

int persistencia_carregar_saldo(void)
{
    nvs_handle_t handle;
    int32_t saldo = 0;

    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READONLY, &handle);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        return 0; /* primeiro boot: nada salvo ainda */
    }
    if (err != ESP_OK) {
        printf("[AVISO] Falha ao abrir NVS (%s). Saldo iniciado em 0.\n", esp_err_to_name(err));
        return 0;
    }

    err = nvs_get_i32(handle, NVS_KEY_SALDO, &saldo);
    nvs_close(handle);

    if (err == ESP_ERR_NVS_NOT_FOUND) {
        return 0;
    }
    if (err != ESP_OK || saldo < 0) {
        printf("[AVISO] Saldo salvo invalido (%s). Saldo iniciado em 0.\n", esp_err_to_name(err));
        return 0;
    }
    return (int)saldo;
}

bool persistencia_salvar_saldo(int saldo)
{
    nvs_handle_t handle;

    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err == ESP_OK) {
        err = nvs_set_i32(handle, NVS_KEY_SALDO, (int32_t)saldo);
        if (err == ESP_OK) {
            err = nvs_commit(handle);
        }
        nvs_close(handle);
    }

    if (err != ESP_OK) {
        /* Nao reinicia a placa: o saldo continua valido na RAM. */
        printf("[AVISO] Falha ao salvar saldo na flash (%s)!\n", esp_err_to_name(err));
        return false;
    }
    return true;
}
