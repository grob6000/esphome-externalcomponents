#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
//#include "esphome/components/text_sensor/text_sensor.h"
//#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace solis_s5 {

class SolisS5Component : public Component, public uart::UARTDevice {
  public:

    void setup() override;
    void loop() override;
    void dump_config() override;

    void set_vdc_1_sensor(sensor::Sensor *s) { vdc1sensor = s; }
    void set_vdc_2_sensor(sensor::Sensor *s) { vdc2sensor = s; }
    void set_vac_u_sensor(sensor::Sensor *s) { vacusensor = s; }
    void set_vac_v_sensor(sensor::Sensor *s) { vacvsensor = s; }
    void set_vac_w_sensor(sensor::Sensor *s) { vacwsensor = s; }

    void set_idc_1_sensor(sensor::Sensor *s) { idc1sensor = s; }
    void set_idc_2_sensor(sensor::Sensor *s) { idc2sensor = s; }
    void set_iac_u_sensor(sensor::Sensor *s) { iacusensor = s; }
    void set_iac_v_sensor(sensor::Sensor *s) { iacvsensor = s; }
    void set_iac_w_sensor(sensor::Sensor *s) { iacwsensor = s; }

    void set_pdc_1_sensor(sensor::Sensor *s) { pdc1sensor = s; }
    void set_pdc_2_sensor(sensor::Sensor *s) { pdc2sensor = s; }
    void set_pac_total_sensor(sensor::Sensor *s) { pactotalsensor = s; }
    void set_vaac_total_sensor(sensor::Sensor *s) { vaactotalsensor = s; }
    void set_pfac_sensor(sensor::Sensor *s) { pfacsensor = s; }

    void set_e_day_sensor(sensor::Sensor *s) { edaysensor = s; }
    void set_e_month_sensor(sensor::Sensor *s) { emonthsensor = s; }
    void set_e_total_sensor(sensor::Sensor *s) { etotalsensor = s; }

    void set_t_igbt_sensor(sensor::Sensor *s) { tigbtsensor = s; }

  protected:
    sensor::Sensor *vdc1sensor;
    sensor::Sensor *vdc2sensor;
    sensor::Sensor *vacusensor;
    sensor::Sensor *vacvsensor;
    sensor::Sensor *vacwsensor;
    
    sensor::Sensor *idc1sensor;
    sensor::Sensor *idc2sensor;
    sensor::Sensor *iacusensor;
    sensor::Sensor *iacvsensor;
    sensor::Sensor *iacwsensor;

    sensor::Sensor *pdc1sensor;
    sensor::Sensor *pdc2sensor;
    sensor::Sensor *pactotalsensor;
    sensor::Sensor *vaactotalsensor;
    sensor::Sensor *pfacsensor;

    sensor::Sensor *edaysensor;
    sensor::Sensor *emonthsensor;
    sensor::Sensor *etotalsensor;

    sensor::Sensor *tigbtsensor;

    uint8_t hex2byte(char* s);
};

} // namespace solis_s5
} // namespace esphome
