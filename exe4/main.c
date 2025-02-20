#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>


const int LED_RED_PIN   = 4;  
const int LED_GREEN_PIN = 6; 
const int BTN_RED_PIN   = 28; 
const int BTN_GREEN_PIN = 26;  


volatile int flag_red   = 0;
volatile int flag_green = 0;


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


    gpio_init(LED_RED_PIN);
    gpio_set_dir(LED_RED_PIN, GPIO_OUT);


    gpio_init(LED_GREEN_PIN);
    gpio_set_dir(LED_GREEN_PIN, GPIO_OUT);


    gpio_init(BTN_RED_PIN);
    gpio_set_dir(BTN_RED_PIN, GPIO_IN);
    gpio_pull_up(BTN_RED_PIN);


    gpio_init(BTN_GREEN_PIN);
    gpio_set_dir(BTN_GREEN_PIN, GPIO_IN);
    gpio_pull_up(BTN_GREEN_PIN);

    gpio_set_irq_enabled_with_callback(BTN_RED_PIN, GPIO_IRQ_EDGE_FALL, true, &btn_callback);
    gpio_set_irq_enabled(BTN_GREEN_PIN, GPIO_IRQ_EDGE_RISE, true);

    while (true) {
        if (flag_red == 1) {
            flag_red = 0;
            if (gpio_get(LED_RED_PIN) == 0){
              gpio_put(LED_RED_PIN,1);
            }else{
              gpio_put(LED_RED_PIN,0);
            }
        }else if (flag_green){
        flag_green = 0;
        if (gpio_get(LED_GREEN_PIN) == 0){
          gpio_put(LED_GREEN_PIN,1);
        }else{
          gpio_put(LED_GREEN_PIN,0);
        }
    }
 }

}
