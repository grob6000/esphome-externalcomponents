import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, binary_sensor, text_sensor, uart
from esphome.const import *
solis_s5_ns = cg.esphome_ns.namespace('solis_s5')
SolisS5Component = solis_s5_ns.class_('SolisS5Component', cg.PollingComponent)

DEPENDENCIES = ['uart']
AUTO_LOAD = ['uart', 'sensor', 'text_sensor', 'binary_sensor']

CONF_VDC_1 = "voltage_dc_1"
CONF_IDC_1 = "current_dc_1"
CONF_PDC_1 = "power_dc_1"

CONF_VDC_2 = "voltage_dc_2"
CONF_IDC_2 = "current_dc_2"
CONF_PDC_2 = "power_dc_2"

CONF_VAC_U = "voltage_ac_u"
CONF_IAC_U = "current_ac_u"
CONF_VAC_V = "voltage_ac_v"
CONF_IAC_V = "current_ac_v"
CONF_VAC_W = "voltage_ac_w"
CONF_IAC_W = "current_ac_w"
CONF_FAC = "frequency"
CONF_PAC_TOTAL = "power_ac_total"
CONF_VAAC_TOTAL = "va_ac_total"
CONF_PFAC = "powerfactor_ac"

CONF_E_DAY = "energy_today"
CONF_E_MONTH = "energy_thismonth"
CONF_E_TOTAL = "energy_total"

