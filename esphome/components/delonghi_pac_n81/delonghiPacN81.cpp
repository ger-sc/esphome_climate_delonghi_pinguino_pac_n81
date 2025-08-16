#include "delonghiPacN81.h"

namespace esphome {
namespace delonghi_pac_n81 {

static const char *TAG = "delonghi_pac_n81";

climate::ClimateTraits DelonghiPacN81Climate::traits() {
  auto traits = climate::ClimateTraits();
  traits.set_supported_modes({climate::CLIMATE_MODE_OFF, climate::CLIMATE_MODE_COOL, climate::CLIMATE_MODE_FAN_ONLY,
                              climate::CLIMATE_MODE_DRY});
  traits.set_supported_fan_modes({climate::CLIMATE_FAN_LOW, climate::CLIMATE_FAN_MEDIUM, climate::CLIMATE_FAN_HIGH});
  traits.set_visual_min_temperature(delonghi_pac_n81::DELONGHI_TEMP_MIN);
  traits.set_visual_max_temperature(delonghi_pac_n81::DELONGHI_TEMP_MAX);
  traits.set_supports_two_point_target_temperature(false);
  traits.set_visual_temperature_step(1.0f);
  return traits;
}

void DelonghiPacN81Climate::setup() { ESP_LOGCONFIG(TAG, "Setting up Delonghi Pinguino Climate..."); }

void DelonghiPacN81Climate::control(const climate::ClimateCall &call) {
  if (call.get_mode().has_value()) {
    this->mode = *call.get_mode();
    current_mode_ = this->mode;
    if (current_mode_ == climate::CLIMATE_MODE_DRY) {
      mode_ = 0x4000;
      command_ = 0x0088;
      ESP_LOGI(TAG, "Set mode to dry");
    } else if (current_mode_ == climate::CLIMATE_MODE_FAN_ONLY) {
      mode_ = 0x8000;
      command_ = 0x0088;
      ESP_LOGI(TAG, "Set mode to fan only");
    } else if (current_mode_ == climate::CLIMATE_MODE_COOL) {
      mode_ = 0x1000;
      command_ = 0x0088;
      ESP_LOGI(TAG, "Set mode to cool");
    } else if (current_mode_ == climate::CLIMATE_MODE_OFF) {
      command_ = 0x0001;
      ESP_LOGI(TAG, "Set mode to off");
    }
  }

  if (call.get_target_temperature().has_value()) {
    this->target_temperature_high = *call.get_target_temperature();
    target_temperature_ = this->target_temperature_high;
    int t = (int) target_temperature_;
    if (t < DELONGHI_TEMP_MIN || t > DELONGHI_TEMP_MAX) {
      ESP_LOGW(TAG, "Temperature %d out of bounds", t);
    } else {
      temp_ = (t - 16) << 2;
      ESP_LOGI(TAG, "Temperature set to %d", t);
    }
  }

  if (call.get_fan_mode().has_value()) {
    this->fan_mode = *call.get_fan_mode();
    current_fan_mode_ = this->fan_mode;
    if (current_fan_mode_ == climate::CLIMATE_FAN_LOW) {
      fan_ = 0x0200;
      ESP_LOGI(TAG, "Set fan to low");
    } else if (current_fan_mode_ == climate::CLIMATE_FAN_MEDIUM) {
      fan_ = 0x0400;
      ESP_LOGI(TAG, "Set fan to medium");
    } else if (current_fan_mode_ == climate::CLIMATE_FAN_HIGH) {
      fan_ = 0x0800;
      ESP_LOGI(TAG, "Set fan to high");
    }
  }
  send_nec_code(mode_ + fan_ + address_, temp_ + command_);
  this->publish_state();
}

// sendet NEC mit der templatisierten API (einzeiliger, sauberer Aufruf)
void DelonghiPacN81Climate::send_nec_code(uint16_t address, uint16_t command, uint16_t repeats) {
  if (!this->transmitter_) {
    ESP_LOGW(TAG, "Transmitter not defined!");
    return;
  }
  esphome::remote_base::NECData data;
  data.address = address;
  data.command = command;
  data.command_repeats = repeats;

  // Option A: direkte, templatisierte Methode (empfohlen)
  this->transmitter_->transmit<esphome::remote_base::NECProtocol>(data, 1, 0);
}

}  // namespace delonghi_pac_n81
}  // namespace esphome
