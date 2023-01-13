# esphome-externalcomponents
Some external components for [esphome](https://esphome.io/index.html). Please refer to the esphome website for information about how esphome works, and how to set up and flash devices with esphome.
## Usage
[See here for how to use external components](https://esphome.io/components/external_components.html).
I've included some examples also.
## beam_binary_sensor
Intended for infra-red (IR) sensors that include both an led and sensor (e.g. beam sensors,reflective sensors, etc.), e.g. [TCTR5000](https://www.vishay.com/docs/83760/tcrt5000.pdf), [RPR220](https://fscdn.rohm.com/en/products/databook/datasheet/opto/optical_sensor/photosensor/rpr-220.pdf), etc. In particular, may be helpful for optical sensors where ambient light might make erroneous readings (e.g. rotating dials on gas and water meters).

Beam binary sensor will alternate an output 'driver' (e.g. ir led) `ON` and `OFF`, and only publish `ON` if: 
1. Sensor is `ON` when Driver is `ON`
2. Sensor is `OFF` when Driver is `OFF`
This ensures that the sensor is not activated erroneously by ambient light (e.g. sunlight).

Frequency on my device is about 30Hz; this will vary depending on how much other code you have on the device (it's using the loop). This frequency is about half the polling frequency of a normal gpio binary sensor. If you want to pick up much shorter duration pulses, this might not be capable - you may be better off using a regular [pulse_counter](https://esphome.io/components/sensor/pulse_counter.html) instead.

Usage is similar to a [gpio_binary_sensor](https://esphome.io/components/binary_sensor/gpio.html). Supports all the configuration [binary_sensor](https://esphome.io/components/binary_sensor/index.html) supports.

| Config | Required? | Type | Default | Description |
| --- | --- | --- | --- | --- |
| `pin_sensor` | required | pin_schema | | pin for the sensor input; as per 'pin' configuration of gpio_binary_sensor |
| `pin_driver` | required | pin_schema | | pin for the output led (or similar) |
| `pin_output` | optional | pin_schema | none | pin to output the state of the beam sensor directly (e.g. for a local led) |

## solis_s5
For connection of esphome to a Ginlong Solis S5 solar inverter using the RS485 COM port. I wanted to connect the inverter to Home Assistant without using a cloud service (i.e. on the local network), to minimise delay and dependence on the Solis API and internet connection being functional. However, my solar installer uses the cloud service for warranty and support, so I wanted to keep this data being delivered. The Solis wifi dongle software does not provide reliable means of adding an additional server connection, so I needed to bypass it / work in parallel. I love ESPhome, so here we are.

Currently, this component is designed only to listen-in to the messages sent from the inverter to the wifi dongle containing operational data. It REQUIRES the Solis wifi stick to function. In future, I would like to extend this to allow a full replacement of the solis wifi stick with the esphome device by transmitting the simple request message the dongle makes periodically.

After some effort reverse-engineering the protocol, we can obtain most primary operational values from the inverter at a reasonable frequency (looks like about 1/min).

Config is relatively self-explanatory, please see the example file soliss5.yaml. This shows all the sensors you can configure. All sensors are optional (delete them if you don't want them). Each is a `sensor` schema, so all options from [base sensor configuration](https://esphome.io/components/sensor/index.html#base-sensor-configuration) can be used. You can also configure a polling interval (default is 60s), and the uart port to use (by id).

| Config | Required? | Type | Default | Description |
| --- | --- | --- | --- | --- |
| `uart_id` | optional | `id` | `UART0` | uart bus to read from; default is UART0 (pin1/3) |
| `update_interval` | optional | time | `'60s'` | minimum interval for reporting values. only sends the last received state each period. set to 1s to receive everything |
| `voltage_dc_1` | optional | `sensor_schema` | none | Inverter DC Voltage 1 |
| `voltage_dc_2` | optional | `sensor_schema` | none | Inverter DC Voltage 2 |
| ... etc ... | | | | see example config [soliss5.yaml](https://github.com/grob6000/esphome-externalcomponents/blob/master/soliss5.yaml) for full list of sensors|

### Hardware:
1. Node-MCU / Wemos or similar ESP8266 or ESP32 device
2. RS485:TTL serial converter
3. Solis Wifi dongle

Connect as per [schematic](https://github.com/grob6000/esphome-externalcomponents/blob/master/solis_piggyback_schematic_0.pdf).

I did the following:
1. Assembled and connected the esp and rs485 modules together & wrapped with insulation tape.
2. Soldered wires between this and the back of the 4-pin JST connector on the solis wifi stick board.
3. Placing this against the back of the solis wifi stick board, the whole assembly could be reinstalled in the original case.
