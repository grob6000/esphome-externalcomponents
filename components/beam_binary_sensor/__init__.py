import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import binary_sensor
from esphome.const import CONF_PIN, CONF_ID

beambinarysensor_ns = cg.esphome_ns.namespace('beam_binary_sensor')
BeamBinarySensor = beambinarysensor_ns.class_('BeamBinarySensor', binary_sensor.BinarySensor, cg.Component)

DEPENDENCIES = []
AUTO_LOAD = ['binary_sensor']

CONF_PIN_SENSOR = "pin_sensor"
CONF_PIN_DRIVER = "pin_driver"
CONF_PIN_OUTPUT = "pin_output"

CONFIG_SCHEMA = binary_sensor.BINARY_SENSOR_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(BeamBinarySensor),
    cv.Required(CONF_PIN_SENSOR): pins.gpio_input_pin_schema,
    cv.Required(CONF_PIN_DRIVER): pins.gpio_output_pin_schema,
    cv.Optional(CONF_PIN_OUTPUT): pins.gpio_output_pin_schema,
}).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = await binary_sensor.new_binary_sensor(config)
    await cg.register_component(var, config)

    p = await cg.gpio_pin_expression(config[CONF_PIN_SENSOR])
    cg.add(var.set_pin_sensor(p))

    p = await cg.gpio_pin_expression(config[CONF_PIN_DRIVER])
    cg.add(var.set_pin_driver(p))

    if CONF_PIN_OUTPUT in config:
      p = await cg.gpio_pin_expression(config[CONF_PIN_OUTPUT])
      cg.add(var.set_pin_output(p))