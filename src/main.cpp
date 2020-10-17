#include <zephyr.h>
#include <drivers/gpio.h>
#include <device.h>

void main() {
  struct device* pb0 = device_get_binding("GPIOB");
  gpio_pin_configure(pb0, 0, GPIO_OUTPUT);

  while (true) {
    gpio_pin_toggle(pb0, 0);
    k_sleep(K_SECONDS(1));
  }
}