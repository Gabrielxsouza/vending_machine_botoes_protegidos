/*
 * produtos.h - Catalogo de produtos (preco, LED e nome).
 */
#ifndef PRODUTOS_H
#define PRODUTOS_H

#include "driver/gpio.h"

/* Preco do produto em moedas. */
int get_product_price(int product);

/* GPIO do LED associado ao produto. */
gpio_num_t get_led_pin_for_product(int product);

/* Nome descritivo do produto. */
const char *get_product_name(int product);

#endif /* PRODUTOS_H */
