import esphome.codegen as cg
from esphome.components import climate, remote_transmitter
import esphome.config_validation as cv
from esphome.const import CONF_ID

AUTO_LOAD = ["remote_transmitter"]

delonghi_pac_n81 = cg.esphome_ns.namespace("delonghi_pac_n81")
DelonghiPacN81Climate = delonghi_pac_n81.class_(
    "DelonghiPacN81Climate", climate.Climate, cg.Component
)

DELONGHI_PAC_N81_SCHEMA = cv.Schema(
    {
        cv.Required("transmitter"): cv.use_id(
            remote_transmitter.RemoteTransmitterComponent
        ),
    }
)

CONFIG_SCHEMA = climate.climate_schema(DelonghiPacN81Climate).extend(
    DELONGHI_PAC_N81_SCHEMA
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await climate.register_climate(var, config)
    tx = await cg.get_variable(config["transmitter"])
    cg.add(var.set_transmitter(tx))
