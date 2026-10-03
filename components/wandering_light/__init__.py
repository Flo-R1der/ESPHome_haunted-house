import esphome.codegen as cg
import esphome.config_validation as cv

CODEOWNERS = ["@Flo-R1der"]

CONFIG_SCHEMA = cv.Schema({})


async def to_code(config):
    cg.add_global(
        cg.RawStatement(
            '#include "esphome/components/wandering_light/wandering_light_effect.h"'
        )
    )
