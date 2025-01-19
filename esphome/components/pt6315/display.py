from esphome.const import CONF_ID, CONF_LAMBDA
from esphome import pins
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import display

pt6315_ns = cg.esphome_ns.namespace("pt6315")
PT6315Display = pt6315_ns.class_("PT6315Display", cg.PollingComponent)
PT6315DisplayRef = PT6315Display.operator("ref")

CONF_DIN_PIN = "din_pin"
CONF_CLK_PIN = "clk_pin"
CONF_STB_PIN = "stb_pin"
CONFIG_SCHEMA = display.BASIC_DISPLAY_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(PT6315Display),
    cv.Required(CONF_DIN_PIN): pins.gpio_output_pin_schema,
    cv.Required(CONF_CLK_PIN): pins.gpio_output_pin_schema,
    cv.Required(CONF_STB_PIN): pins.gpio_output_pin_schema,
}).extend(cv.polling_component_schema("1s"))


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await display.register_display(var, config)

    pin = await cg.gpio_pin_expression(config[CONF_DIN_PIN])
    cg.add(var.set_din_pin(pin))

    pin = await cg.gpio_pin_expression(config[CONF_CLK_PIN])
    cg.add(var.set_clk_pin(pin))

    pin = await cg.gpio_pin_expression(config[CONF_STB_PIN])
    cg.add(var.set_stb_pin(pin))

    if CONF_LAMBDA in config:
        lambda_ = await cg.process_lambda(
            config[CONF_LAMBDA], [(PT6315DisplayRef, "it")], return_type=cg.void
        )
        cg.add(var.set_writer(lambda_))
