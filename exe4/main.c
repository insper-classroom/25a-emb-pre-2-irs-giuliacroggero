#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

// Definições dos pinos
const int LED_RED_PIN   = 15;  // LED vermelho (pode ser um LED externo)
const int LED_GREEN_PIN = 16;  // LED verde (pode ser um LED externo)
const int BTN_RED_PIN   = 28;  // Botão vermelho
const int BTN_GREEN_PIN = 26;  // Botão verde

// Flags para sinalizar os eventos dos botões
volatile bool flag_red   = false;
volatile bool flag_green = false;

// Callback para ambos os botões
void gpio_callback(uint gpio, uint32_t events) {
    // Botão vermelho: aciona na borda de descida (quando pressionado)
    if (gpio == BTN_RED_PIN && (events & GPIO_IRQ_EDGE_FALL)) {
        flag_red = true;
    }
    // Botão verde: aciona na borda de subida (quando solto)
    else if (gpio == BTN_GREEN_PIN && (events & GPIO_IRQ_EDGE_RISE)) {
        flag_green = true;
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
