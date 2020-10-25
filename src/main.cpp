#include <device.h>
#include <drivers/adc.h>
#include <drivers/gpio.h>
#include <zephyr.h>

static const struct adc_channel_cfg adc_channel_9_cfg = {
    .gain = ADC_GAIN_1,
    .reference = ADC_REF_INTERNAL,
    .channel_id = 9
};

static const struct adc_channel_cfg adc_channel_12_cfg = {
    .gain = ADC_GAIN_1,
    .reference = ADC_REF_INTERNAL,
    .channel_id = 12
};

static uint16_t m_sample_buffer[1];
static uint16_t m_sample_buffer2[1];

void main() {
  // LD1 is connected to PB0.
  const struct device *pb = device_get_binding("GPIOB");
  gpio_pin_configure(pb, 0, GPIO_OUTPUT);

  const struct device *adc = device_get_binding("ADC_1");
  if (adc == nullptr) {
    printk("failed to get ADC_1\n");
  }

  int ret = adc_channel_setup(adc, &adc_channel_9_cfg);
  if (ret != 0) {
    printk("failed to setup ADC1_IN9: %d\n", ret);
  }

  ret = adc_channel_setup(adc, &adc_channel_12_cfg);
  if (ret != 0) {
    printk("failed to setup ADC1_IN12: %d\n", ret);
  }

  const struct adc_sequence sequence_chan_9 = {
      .channels = BIT(9),
      .buffer = m_sample_buffer,
      .buffer_size = sizeof(m_sample_buffer),
      .resolution = 12,
  };

  const struct adc_sequence sequence_chan_12 = {
      .channels = BIT(12),
      .buffer = m_sample_buffer2,
      .buffer_size = sizeof(m_sample_buffer2),
      .resolution = 12,
  };


  while (true) {
    int ret = adc_read(adc, &sequence_chan_9);
    if (ret != 0) {
      printk("adc_read() failed with code %d", ret);
    } else {
      printk("channel 9: %d\n", m_sample_buffer[0]);
    }

    ret = adc_read(adc, &sequence_chan_12);
    if (ret != 0) {
      printk("adc_read() failed with code %d", ret);
    } else {
      printk("channel 12: %d\n", m_sample_buffer2[0]);
    }

    // Blinky
    gpio_pin_toggle(pb, 0);
    k_msleep(500);
  }
}
