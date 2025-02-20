#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN_R = 28;
volatile int event_flag = 0;  

void btn_callback(uint gpio, uint32_t events) {
  if (events == 0x4) { // borda de descida (fall edge)
    event_flag = 1;
  } else if (events == 0x8) { // borda de subida (rise edge)
    event_flag = 2;
  }
}

int main() {
  stdio_init_all();

  gpio_init(BTN_PIN_R);
  gpio_set_dir(BTN_PIN_R, GPIO_IN);
  gpio_pull_up(BTN_PIN_R);

  gpio_set_irq_enabled_with_callback(
      BTN_PIN_R, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &btn_callback);

  while (true) {
    if (event_flag != 0) {
      if (event_flag == 1) {
        printf("fall\n");
      } else if (event_flag == 2) {
        printf("rise\n");
      }
      event_flag = 0;  // reseta a flag após tratar o evento
    }
  }
  return 0;
}
