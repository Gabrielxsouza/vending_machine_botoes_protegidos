/*
 * produtos.c - Catalogo de produtos (preco, LED e nome).
 */
#include "produtos.h"

#include "pinos.h"

int get_product_price(int product)
{
    switch (product) {
        case 1: return 1;
        case 2: return 2;
        case 3: return 3;
        default: return 1;
    }
}

gpio_num_t get_led_pin_for_product(int product)
{
    switch (product) {
        case 1: return LED1_PIN;
        case 2: return LED2_PIN;
        case 3: return LED3_PIN;
        default: return LED1_PIN;
    }
}

const char *get_product_name(int product)
{
    switch (product) {
        case 1: return "Produto 1 (LED Vermelho) - Refrigerante";
        case 2: return "Produto 2 (LED Verde) - Snack / Batatas";
        case 3: return "Produto 3 (LED Azul) - Agua Mineral";
        default: return "Produto Desconhecido";
    }
}
