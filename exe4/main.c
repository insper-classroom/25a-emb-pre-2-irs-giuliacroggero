#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

// Definições dos pinos
const int LED_RED_PIN   = 4;  
const int LED_GREEN_PIN = 6; 
const int BTN_RED_PIN   = 28; 
const int BTN_GREEN_PIN = 26;  

// Flags para sinalizar os eventos dos botões
volatile int flag_red   = 0;
volatile int flag_green = 0;

// Callback para ambos os botões
void btn_callback(uint gpio, uint32_t events) {
    if (events == 0x4) {
        flag_red = 1;
    }
    else if (events == 0x8) {
        flag_green = 1;
    }
}

int main() {
    stdio_init_all();

    // Configura LED vermelho
    gpio_init(LED_RED_PIN);
    gpio_set_dir(LED_RED_PIN, GPIO_OUT);
    gpio_put(LED_RED_PIN, 0);  // Inicialmente desligado

    // Configura LED verde
    gpio_init(LED_GREEN_PIN);
    gpio_set_dir(LED_GREEN_PIN, GPIO_OUT);
    gpio_put(LED_GREEN_PIN, 0);  // Inicialmente desligado

    // Configura botão vermelho com pull-up
    gpio_init(BTN_RED_PIN);
    gpio_set_dir(BTN_RED_PIN, GPIO_IN);
    gpio_pull_up(BTN_RED_PIN);

    // Configura botão verde com pull-up
    gpio_init(BTN_GREEN_PIN);
    gpio_set_dir(BTN_GREEN_PIN, GPIO_IN);
    gpio_pull_up(BTN_GREEN_PIN);

    // Configura as interrupções com callback para ambos os botões
    gpio_set_irq_enabled_with_callback(BTN_RED_PIN, GPIO_IRQ_EDGE_FALL, true, gpio_callback);
    gpio_set_irq_enabled_with_callback(BTN_GREEN_PIN, GPIO_IRQ_EDGE_RISE, true, gpio_callback);

    while (true) {
        if (flag_red) {
            flag_red = false;
            // Alterna o estado do LED vermelho
            gpio_put(LED_RED_PIN, !gpio_get(LED_RED_PIN));
        }
        if (flag_green) {
            flag_green = false;
            // Alterna o estado do LED verde
            gpio_put(LED_GREEN_PIN, !gpio_get(LED_GREEN_PIN));
        }
        sleep_ms(10);  // Pequena pausa para evitar busy waiting
    }
    return 0;
}
