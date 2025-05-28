import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart, sensor
from esphome.const import CONF_ID

DEPENDENCIES = ["uart"]
AUTO_LOAD = ["sensor"]

pylontech_ns = cg.esphome_ns.namespace("pylontech_sensor")
PylontechSensorClass = pylontech_ns.class_("PylontechSensor",
                                           cg.Component, sensor.Sensor)

CONFIG_SCHEMA = sensor.sensor_schema().extend(
    {
        cv.GenerateID(): cv.declare_id(PylontechSensorClass),
        cv.Required("uart_id"): cv.use_id(uart.UARTComponent),
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)

    uart_comp = await cg.get_variable(config["uart_id"])
    cg.add(var.set_uart(uart_comp))