CONF_T_IGBT = "temperature_igbt"

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(SolisS5Component),

    cv.Optional(CONF_VDC_1):
        sensor.sensor_schema(device_class=DEVICE_CLASS_VOLTAGE,unit_of_measurement=UNIT_VOLT,accuracy_decimals=1,state_class=STATE_CLASS_MEASUREMENT).extend(),
    cv.Optional(CONF_VDC_2):
        sensor.sensor_schema(device_class=DEVICE_CLASS_VOLTAGE,unit_of_measurement=UNIT_VOLT,accuracy_decimals=1,state_class=STATE_CLASS_MEASUREMENT).extend(),
    cv.Optional(CONF_VAC_U):
        sensor.sensor_schema(device_class=DEVICE_CLASS_VOLTAGE,unit_of_measurement=UNIT_VOLT,accuracy_decimals=1,state_class=STATE_CLASS_MEASUREMENT).extend(),  
    cv.Optional(CONF_VAC_V):
        sensor.sensor_schema(device_class=DEVICE_CLASS_VOLTAGE,unit_of_measurement=UNIT_VOLT,accuracy_decimals=1,state_class=STATE_CLASS_MEASUREMENT).extend(), 
    cv.Optional(CONF_VAC_W):
        sensor.sensor_schema(device_class=DEVICE_CLASS_VOLTAGE,unit_of_measurement=UNIT_VOLT,accuracy_decimals=1,state_class=STATE_CLASS_MEASUREMENT).extend(),   

    cv.Optional(CONF_IDC_1):
        sensor.sensor_schema(device_class=DEVICE_CLASS_CURRENT,unit_of_measurement=UNIT_AMPERE,accuracy_decimals=1,state_class=STATE_CLASS_MEASUREMENT).extend(),   
    cv.Optional(CONF_IDC_2):
        sensor.sensor_schema(device_class=DEVICE_CLASS_CURRENT,unit_of_measurement=UNIT_AMPERE,accuracy_decimals=1,state_class=STATE_CLASS_MEASUREMENT).extend(),   
    cv.Optional(CONF_IAC_U):
        sensor.sensor_schema(device_class=DEVICE_CLASS_CURRENT,unit_of_measurement=UNIT_AMPERE,accuracy_decimals=1,state_class=STATE_CLASS_MEASUREMENT).extend(),  
    cv.Optional(CONF_IAC_V):
        sensor.sensor_schema(device_class=DEVICE_CLASS_CURRENT,unit_of_measurement=UNIT_AMPERE,accuracy_decimals=1,state_class=STATE_CLASS_MEASUREMENT).extend(), 
    cv.Optional(CONF_IAC_W):
        sensor.sensor_schema(device_class=DEVICE_CLASS_CURRENT,unit_of_measurement=UNIT_AMPERE,accuracy_decimals=1,state_class=STATE_CLASS_MEASUREMENT).extend(),  
    
    cv.Optional(CONF_PDC_1):
        sensor.sensor_schema(device_class=DEVICE_CLASS_POWER,unit_of_measurement=UNIT_WATT,accuracy_decimals=0,state_class=STATE_CLASS_MEASUREMENT).extend(), 
    cv.Optional(CONF_PDC_2):
        sensor.sensor_schema(device_class=DEVICE_CLASS_POWER,unit_of_measurement=UNIT_WATT,accuracy_decimals=0,state_class=STATE_CLASS_MEASUREMENT).extend(), 
    cv.Optional(CONF_PAC_TOTAL):
        sensor.sensor_schema(device_class=DEVICE_CLASS_POWER,unit_of_measurement=UNIT_WATT,accuracy_decimals=0,state_class=STATE_CLASS_MEASUREMENT).extend(), 
    cv.Optional(CONF_VAAC_TOTAL):
        sensor.sensor_schema(device_class=DEVICE_CLASS_APPARENT_POWER,unit_of_measurement=UNIT_VOLT_AMPS,accuracy_decimals=0,state_class=STATE_CLASS_MEASUREMENT).extend(),    
    cv.Optional(CONF_PFAC):
        sensor.sensor_schema(device_class=DEVICE_CLASS_POWER_FACTOR,accuracy_decimals=3,state_class=STATE_CLASS_MEASUREMENT).extend(),

    cv.Optional(CONF_E_DAY):
        sensor.sensor_schema(device_class=DEVICE_CLASS_ENERGY,unit_of_measurement=UNIT_KILOWATT_HOURS,accuracy_decimals=1,state_class=STATE_CLASS_TOTAL_INCREASING).extend(),    
    cv.Optional(CONF_E_MONTH):
        sensor.sensor_schema(device_class=DEVICE_CLASS_ENERGY,unit_of_measurement=UNIT_KILOWATT_HOURS,accuracy_decimals=0,state_class=STATE_CLASS_TOTAL_INCREASING).extend(), 
    cv.Optional(CONF_E_TOTAL):
        sensor.sensor_schema(device_class=DEVICE_CLASS_ENERGY,unit_of_measurement=UNIT_KILOWATT_HOURS,accuracy_decimals=0,state_class=STATE_CLASS_TOTAL_INCREASING).extend(),         

    cv.Optional(CONF_T_IGBT):
        sensor.sensor_schema(device_class=DEVICE_CLASS_TEMPERATURE,unit_of_measurement=UNIT_CELSIUS,accuracy_decimals=1,state_class=STATE_CLASS_MEASUREMENT).extend(),       

}).extend(cv.polling_component_schema('60s')).extend(uart.UART_DEVICE_SCHEMA)

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield uart.register_uart_device(var, config)
    
    if CONF_VDC_1 in config:
        conf = config[CONF_VDC_1]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_vdc_1_sensor(sens))

    if CONF_VDC_2 in config:
        conf = config[CONF_VDC_2]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_vdc_2_sensor(sens))

    if CONF_VAC_U in config:
        conf = config[CONF_VAC_U]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_vac_u_sensor(sens))
    
    if CONF_VAC_V in config:
        conf = config[CONF_VAC_V]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_vac_v_sensor(sens))

    if CONF_VAC_W in config:
        conf = config[CONF_VAC_W]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_vac_w_sensor(sens))



    if CONF_IDC_1 in config:
        conf = config[CONF_IDC_1]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_idc_1_sensor(sens))

    if CONF_IDC_2 in config:
        conf = config[CONF_IDC_2]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_idc_2_sensor(sens))

    if CONF_IAC_U in config:
        conf = config[CONF_IAC_U]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_iac_u_sensor(sens))
    
    if CONF_IAC_V in config:
        conf = config[CONF_IAC_V]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_iac_v_sensor(sens))

    if CONF_IAC_W in config:
        conf = config[CONF_IAC_W]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_iac_w_sensor(sens))



    if CONF_PDC_1 in config:
        conf = config[CONF_PDC_1]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_pdc_1_sensor(sens))

    if CONF_PDC_2 in config:
        conf = config[CONF_PDC_2]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_pdc_2_sensor(sens))

    if CONF_PAC_TOTAL in config:
        conf = config[CONF_PAC_TOTAL]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_pac_total_sensor(sens))

    if CONF_VAAC_TOTAL in config:
        conf = config[CONF_VAAC_TOTAL]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_vaac_total_sensor(sens))

    if CONF_PFAC in config:
        conf = config[CONF_PFAC]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_pfac_sensor(sens))



    if CONF_E_DAY in config:
        conf = config[CONF_E_DAY]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_e_day_sensor(sens))

    if CONF_E_MONTH in config:
        conf = config[CONF_E_MONTH]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_e_month_sensor(sens))

    if CONF_E_TOTAL in config:
        conf = config[CONF_E_TOTAL]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_e_total_sensor(sens))



    if CONF_T_IGBT in config:
        conf = config[CONF_T_IGBT]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_t_igbt_sensor(sens))