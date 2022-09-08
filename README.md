# esphome-externalcomponents
Some external components for [esphome](https://esphome.io/index.html).
## Usage
[See here for how to use external components](https://esphome.io/components/external_components.html).
I've included some examples also.
## beam_binary_sensor
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
| `pin_driver` | required | pin_schema | pin for the output led (or similar) |
| `pin_output` | optional | pin_schema | pin to provide the output of this beam sensor directly (e.g. for a local led) |

## solis_s5
For connection of esphome to a Ginlong Solis S5 solar inverter using the RS485 COM port. I wanted to connect the inverter to Home Assistant without using a cloud service (i.e. on the local network), to minimise delay and dependence on the Solis API and internet connection being functional. However, my solar installer uses the cloud service for warranty and support, so I wanted to keep this data being delivered. The Solis wifi dongle software does not provide reliable means of adding an additional server connection, so I needed to bypass it / work in parallel. I love ESPhome, so here we are.

Currently, this component is designed only to listen-in to the messages sent from the inverter to the wifi dongle containing operational data. It REQUIRES the Solis wifi stick to function. In future, I would like to extend this to allow a full replacement of the solis wifi stick with the esphome device by transmitting the simple request message the dongle makes periodically.

After some effort reverse-engineering the protocol, we can obtain most primary operational values from the inverter at a reasonable frequency (looks like about 1/min).

Config is relatively self-explanatory, please see the example file soliss5.yaml. This shows all the sensors you can configure. All sensors are optional (delete them if you don't want them). Each is a `sensor` schema, so all options from [base sensor configuration](https://esphome.io/components/sensor/index.html#base-sensor-configuration) can be used.

### Hardware:
1. Node-MCU / Wemos or similar ESP8266 device
2. RS485:TTL serial converter
3. Solis Wifi dongle
- The RS485 dongle is connected to the ESP8266 device & taped together (make sure they're not shorted together - a bit of double sided tape worked for me (see connections below). Recommend only connecting RX, as there's no TX functionality at this point (and this might interfere with the correct operation of the exisiting wifi dongle).
- You can disassemble the solis dongle by popping the 4 tabs at the top, and pulling the board/connector out. Inside, the solis board connects to the connector using a 4-pin JST-XH connector, on mine the pins were helpfully labelled GND, A, B, 5V. Connect the esp8266/rs485 module in parallel to the existing connection (e.g. by soldering wires - I used an extra JST connector, soldered onto the back of the board, so I could pop it in and out easily for testing).
- Wrap the piggy-back in insulating tape.
- Reassemble the dongle with the piggy-back inside, slipping the piggy-back down in the large amount of free space on the rear side of the existing board.
- Voila, one parallel wifi connection to your inverter!

| Wemos Pin | RS485 Module TTL Connection | RS485 Module Bus Connection | Solis Wifi Dongle Pin |
| --- | --- | --- | --- |
| GND | | | GND |
| VIN | | | 5V |
| GND | GND | | |
| RX | RXD | | |
| 3V3 | VCC | | |
| GND | | GND | |
| | | A | A |
| | | B | B |
