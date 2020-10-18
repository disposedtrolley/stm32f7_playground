#include <zephyr.h>
#include <drivers/gpio.h>
#include <drivers/adc.h>
#include <device.h>

static const struct adc_channel_cfg adc_cfg = {
    .gain = ADC_GAIN_1,
    .reference = ADC_REF_INTERNAL,
    .channel_id = 9
};

static uint16_t m_sample_buffer[6];

void main() {
  // LD1 is connected to PB0.
  const struct device *pb = device_get_binding("GPIOB");
  gpio_pin_configure(pb, 0, GPIO_OUTPUT);

  const struct device* adc = device_get_binding("ADC_1");
  if (adc == nullptr) {
    printk("failed to get ADC_1\n");
  }

  int ret = adc_channel_setup(adc, &adc_cfg);
  if (ret != 0) {
    printk("failed to setup ADC_1: %d\n", ret);
  }

  const struct adc_sequence sequence = {
		.channels    = BIT(9),
		.buffer      = m_sample_buffer,
		.buffer_size = sizeof(m_sample_buffer),
		.resolution  = 12,
	};

  while (true) {
    int ret = adc_read(adc, &sequence);
    if (ret != 0) {
      printk("adc_read() failed with code %d", ret);
    } else {
      printk("adc_read() value %d\n", m_sample_buffer[0]);
    }

    // Blinky
    gpio_pin_toggle(pb, 0);
    k_msleep(500);
  }
}