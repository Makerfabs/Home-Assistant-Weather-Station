"""ssap10/sensor.py

这个文件定义了ssap10的传感器平台实现
"""

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, uart
from esphome.const import (
    CONF_ID,
    CONF_UART_ID,
    DEVICE_CLASS_PM25,
    STATE_CLASS_MEASUREMENT,
    UNIT_MICROGRAMS_PER_CUBIC_METER,
)

# 从主模块导入命名空间和类定义
from . import ssap10_ns

# 定义传感器组件类
SSAP10Sensor = ssap10_ns.class_(
    "SSAP10Sensor",
    sensor.Sensor,
    cg.PollingComponent,
    uart.UARTDevice
)

# 传感器平台配置架构
CONFIG_SCHEMA = (
    sensor.sensor_schema(
        SSAP10Sensor,
        unit_of_measurement=UNIT_MICROGRAMS_PER_CUBIC_METER,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_PM25,
        state_class=STATE_CLASS_MEASUREMENT,
    )
    .extend(cv.polling_component_schema("15s"))
    .extend(uart.UART_DEVICE_SCHEMA)
)

async def to_code(config):
    """生成传感器平台代码"""
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)
    await uart.register_uart_device(var, config)