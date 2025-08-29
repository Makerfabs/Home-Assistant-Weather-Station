"""ssap10/__init__.py

这个文件定义了SSAP10组件，但不直接实现传感器平台
"""

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID

# 声明组件依赖
DEPENDENCIES = ["uart"]
CODEOWNERS = ["@xiaren"]

# 定义C++命名空间和基础组件类
ssap10_ns = cg.esphome_ns.namespace("ssap10")
SSAP10Component = ssap10_ns.class_(
    "SSAP10Component", 
    cg.PollingComponent, 
    cg.Component
)

# 基础组件配置（不包含传感器配置）
CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(SSAP10Component),
}).extend(cv.polling_component_schema("15s"))

async def to_code(config):
    
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)