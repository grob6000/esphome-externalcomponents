#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
//#include "esphome/components/text_sensor/text_sensor.h"
//#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/uart/uart.h"
#include "esphome/core/log.h"
#include "solis_s5.h"

#define SOLIS_S5_LOOP_WAIT 2
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
    if (buffer[0] == 126) { // message starts with the right preamble
      uint8_t msglen = buffer[3];
      if (index == msglen + 5) { // messasge has correct length
        uint8_t csum = 0;
        for (uint8_t i = 1; i < msglen+4; i++) { // csum after preamble, before tx csum
          csum += buffer[i];
        }
        if (csum == buffer[msglen+4]) { // checksum ok
          if ((buffer[2] == 161) && (msglen == 80)) { // inverter response; parse and update sensors

            ESP_LOGD(TAG, "inverter reponse received; processing");

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
              uint16_t v = buffer[4] + buffer[5]*256;
              uint16_t i = buffer[6] + buffer[7]*256;
              this->pdc1sensor->publish_state((float)v * (float)i * 0.01f);
            }
            if (this->pdc2sensor != nullptr) {
              uint16_t v = buffer[28] + buffer[29]*256;
              uint16_t i = buffer[30] + buffer[31]*256;
              this->pdc1sensor->publish_state((float)v * (float)i * 0.01f);
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
        ESP_LOGD(TAG, "message insufficient length (%d); discarding", msglen);
      }
    }
    // reset message, ready for next one
    loopwait = 0;
    index = 0;
  }

/*
  if ((index >= 17) && (buffer[index-1] == '\n')) {
    // strip
    buffer[index-1] = 0;
    if (buffer[index-2]=='\r') { buffer[index-2] = 0; }
    // log input
    ESP_LOGD(TAG, "Received: %s", buffer);
    // get data
    uint8_t b_status = hex2byte(&buffer[0]);
    StatusStruct *status = reinterpret_cast<StatusStruct *>(&b_status);
    uint8_t b_mode = hex2byte(&buffer[2]);
    ModeStruct *mode = reinterpret_cast<ModeStruct *>(&b_mode);
    uint8_t b_io = hex2byte(&buffer[4]);
    int16_t drift = hex2byte(&buffer[6])*0x100 + hex2byte(&buffer[8]);
    int16_t tz = hex2byte(&buffer[10])*0x100 + hex2byte(&buffer[12]);
    // calculate checksum
    uint8_t csum_check = 0;
    uint8_t i = 0;
    for (i = 0; i < strlen(buffer); i++) {
      if (buffer[i] == '*') { break; }
      csum_check ^= buffer[i];
    }
    uint8_t b_csum = hex2byte(&buffer[i+1]);
    ESP_LOGD(TAG, "csum received: 0x%02X csum calculated: 0x%02X", b_csum, csum_check);
    if (b_csum == csum_check) {
      // gps sensor
      if (this->gpssensor != nullptr) {
        if (status->gps_hascomms) {
          if (status->gps_hasfix) {
            this->gpssensor->publish_state("OK");
          } else if (status->gps_hastime) {
            this->gpssensor->publish_state("No Fix");
          } else {
            this->gpssensor->publish_state("Init");
          }
        } else {
          this->gpssensor->publish_state("No Comms");
        }
      }
      // sync sensor
      if (this->syncsensor != nullptr) {
        const char* s;
        switch (mode->syncstate) {
          case Ok:
            s = "OK";
            break;
          case Begin:
            s = "Begin";
            break;
          case ZeroM0:
            s = "Zero M";
            break;
          case ZeroH:
            s = "Zero H";
            break;
          case SetH:
            s = "Set H";
            break;
          case SetM:
            s = "Set M";
            break;
          case WaitMark:
            s = "Wait Mark";
            break;
          case None:
            s = "None";
            break;
          case Error:
          default:
            s = "Error";
            break;
        }
        this->syncsensor->publish_state(s);
      }
      // track sensor
      if (this->tracksensor != nullptr) {
        if (status->time_error) {
          this->tracksensor->publish_state("Error");
        } else if (status->time_drift) {
          this->tracksensor->publish_state("Drift");
        } else {
          this->tracksensor->publish_state("OK");
        }
      }
      // run sensor
      if (this->run_ok != nullptr) {
        this->run_ok->publish_state(status->run_ok);
      }
      // drift sensor
      if (this->driftsensor != nullptr) {
        this->driftsensor->publish_state(drift);
      }
      // io sensors
      if (this->zero_m0 != nullptr) {
        this->zero_m0->publish_state((bool)(b_io & 0x01));
      }
      if (this->zero_m00 != nullptr) {
        this->zero_m00->publish_state((bool)(b_io & 0x02));
      }
      if (this->zero_h0 != nullptr) {
        this->zero_h0->publish_state((bool)(b_io & 0x04));
      }
      if (this->zero_h00 != nullptr) {
        this->zero_h00->publish_state((bool)(b_io & 0x08));
      }
      if (this->run_in != nullptr) {
        this->run_out->publish_state((bool)(b_io & 0x10));
      }
      if (this->run_out != nullptr) {
        this->run_in->publish_state((bool)(b_io & 0x20));
      }
      if (this->tzsensor != nullptr) {
        char s[8];
        sprintf(s, "%s%02d:%02d", (tz>=0) ? "+" : "-", abs(tz)/60, abs(tz)%60);
        this->tzsensor->publish_state(s);
      }
    } // csum check
    // reset buffer index
    index = 0;
  } 
  */
}

void SolisS5Component::dump_config() {
  ESP_LOGCONFIG(TAG, "SolisS5Component");
}

} // namespace dekacontroller
} // namespace esphome
