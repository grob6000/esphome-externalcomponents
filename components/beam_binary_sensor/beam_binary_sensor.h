#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/binary_sensor/binary_sensor.h"

namespace esphome {
namespace beam_binary_sensor {

class BeamBinarySensor : public binary_sensor::BinarySensor, public Component {
 public:
  void set_pin_sensor(GPIOPin *pin) { pin_sensor = pin; }
  void set_pin_driver(GPIOPin *pin) { pin_driver = pin; }
  void set_pin_output(GPIOPin *pin) { pin_output = pin; }
  // ========== INTERNAL METHODS ==========
  // (In most use cases you won't need these)
  /// Setup pin
  void setup() override;
  void dump_config() override;
  /// Hardware priority
  float get_setup_priority() const override;
  /// Check sensor
  void loop() override;

 protected:
  GPIOPin *pin_sensor;
  GPIOPin *pin_driver;
  GPIOPin *pin_output;
};

}  // namespace gpio
}  // namespace esphome