#include "beam_binary_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace beam_binary_sensor {

static const char *const TAG = "beam_binary_sensor";
static const uint8_t CYCLECOUNT = 2; // two-stroke

void BeamBinarySensor::setup() {
  this->pin_sensor->setup();
  this->pin_driver->setup();
  this->pin_driver->digital_write(false); // driver off initially
  if (this->pin_output != nullptr) { // if configured
    this->pin_output->setup();
    this->pin_output->digital_write(false); // output off initially
  }
  //this->publish_initial_state(false);
}

void BeamBinarySensor::dump_config() {
  LOG_BINARY_SENSOR("", "Beam Binary Sensor", this);
  LOG_PIN("  Sensor Pin: ", this->pin_sensor);
  LOG_PIN("  Driver Pin: ", this->pin_driver);
  LOG_PIN("  Output Pin: ", this->pin_output); // note this checks for null internally
}

void BeamBinarySensor::loop() {
  static uint8_t cyclestate = 0; // keep track of where in the cycle we are
  static bool v = true; // state is invalidated if input is not consistent with beam detection, per cycle
  switch (cyclestate) {
    case 0:
      // read sensor (if not low, then value is false - ambient effect)
      if (this->pin_sensor->digital_read()) {
        v = false;
      }
      // drive output
      this->pin_driver->digital_write(true);
      break;
    case 1:
      // read sensor (if not high, then value is false - no detection)
      if (!(this->pin_sensor->digital_read())) {
        v = false;
      }
      // publish state based on last cycle
      this->publish_state(v);
      // update output pin (if configured) to match
      if (this->pin_output != nullptr) {
        this->pin_output->digital_write(v);
      }
      // release driver
      this->pin_driver->digital_write(false); // release driver
      v = true; // reset value for next cycle
      break;
    default:
      // reset if confused
      this->pin_driver->digital_write(false);
      v = true;
      cyclestate = 0;
      break;
  }
  // progress through cycle
  cyclestate++;
  cyclestate%=CYCLECOUNT;
  
}

float BeamBinarySensor::get_setup_priority() const { return setup_priority::HARDWARE; }

}  // namespace gpio
}  // namespace esphome