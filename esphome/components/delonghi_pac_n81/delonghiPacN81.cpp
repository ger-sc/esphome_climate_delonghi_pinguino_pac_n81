#include "delonghiPacN81.h"

namespace esphome {
namespace delonghi_pac_n81 {

class PinguinoNecClimate : public climate::Climate, public Component {
 public:
  // wird im YAML zugewiesen: my_ac->transmitter_ = &id(ir_tx);
  remote_transmitter::RemoteTransmitterComponent *transmitter_{nullptr};

  uint16_t mode;
  uint16_t fan;
  const uint16_t address = 0x48;
  uint16_t temp;
  uint16_t command;

  climate::ClimateTraits traits() override {
    auto traits = climate::ClimateTraits();
    traits.set_supported_modes({climate::CLIMATE_MODE_OFF, climate::CLIMATE_MODE_COOL, climate::CLIMATE_MODE_FAN_ONLY,
                                climate::CLIMATE_MODE_DRY});
    traits.set_supported_fan_modes({climate::CLIMATE_FAN_LOW, climate::CLIMATE_FAN_MEDIUM, climate::CLIMATE_FAN_HIGH});
    traits.set_visual_min_temperature(delonghi_pac_n81::DELONGHI_TEMP_MIN);
    traits.set_visual_max_temperature(delonghi_pac_n81::DELONGHI_TEMP_MAX);
    traits.set_visual_temperature_step(1.0f);
    return traits;
  }

  void control(const climate::ClimateCall &call) override {
    if (call.get_mode().has_value()) {
      this->mode = *call.get_mode();
      if (this->mode == climate::CLIMATE_MODE_DRY) {
        mode = 0x4000;
        command = 0x0088;
      } else if (this->mode == climate::CLIMATE_MODE_FAN_ONLY) {
        mode = 0x8000;
        command = 0x0088;
      } else if (this->mode == climate::CLIMATE_MODE_COOL) {
        mode = 0x1000;
        command = 0x0088;
      } else if (this->mode == climate::CLIMATE_MODE_OFF) {
        command = 0x0001;
      }
    }

    if (call.get_target_temperature().has_value()) {
      this->target_temperature = *call.get_target_temperature();
      int t = (int) this->target_temperature;
      send_temp_code(t);
    }

    if (call.get_fan_mode().has_value()) {
      this->fan_mode = *call.get_fan_mode();
      if (this->fan_mode == climate::CLIMATE_FAN_LOW) {
        fan = 0x0200;
      } else if (this->fan_mode == climate::CLIMATE_FAN_MEDIUM) {
        fan = 0x0400;
      } else if (this->fan_mode == climate::CLIMATE_FAN_HIGH) {
        fan = 0x0800;
      }
    }
    send_nec_code(mode + fan + address, temp + command);
    this->publish_state();
  }

 protected:
  // sendet NEC mit der templatisierten API (einzeiliger, sauberer Aufruf)
  void send_nec_code(uint16_t address, uint16_t command, uint16_t repeats = 1) {
    if (!this->transmitter_) {
      ESP_LOGW("pinguino", "Transmitter nicht gesetzt!");
      return;
    }
    esphome::remote_base::NECData data;
    data.address = address;
    data.command = command;
    data.command_repeats = repeats;

    // Option A: direkte, templatisierte Methode (empfohlen)
    this->transmitter_->transmit<esphome::remote_base::NECProtocol>(data, 1, 0);
  }

  void send_temp_code(int inputTemp) {
    switch (inputTemp) {
      case 16:
        temp = 0x0000;
        break;
      case 17:
        temp = 0x0100;
        break;
      case 18:
        temp = 0x0200;
        break;  // <-- ersetze
      case 19:
        temp = 0x0300;
        break;
      case 20:
        temp = 0x0400;
        break;
      case 21:
        temp = 0x0500;
        break;
      case 22:
        temp = 0x0600;
        break;
      case 23:
        temp = 0x0700;
        break;
      case 24:
        temp = 0x0800;
        break;
      case 25:
        temp = 0x0900;
        break;
      case 26:
        temp = 0x0A00;
        break;
      case 27:
        temp = 0x0B00;
        break;
      case 28:
        temp = 0x0C00;
        break;
      case 29:
        temp = 0x0D00;
        break;
      case 30:
        temp = 0x0E00;
        break;
      case 31:
        temp = 0x0F00;
        break;
      case 32:
        temp = 0x1000;
        break;
      default:
        ESP_LOGW("pinguino", "Temperatur %d nicht gemappt", inputTemp);
    }
    send_nec_code(mode + fan + address, temp + command);
  }
};

}  // namespace delonghi_pac_n81
}  // namespace esphome
