#pragma once

#include "esphome/core/component.h"
#include "esphome/components/climate/climate.h"
#include "esphome/components/remote_transmitter/remote_transmitter.h"
#include "esphome/components/remote_base/nec_protocol.h"

namespace esphome {
namespace delonghi_pac_n81 {

// Temperature
const uint8_t DELONGHI_TEMP_MIN = 13;  // Celsius
const uint8_t DELONGHI_TEMP_MAX = 32;  // Celsius

class DelonghiPacN81Climate : public climate::Climate, public Component {
 public:
  void setup() override;
  climate::ClimateTraits traits() override;
  void control(const climate::ClimateCall &call) override;

  void set_transmitter(remote_transmitter::RemoteTransmitterComponent *tx) { this->transmitter_ = tx; }

 protected:
  void send_nec_code(uint16_t address, uint16_t command, uint16_t repeats = 1);
  void set_temp_code(int temp);
  remote_transmitter::RemoteTransmitterComponent *transmitter_{nullptr};
  climate::ClimateMode current_mode_ = climate::CLIMATE_MODE_OFF;
  esphome::optional<esphome::climate::ClimateFanMode> current_fan_mode_ = climate::CLIMATE_FAN_LOW;
  float target_temperature_ = 24.0;
  uint16_t mode_;
  uint16_t fan_;
  const uint16_t address_ = 0x48;
  uint16_t temp_;
  uint16_t command_;
};

}  // namespace delonghi_pac_n81
}  // namespace esphome
