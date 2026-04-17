import esphome.codegen as cg
from esphome.components import climate_ir

AUTO_LOAD = ["climate_ir"]

haierac160 = cg.esphome_ns.namespace("haierac160")
HaierAC160Climate = haierac160.class_("HaierAC160Climate", climate_ir.ClimateIR)
CONFIG_SCHEMA = climate_ir.climate_ir_schema(HaierAC160Climate)

async def to_code(config):
    await climate_ir.new_climate_ir(config)