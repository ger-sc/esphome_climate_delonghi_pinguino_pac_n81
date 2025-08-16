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

}  // namespace delonghi_pac_n81
}  // namespace esphome
