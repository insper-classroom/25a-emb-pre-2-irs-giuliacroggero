#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int LED_PIN = 25;   // LED onboard do RP2040
const int BTN_PIN = 28;   // Pino do botão

volatile bool btn_pressed = false;  // Flag para sinalizar que o botão foi pressionado

// Callback da interrupção: apenas seta a flag se for borda de descida (press)
void btn_callback(uint gpio, uint32_t events) {
    if (events & GPIO_IRQ_EDGE_FALL) {
        btn_pressed = true;
    }
}

int main() {
    stdio_init_all();

    // Inicializa o LED como saída e o desliga inicialmente
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 0);

    // Inicializa o botão como entrada com pull-up
    gpio_init(BTN_PIN);
    gpio_set_dir(BTN_PIN, GPIO_IN);
    gpio_pull_up(BTN_PIN);

    
    gpio_set_irq_enabled_with_callback(BTN_PIN,GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE,true, &btn_callback);

    while (true) {
        if (btn_pressed) {
            btn_pressed = false;  // Reseta a flag após processar o evento
            // Altera o estado do LED (toggle)
            gpio_put(LED_PIN, !gpio_get(LED_PIN));
        }
    }
    return 0;
}
