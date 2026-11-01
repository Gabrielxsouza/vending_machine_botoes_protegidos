/*
 * buzzer.h - Buzzer passivo acionado por PWM (LEDC).
 */
#ifndef BUZZER_H
#define BUZZER_H

/* O PWM do buzzer e configurado em hardware_init() (hardware.c). */

/* Emite um tom de 'freq' Hz por 'duration_ms' ms (bloqueante). */
void beep(int freq, int duration_ms);

/* Sons de feedback. */
void play_coin_sound(void);
void play_success_sound(void);
void play_error_sound(void);
void play_change_coin_sound(void);  /* 1 bipe por moeda de troco */

#endif /* BUZZER_H */
