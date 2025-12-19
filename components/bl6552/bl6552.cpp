#include "bl6552.h"
#include "esphome/core/log.h"
#include <cinttypes>

// Datasheet: https://www.belling.com.cn/media/file_object/bel_product/BL6552/datasheet/BL6552_V1.12_en.pdf

namespace esphome {
namespace bl6552 {

static const char *const TAG = "bl6552";
//3.1
static const uint8_t BL6552_REG_GAIN1 = 0x60;
static const uint8_t BL6552_REG_GAIN2 = 0x61;
//3.1.1
static const uint8_t BL6552_REG_IRMS_P1 = 0x62;
static const uint8_t BL6552_REG_IRMS_P2 = 0x63;
static const uint8_t BL6552_REG_IA_PHCAL = 0x64;
static const uint8_t BL6552_REG_IB_PHCAL = 0x65;
static const uint8_t BL6552_REG_IC_PHCAL = 0x66;
static const uint8_t BL6552_REG_VA_PHCAL = 0x67;
static const uint8_t BL6552_REG_VB_PHCAL = 0x68;
static const uint8_t BL6552_REG_VC_PHCAL = 0x69;
static const uint8_t BL6552_REG_IN_PHCAL = 0x90;
//3.1.2 Channel Offset Correction
static const uint8_t BL6552_REG_IC_CHOS = 0xAC;
static const uint8_t BL6552_REG_IB_CHOS = 0xAD;
static const uint8_t BL6552_REG_IA_CHOS= 0xAE;
static const uint8_t BL6552_REG_IN_CHOS = 0xAF;
static const uint8_t BL6552_REG_VA_CHOS = 0xB2;
static const uint8_t BL6552_REG_VB_CHOS = 0xB3;
static const uint8_t BL6552_REG_VC_CHOS = 0xB4;
//3.1.3 Channel Gain Correction
static const uint8_t BL6552_REG_IC_CHGN = 0xA1;
static const uint8_t BL6552_REG_IB_CHGN = 0xA2;
static const uint8_t BL6552_REG_IA_CHGN= 0xA3;
static const uint8_t BL6552_REG_IN_CHGN = 0xA4;
static const uint8_t BL6552_REG_VA_CHGN = 0xA7;
static const uint8_t BL6552_REG_VB_CHGN = 0xA8;
static const uint8_t BL6552_REG_VC_CHGN = 0xA9;
//3.1.4 Current and Voltage Waveform Output
static const uint8_t BL6552_REG_IC_WAVE = 0x02;
static const uint8_t BL6552_REG_IB_WAVE = 0x03;
static const uint8_t BL6552_REG_IA_WAVE = 0x04;
static const uint8_t BL6552_REG_IN_WAVE = 0x05;
static const uint8_t BL6552_REG_VA_WAVE = 0x08;
static const uint8_t BL6552_REG_VB_WAVE = 0x09;
static const uint8_t BL6552_REG_VC_WAVE = 0x0A;
//3.2.1 Active Power Output
static const uint8_t BL6552_REG_WATT_A = 0x22;
static const uint8_t BL6552_REG_WATT_B = 0x23;
static const uint8_t BL6552_REG_WATT_C = 0x24;
static const uint8_t BL6552_REG_wATT = 0x25;
//3.2.2 Active Power Calibration
static const uint8_t BL6552_REG_WATTGN_A = 0xB6
static const uint8_t BL6552_REG_WATTGN_B = 0xB7;
static const uint8_t BL6552_REG_WATTGN_C = 0xB8;
static const uint8_t BL6552_REG_WATTOS_A = 0xC2;
static const uint8_t BL6552_REG_WATTOS_B = 0xC3;
static const uint8_t BL6552_REG_WATTOS_C = 0xC4;
//3.2.3 Active Power Anti-Creeping
static const uint8_t BL6552_REG_CREEP = 0x88;
static const uint8_t BL6552_REG_CREEP2 = 0x89;
//3.2.4 Active Power Small Signal Compensation
static const uint8_t BL6552_REG_WA_LOS_A = 0x82
static const uint8_t BL6552_REG_WA_LOS_B = 0x83;
static const uint8_t BL6552_REG_WA_LOS_C = 0x84;
//3.2.5 Active Power Selection
//3.3.1 Active Energy Output
static const uint8_t BL6552_REG_CF_A_CNT = 0x2F;
static const uint8_t BL6552_REG_CF_B_CNT = 0x30;
static const uint8_t BL6552_REG_CF_C_CNT = 0x31;
static const uint8_t BL6552_REG_CF_CNT = 0x32;
static const uint8_t BL6552_REG_CFP_A_CNT = 0x33;
static const uint8_t BL6552_REG_CFP_B_CNT = 0x34;
static const uint8_t BL6552_REG_CFP_C_CNT = 0x35;
static const uint8_t BL6552_REG_CFP_CNT = 0x36;
static const uint8_t BL6552_REG_CFN_A_CNT = 0x37;
static const uint8_t BL6552_REG_CFN_B_CNT = 0x38;
static const uint8_t BL6552_REG_CFN_C_CNT = 0x39;
static const uint8_t BL6552_REG_CFN_CNT = 0x3A;
//3.3.3 Active Energy Pulse Output Ratio
static const uint8_t BL6552_REG_CFDIV = 0xCE;
//3.4.1 Effective Value Output
static const uint8_t BL6552_REG_IC_RMS = 0x0D;
static const uint8_t BL6552_REG_IB_RMS = 0x0E;
static const uint8_t BL6552_REG_IA_RMS = 0x0F;
static const uint8_t BL6552_REG_IN_RMS = 0x10;
static const uint8_t BL6552_REG_VA_RMS = 0x13;
static const uint8_t BL6552_REG_VB_RMS = 0x14;
static const uint8_t BL6552_REG_VC_RMS = 0x15;
//3.4.4 Current and Voltage RMS Calibration
static const uint8_t BL6552_REG_IC_RMSGN = 0x6D;
static const uint8_t BL6552_REG_IB_RMSGN = 0x6E;
static const uint8_t BL6552_REG_IA_RMSGN = 0x6F;
static const uint8_t BL6552_REG_IN_RMSGN = 0x70;
static const uint8_t BL6552_REG_VA_RMSGN = 0x73;
static const uint8_t BL6552_REG_VB_RMSGN = 0x74;
static const uint8_t BL6552_REG_VC_RMSGN = 0x75;
static const uint8_t BL6552_REG_IC_RMSOS = 0x78;
static const uint8_t BL6552_REG_IB_RMSOS = 0x79;
static const uint8_t BL6552_REG_IA_RMSOS = 0x7A;
static const uint8_t BL6552_REG_IN_RMSOS = 0x7B;
static const uint8_t BL6552_REG_VA_RMSOS = 0x7E;
static const uint8_t BL6552_REG_VB_RMSOS = 0x7F;
static const uint8_t BL6552_REG_VC_RMSOS = 0x80;
//3.4.5 Effective Value of Anti-Creeping
static const uint8_t BL6552_REG_REVPRMS_CREEP = 0x8A;
//3.5.1 Fast RMS Output
static const uint8_t BL6552_REG_IC_FAST_RMS = 018;
static const uint8_t BL6552_REG_IB_FAST_RMS = 0x19;
static const uint8_t BL6552_REG_IA_FAST_RMS = 0x1A;
static const uint8_t BL6552_REG_IN_FAST_RMS = 0x1B;
static const uint8_t BL6552_REG_VA_FAST_RMS = 0x1E;
static const uint8_t BL6552_REG_VB_FAST_RMS = 0x1F;
static const uint8_t BL6552_REG_VC_FAST_RMS = 0x20;
//3.5.3 Fast RMS Cumulative Time
static const uint8_t BL6552_REG_VC_FAST_CTRL = 0x8B;
//3.6.1 Reactive Phase Compensation
static const uint8_t BL6552_REG_VAR_PHCAL_I = 0x6A;
static const uint8_t BL6552_REG_VAR_PHCAL_V = 0x6B;
//3.6.2 Reactive Power Output
static const uint8_t BL6552_REG_VAR_A = 0x5A;
static const uint8_t BL6552_REG_VAR_B = 0x5B;
static const uint8_t BL6552_REG_VAR_C = 0x5C;
static const uint8_t BL6552_REG_VAR = 0x5D;
static const uint8_t BL6552_REG_FVAR_A = 0x2A;
static const uint8_t BL6552_REG_FVAR_B = 0x2B;
static const uint8_t BL6552_REG_FVAR_C = 0x2C;
static const uint8_t BL6552_REG_FVAR = 0x2D;
//3.6.3 Reactive Power Calibration
static const uint8_t BL6552_REG_VARGN_A = 0xB9;
static const uint8_t BL6552_REG_VARGN_B = 0xBA;
static const uint8_t BL6552_REG_VARGN_C = 0xBB;
static const uint8_t BL6552_REG_VAROS_A = 0xC5;
static const uint8_t BL6552_REG_VAROS_B = 0xC6;
static const uint8_t BL6552_REG_VAROS_C = 0xC7;
static const uint8_t BL6552_REG_FVARGN_A = 0xBC;
static const uint8_t BL6552_REG_FVARGN_B = 0xBD;
static const uint8_t BL6552_REG_FVARGN_C = 0xBE;
static const uint8_t BL6552_REG_FVAROS_A = 0xC8;
static const uint8_t BL6552_REG_FVAROS_B = 0xC9;
static const uint8_t BL6552_REG_FVAROS_C = 0xCA;
//3.6.4 Anti-creeping of reactive power
static const uint8_t BL6552_REG_VARWA_CREEP = 0x88;
//3.6.5 Reactive power small signal compensation
static const uint8_t BL6552_REG_VAR_LOS_A = 0x82;
static const uint8_t BL6552_REG_VAR_LOS_B = 0x83;
static const uint8_t BL6552_REG_VAR_LOS_C = 0x84;
static const uint8_t BL6552_REG_FVAR_LOS_A = 0x85;
static const uint8_t BL6552_REG_FVAR_LOS_B = 0x86;
static const uint8_t BL6552_REG_FVAR_LOS_C = 0x87;
//3.6.6 Reactive energy output
static const uint8_t BL6552_REG_CFQ_A_CNT = 0x3B;
static const uint8_t BL6552_REG_CFQ_B_CNT = 0x3C;
static const uint8_t BL6552_REG_CFQ_C_CNT = 0x3D;
static const uint8_t BL6552_REG_CFQ_CNT = 0x3E;
static const uint8_t BL6552_REG_CFQ1_CNT = 0x3F;
static const uint8_t BL6552_REG_CFQ2_CNT = 0x40;
static const uint8_t BL6552_REG_CFQ3_CNT = 0x41;
static const uint8_t BL6552_REG_CFQ4_CNT = 0x42;
//3.7.1 Apparent power and energy output
static const uint8_t BL6552_REG_VA_A = 0x26;
static const uint8_t BL6552_REG_VA_B = 0x27;
static const uint8_t BL6552_REG_VA_C = 0x28;
static const uint8_t BL6552_REG_VA = 0x29;
static const uint8_t BL6552_REG_CFS_A_CNT = 0x43;
static const uint8_t BL6552_REG_CFS_B_CNT = 0x44;
static const uint8_t BL6552_REG_CFS_C_CNT = 0x45;
static const uint8_t BL6552_REG_CFS_CNT = 0x46;
//3.7.2 Apparent power calculation
static const uint8_t BL6552_REG_VAGN_A = 0xBF;
static const uint8_t BL6552_REG_VAGN_B = 0xC0;
static const uint8_t BL6552_REG_VAGN_C = 0xC1;
static const uint8_t BL6552_REG_VAOS_A = 0xCB;
static const uint8_t BL6552_REG_VAOS_B = 0xCC;
static const uint8_t BL6552_REG_VAOS_C = 0xCD;
//3.7.3 Power factor
static const uint8_t BL6552_REG_PF_A = 0x47;
static const uint8_t BL6552_REG_PF_B = 0x48;
static const uint8_t BL6552_REG_PF_C = 0x49;
static const uint8_t BL6552_REG_PF = 0x4A;
//3.8.1 The output of the current sum
static const uint8_t BL6552_REG_I_SUM = 0x57;
static const uint8_t BL6552_REG_I_SUM_RMS = 0x58;
static const uint8_t BL6552_REG_I_SUM_FAST_RMS = 0x59;
//3.8.2 Adjustment of current sum
static const uint8_t BL6552_REG_ISUM_RMSGN = 0x91;
static const uint8_t BL6552_REG_ISUM_RMSOS = 0x92;
//3.8.3 Comparison of current sum
static const uint8_t BL6552_REG_ISUMLVL = 0x8D;
//3.9 Small signal compensation
static const uint8_t BL6552_REG_WAVAR_LOS_A = 0x82;
static const uint8_t BL6552_REG_WAVAR_LOS_B = 0x83;
static const uint8_t BL6552_REG_WAVAR_LOS_C = 0x84;
static const uint8_t BL6552_REG_FVAR_LOS_A = 0x85;
static const uint8_t BL6552_REG_FVAR_LOS_B = 0x86;
static const uint8_t BL6552_REG_FVAR_LOS_C = 0x87;
//3.10.1 Line Cycle measurement
static const uint8_t BL6552_REG_LINE_WATTHR = 0x4B;
static const uint8_t BL6552_REG_LINE_VARHR = 0x4C;
static const uint8_t BL6552_REG_SAGLVL_LINECYC = 0x8F;
//3.10.2 Line frequency measurement
static const uint8_t BL6552_REG_PERIOD = 0x2E;
//3.10.3 Phase angle calculation
static const uint8_t BL6552_REG_ANGLE_AB = 0x4E;
static const uint8_t BL6552_REG_ANGLE_BC = 0x4F;
static const uint8_t BL6552_REG_ANGLE_AC = 0x50;
static const uint8_t BL6552_REG_ANGLE_A = 0x51;
static const uint8_t BL6552_REG_ANGLE_B = 0x52;
static const uint8_t BL6552_REG_ANGLE_C = 0x53;
//3.10.4 Power sign bit
static const uint8_t BL6552_REG_SIGN = 0x4D;
//3.11.2 peak detection
static const uint8_t BL6552_REG_IV_PKLVL = 0x8C;
//3.11.3 SAG detection
static const uint8_t BL6552_REG_SAGCYC_ZXTOUT = 0x8E;
//static const uint8_t BL6552_REG_SAGLVL_LINECYC = 0x8F;
// OTHER
static const uint8_t BL6552_REG_MODE1 = 0x96;
static const uint8_t BL6552_REG_MODE2 = 0x97;
static const uint8_t BL6552_REG_MODE3 = 0x98;
static const uint8_t BL6552_REG_MASK1 = 0x9A;
static const uint8_t BL6552_REG_MASK2 = 0x9B;
static const uint8_t BL6552_REG_RST_ENG = 0x9D;
static const uint8_t BL6552_REG_USR_WPPROT = 0x9E;
static const uint8_t BL6552_REG_SOFT_RESET = 0x9F;

static const uint8_t BL6552_READ_COMMAND = 0x35;
static const uint8_t BL6552_WRITE_COMMAND = 0xCA;

static const uint8_t BL6552_FULL_PACKET = 0xAA;
static const uint8_t BL6552_PACKET_HEADER = 0x55;

static const uint8_t BL6552_REG_I_RMSOS = 0x12;
static const uint8_t BL6552_REG_WA_CREEP = 0x14;
static const uint8_t BL6552_REG_I_FAST_RMS_TH = 0x15;
static const uint8_t BL6552_REG_I_FAST_RMS_CYC = 0x16;
static const uint8_t BL6552_REG_FREQ_CYC = 0x17;
static const uint8_t BL6552_REG_OT_FUNX = 0x18;
static const uint8_t BL6552_REG_MODE = 0x19;
static const uint8_t BL6552_REG_SOFT_RESET = 0x1C;
static const uint8_t BL6552_REG_USR_WRPROT = 0x1D;
static const uint8_t BL6552_REG_TPS_CTRL = 0x1B;

static const uint32_t BL6552_REG_MODE_RESV = 0x03;
static const uint32_t BL6552_REG_MODE_CF_EN = 0x04;
static const uint32_t BL6552_REG_MODE_RMS_UPDATE_SEL = 0x08;
static const uint32_t BL6552_REG_MODE_FAST_RMS_SEL = 0x10;
static const uint32_t BL6552_REG_MODE_AC_FREQ_SEL = 0x20;
static const uint32_t BL6552_REG_MODE_CF_CNT_CLR_SEL = 0x40;
static const uint32_t BL6552_REG_MODE_CF_CNT_ADD_SEL = 0x80;
static const uint32_t BL6552_REG_MODE_UART_RATE_19200 = 0x200;
static const uint32_t BL6552_REG_MODE_UART_RATE_38400 = 0x300;
static const uint32_t BL6552_REG_MODE_DEFAULT =
    BL6552_REG_MODE_RESV | BL6552_REG_MODE_CF_EN | BL6552_REG_MODE_CF_CNT_ADD_SEL;

static const uint32_t BL6552_REG_SOFT_RESET_MAGIC = 0x5a5a5a;
static const uint32_t BL6552_REG_USR_WRPROT_MAGIC = 0x55;

// 23-byte packet, 11 bits per byte, 2400 baud: about 105ms
static const uint32_t PKT_TIMEOUT_MS = 200;

void BL6552::loop() {
  DataPacket buffer;
  int avail = this->available();

  if (!avail) {
    return;
  }
  if (avail < sizeof(buffer)) {
    if (!this->rx_start_) {
      this->rx_start_ = millis();
    } else if (millis() > this->rx_start_ + PKT_TIMEOUT_MS) {
      ESP_LOGW(TAG, "Junk on wire. Throwing away partial message (%d bytes)", avail);
      this->read_array((uint8_t *) &buffer, avail);
      this->rx_start_ = 0;
    }
    return;
  }

  if (this->read_array((uint8_t *) &buffer, sizeof(buffer))) {
    if (this->validate_checksum_(&buffer)) {
      this->received_package_(&buffer);
    }
  }
  this->rx_start_ = 0;
}

bool BL6552::validate_checksum_(DataPacket *data) {
  uint8_t checksum = BL6552_READ_COMMAND | this->address_;
  // Whole package but checksum
  uint8_t *raw = (uint8_t *) data;
  for (uint32_t i = 0; i < sizeof(*data) - 1; i++) {
    checksum += raw[i];
  }
  checksum ^= 0xFF;
  if (checksum != data->checksum) {
    ESP_LOGW(TAG, "BL6552 invalid checksum! 0x%02X != 0x%02X", checksum, data->checksum);
  }
  return checksum == data->checksum;
}

void BL6552::write_reg_(uint8_t reg, uint32_t val) {
  uint8_t pkt[6];

  this->flush();
  pkt[0] = BL6552_WRITE_COMMAND | this->address_;
  pkt[1] = reg;
  pkt[2] = (val & 0xff);
  pkt[3] = (val >> 8) & 0xff;
  pkt[4] = (val >> 16) & 0xff;
  pkt[5] = (pkt[0] + pkt[1] + pkt[2] + pkt[3] + pkt[4]) ^ 0xff;
  this->write_array(pkt, 6);
  delay(1);
}

int BL6552::read_reg_(uint8_t reg) {
  union {
    uint8_t b[4];
    uint32_le_t le32;
  } resp;

  this->write_byte(BL6552_READ_COMMAND | this->address_);
  this->write_byte(reg);
  this->flush();
  if (this->read_array(resp.b, 4) &&
      resp.b[3] ==
          (uint8_t) ((BL6552_READ_COMMAND + this->address_ + reg + resp.b[0] + resp.b[1] + resp.b[2]) ^ 0xff)) {
    resp.b[3] = 0;
    return resp.le32;
  }
  return -1;
}

void BL6552::update() {
  this->write_byte(BL6552_READ_COMMAND | this->address_);
  this->write_byte(BL6552_FULL_PACKET);
}

void BL6552::setup() {
  // If either current or voltage references are set explicitly by the user,
  // calculate the power reference from it unless that is also explicitly set.
  if ((this->current_reference_set_ || this->voltage_reference_set_) && !this->power_reference_set_) {
    this->power_reference_ = (this->voltage_reference_ * this->current_reference_ * 3537.0 / 305978.0) / 73989.0;
    this->power_reference_set_ = true;
  }

  // Similarly for energy reference, if the power reference was set by the user
  // either implicitly or explicitly.
  if (this->power_reference_set_ && !this->energy_reference_set_) {
    this->energy_reference_ = this->power_reference_ * 3600000 / 419430.4;
    this->energy_reference_set_ = true;
  }

  this->write_reg_(BL6552_REG_USR_WRPROT, BL6552_REG_USR_WRPROT_MAGIC);
  if (this->reset_)
    this->write_reg_(BL6552_REG_SOFT_RESET, BL6552_REG_SOFT_RESET_MAGIC);

  uint32_t mode = BL6552_REG_MODE_DEFAULT;
  mode |= BL6552_REG_MODE_RMS_UPDATE_SEL; /* 800ms refresh time */
  if (this->line_freq_ == LINE_FREQUENCY_60HZ)
    mode |= BL6552_REG_MODE_AC_FREQ_SEL;
  this->write_reg_(BL6552_REG_MODE, mode);

  this->write_reg_(BL6552_REG_USR_WRPROT, 0);

  if (this->read_reg_(BL6552_REG_MODE) != mode)
    this->status_set_warning("BL6552 setup failed!");

  this->flush();
}

void BL6552::received_package_(DataPacket *data) {
  // Bad header
  if (data->frame_header != BL6552_PACKET_HEADER) {
    ESP_LOGI(TAG, "Invalid data. Header mismatch: %d", data->frame_header);
    return;
  }

  // cf_cnt is only 24 bits, so track overflows
  uint32_t cf_cnt = (uint24_t) data->cf_cnt;
  cf_cnt |= this->prev_cf_cnt_ & 0xff000000;
  if (cf_cnt < this->prev_cf_cnt_) {
    cf_cnt += 0x1000000;
  }
  this->prev_cf_cnt_ = cf_cnt;

  float v_rms = (uint24_t) data->v_rms / voltage_reference_;
  float i_rms = (uint24_t) data->i_rms / current_reference_;
  float watt = (int24_t) data->watt / power_reference_;
  float total_energy_consumption = cf_cnt / energy_reference_;
  float frequency = 1000000.0f / data->frequency;

  if (voltage_sensor_ != nullptr) {
    voltage_sensor_->publish_state(v_rms);
  }
  if (current_sensor_ != nullptr) {
    current_sensor_->publish_state(i_rms);
  }
  if (power_sensor_ != nullptr) {
    power_sensor_->publish_state(watt);
  }
  if (energy_sensor_ != nullptr) {
    energy_sensor_->publish_state(total_energy_consumption);
  }
  if (frequency_sensor_ != nullptr) {
    frequency_sensor_->publish_state(frequency);
  }
  this->status_clear_warning();
  ESP_LOGV(TAG, "BL6552: U %fV, I %fA, P %fW, Cnt %" PRId32 ", ∫P %fkWh, frequency %fHz, status 0x%08X", v_rms, i_rms,
           watt, cf_cnt, total_energy_consumption, frequency, data->status);
}

void BL6552::dump_config() {  // NOLINT(readability-function-cognitive-complexity)
  ESP_LOGCONFIG(TAG, "BL6552:");
  ESP_LOGCONFIG(TAG, "  Reset: %s", TRUEFALSE(this->reset_));
  ESP_LOGCONFIG(TAG, "  Address: %d", this->address_);
  ESP_LOGCONFIG(TAG, "  Nominal line frequency: %d Hz", this->line_freq_);
  ESP_LOGCONFIG(TAG, "  Current reference: %f", this->current_reference_);
  ESP_LOGCONFIG(TAG, "  Energy reference: %f", this->energy_reference_);
  ESP_LOGCONFIG(TAG, "  Power reference: %f", this->power_reference_);
  ESP_LOGCONFIG(TAG, "  Voltage reference: %f", this->voltage_reference_);
  LOG_SENSOR("", "Voltage", this->voltage_sensor_);
  LOG_SENSOR("", "Current", this->current_sensor_);
  LOG_SENSOR("", "Power", this->power_sensor_);
  LOG_SENSOR("", "Energy", this->energy_sensor_);
  LOG_SENSOR("", "Frequency", this->frequency_sensor_);
}

}  // namespace BL6552
}  // namespace esphome
