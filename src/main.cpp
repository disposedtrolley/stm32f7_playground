#include <device.h>
#include <drivers/adc.h>
#include <drivers/gpio.h>
#include <zephyr.h>

struct ADCChannel {
  uint8_t id;
  uint8_t resolution;
  uint16_t buffer[1];
};

void main() {
  // LD1 is connected to PB0.
  const struct device *pb = device_get_binding("GPIOB");
  gpio_pin_configure(pb, 0, GPIO_OUTPUT);

  // Configure the ADC channels we're interested in.
  ADCChannel const channels[] = {
    {
      .id = 9,
      .resolution = 12,
      .buffer = {},
    },
    {
      .id = 12,
      .resolution = 12,
      .buffer = {},
    }
  };

  // Get ADC1.
  const struct device *adc_dev = device_get_binding("ADC_1");
  if (adc_dev == nullptr) {
    printk("failed to get ADC_1\n");
    return;
  }

  // Setup channels for ADC1.
  for (ADCChannel chan : channels) {
    struct adc_channel_cfg adc_cfg = {
      .gain = ADC_GAIN_1,
      .reference = ADC_REF_INTERNAL,
      .channel_id = chan.id,
    };
    int err = adc_channel_setup(adc_dev, &adc_cfg);
    if (err != 0) {
      printk("adc_channel_setup() :: failed to setup channel %d: %d\n", chan.id, err);
      return;
    }
  }

  while (true) {
    // Read each channel consecutively. The STM32 ADC driver doesn't support
    // reading from multiple channels in the same sequence struct:
    // https://github.com/zephyrproject-rtos/zephyr/pull/13447#issuecomment-464420196
    for (ADCChannel chan : channels) {
      struct adc_sequence seq_cfg = {
        .channels = BIT(chan.id),
        .buffer = chan.buffer,
        .buffer_size = sizeof(chan.buffer),
        .resolution = chan.resolution,
      };

      int ret = adc_read(adc_dev, &seq_cfg);
      if (ret != 0) {
        printk("adc_read() :: failed with code %d", ret);
        return;
      } 
      
      printk("adc_read() :: channel %d: %d\n", chan.id, chan.buffer[0]);
    }
    
    // Blinky
    gpio_pin_toggle(pb, 0);
    k_msleep(500);
  }
}
