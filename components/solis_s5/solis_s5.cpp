#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
//#include "esphome/components/text_sensor/text_sensor.h"
//#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/uart/uart.h"
#include "esphome/core/log.h"
#include "solis_s5.h"

#define SOLIS_S5_LOOP_WAIT 3
#define SOLIS_S5_SERIAL_BUFFER_LEN 150

namespace esphome {
namespace solis_s5 {

static const char *TAG = "solis_s5";

void SolisS5Component::setup() {
  // nothing to do here
}

void SolisS5Component::loop() {
  static char buffer[SOLIS_S5_SERIAL_BUFFER_LEN] = {0};
  static uint8_t index = 0;
  static uint8_t loopwait = 0;

  while (available()) {
    buffer[index] = read();
    index++;
    index%=SOLIS_S5_SERIAL_BUFFER_LEN;
    loopwait = 0;
  }
  if (index > 0) {
    loopwait++;
  }
  
  if (loopwait > SOLIS_S5_LOOP_WAIT) { // some time has passed without receiving another character. this should be the end of a message.
    ESP_LOGV(TAG, "message recieved len=%d", index);
    if (buffer[0] == 126) { // message starts with the right preamble
      uint8_t msglen = buffer[3];
      if (index == msglen + 5) { // messasge has correct length
        uint8_t csum = 0;
        for (uint8_t i = 1; i < msglen+4; i++) { // csum after preamble, before tx csum
          csum += buffer[i];
        }
        if (csum == buffer[msglen+4]) { // checksum ok
        
          if ((buffer[2] == 161) && (msglen == 80)) { // inverter response; parse and update sensors

            ESP_LOGD(TAG, "inverter data received");

            if (this->vdc1sensor != nullptr) {
              uint16_t v = buffer[4] + buffer[5]*256;
              this->vdc1sensor->publish_state((float)v * 0.1f);
            }
            if (this->vdc2sensor != nullptr) {
              uint16_t v = buffer[28] + buffer[29]*256;
              this->vdc2sensor->publish_state((float)v * 0.1f);
            }
            if (this->vacusensor != nullptr) {
              uint16_t v = buffer[8] + buffer[9]*256;
              this->vacusensor->publish_state((float)v * 0.1f);
            }
            if (this->vacvsensor != nullptr) {
              uint16_t v = buffer[74] + buffer[75]*256;
              this->vacvsensor->publish_state((float)v * 0.1f);
            }
            if (this->vacwsensor != nullptr) {
              uint16_t v = buffer[70] + buffer[71]*256;
              this->vacwsensor->publish_state((float)v * 0.1f);
            }

            if (this->idc1sensor != nullptr) {
              uint16_t v = buffer[6] + buffer[7]*256;
              this->idc1sensor->publish_state((float)v * 0.1f);
            }
            if (this->idc2sensor != nullptr) {
              uint16_t v = buffer[30] + buffer[31]*256;
              this->idc2sensor->publish_state((float)v * 0.1f);
            }
            if (this->iacusensor != nullptr) {
              uint16_t v = buffer[10] + buffer[11]*256;
              this->iacusensor->publish_state((float)v * 0.1f);
            }
            if (this->iacvsensor != nullptr) {
              uint16_t v = buffer[76] + buffer[77]*256;
              this->iacvsensor->publish_state((float)v * 0.1f);
            }
            if (this->iacwsensor != nullptr) {
              uint16_t v = buffer[72] + buffer[73]*256;
              this->iacwsensor->publish_state((float)v * 0.1f);
            }

            if (this->pdc1sensor != nullptr) {
              uint16_t v1 = buffer[4] + buffer[5]*256;
              uint16_t i1 = buffer[6] + buffer[7]*256;
              this->pdc1sensor->publish_state((float)v1 * (float)i1 * 0.01f);
            }
            
            if (this->pdc2sensor != nullptr) {
              uint16_t v2 = buffer[28] + buffer[29]*256;
              uint16_t i2 = buffer[30] + buffer[31]*256;
              this->pdc2sensor->publish_state((float)v2 * (float)i2 * 0.01f);
            }
            
            if (this->pactotalsensor != nullptr) {
              uint16_t v = buffer[59] + buffer[60]*256;
              this->pactotalsensor->publish_state((float)v);
            }
            
            if (this->vaactotalsensor != nullptr) {
              uint16_t v = buffer[65] + buffer[66]*256;
              this->vaactotalsensor->publish_state((float)v);
            }
            if (this->pfacsensor != nullptr) {
              uint16_t v = buffer[68] + buffer[69]*256;
              this->pfacsensor->publish_state((float)v * 0.01f);
            }            

            if (this->edaysensor != nullptr) {
              uint16_t v = buffer[37] + buffer[38]*256;
              this->edaysensor->publish_state((float)v*0.1f);
            }
            if (this->emonthsensor != nullptr) {
              uint16_t v = buffer[33] + buffer[34]*256;
              this->emonthsensor->publish_state((float)v);
            }
            if (this->etotalsensor != nullptr) {
              uint16_t v = buffer[14] + buffer[15]*256;
              this->etotalsensor->publish_state((float)v);
            }

            if (this->tigbtsensor != nullptr) {
              uint16_t v = buffer[12] + buffer[13]*256;
              this->tigbtsensor->publish_state((float)v*0.1f);
            }
  
          } else if ((buffer[2] == 193) && (msglen == 40)) { // inverter config response
            ESP_LOGD(TAG, "inverter config response received");
          }
          
        } else {
          ESP_LOGD(TAG, "message checksum fail; discarding. csum = 0x%02X, check = 0x%02X", buffer[msglen+4], csum);
        }
      } else if ((msglen == 0) && (index == 55)) { // wifi stick command
        ESP_LOGD(TAG, "wifi stick command received; ignoring");
      } else {
        ESP_LOGD(TAG, "message insufficient length (requested: %d, received: %d); discarding", msglen+5, index);
      }
    } else {
      ESP_LOGD(TAG, "message received, invalid start character");
    }
    // reset message, ready for next one
    loopwait = 0;
    index = 0;
  }

}

void SolisS5Component::dump_config() {
  ESP_LOGCONFIG(TAG, "Solis S5 Component");
}

} // namespace dekacontroller
} // namespace esphome
