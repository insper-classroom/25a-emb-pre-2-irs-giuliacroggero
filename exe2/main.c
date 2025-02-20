#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int LED_PIN = 4;  
const int BTN_PIN = 28;   

volatile int btn_pressed = 1;  //Flag para sinalizar que o botão foi pressionado

void btn_callback(uint gpio, uint32_t events) {
    if (events == 0x4) {
        btn_pressed = 0;
    }
}

int main() {
    stdio_init_all();

    gpio_init(BTN_PIN);
    gpio_init(LED_PIN);
  
    gpio_set_dir(BTN_PIN, GPIO_IN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
  
    gpio_pull_up(BTN_PIN);
  

    gpio_set_irq_enabled_with_callback(BTN_PIN, GPIO_IRQ_EDGE_FALL, true, btn_callback);

    while (true) {
        if (btn_pressed ==1) {
          btn_pressed = 0;  
          if (gpio_get(LED_PIN) == 0){
            gpio_put(LED_PIN,1);
          }else{
            gpio_put(LED_PIN,0);
          }
        }

      }
}
