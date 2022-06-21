# esphome-externalcomponents
Some external components for [esphome](https://esphome.io/index.html).
## Usage
[See here for how to use external components.](https://esphome.io/components/external_components.html).
I've included some examples also.
## Components
### beam_binary_sensor
Intended for infra-red (IR) sensors that include both an led and sensor (e.g. beam sensors,reflective sensors, etc.), e.g. [TCTR5000](https://www.vishay.com/docs/83760/tcrt5000.pdf), [RPR220](https://fscdn.rohm.com/en/products/databook/datasheet/opto/optical_sensor/photosensor/rpr-220.pdf), etc. In particular, may be helpful for optical sensors where ambient light might make erroneous readings (e.g. red lamp detectors on electricity meters).

Beam binary sensor will alternate an output 'driver' (e.g. ir led) `ON` and `OFF`.
Frequency on my device is about 30Hz; this will vary depending on how much other code you have on the device), and only publish `ON` if during the cycle both. This frequency is about half of the frequency of a normal gpio binary sensor. If you want to pick up much shorter duration pulses, this might not be capable - see [pulse_counter](https://esphome.io/components/sensor/pulse_counter.html) instead.
1. Sensor is `ON` when Driver is `ON`
2. Sensor is `OFF` when Driver is `OFF`
This ensures that the sensor is not activated erroneously by ambient light (e.g. sunlight).

Usage is similar to a [gpio_binary_sensor](https://esphome.io/components/binary_sensor/gpio.html). Supports all the configuration [binary_sensor](https://esphome.io/components/binary_sensor/index.html) supports.

| Config | Required? | Type | Description |
| --- | --- | --- | --- |
| `pin_sensor` | required | pin_schema | pin for the sensor input; as per 'pin' configuration of gpio_binary_sensor |
| `pin_driver | required | pin_schema | pin for the output led (or similar) |
| `pin_output` | optional | pin_schema | pin to provide the output of this beam sensor directly (e.g. for a local led) |
