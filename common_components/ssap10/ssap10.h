// 头文件保护，防止重复包含
#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"

#define BUFFER_SIZE (32 * 6)

// ESPHome命名空间
namespace esphome {
// SSAP10传感器命名空间
namespace ssap10 {

/**
 * SSAP10Sensor类 - Makerfabs PM2.5传感器组件
 * 
 * 这个类实现了对SSAP10 PM2.5传感器的支持，该传感器通过UART接口
 * 使用特定的协议发送PM2.5浓度数据。
 * 
 * 继承关系：
 * - sensor::Sensor: 提供传感器基础功能，可以发布数据到Home Assistant
 * - PollingComponent: 提供定时轮询功能，会定期调用update()方法
 * - uart::UARTDevice: 提供UART通信能力，处理串口读写操作
 */
class SSAP10Sensor : public sensor::Sensor, public PollingComponent, public uart::UARTDevice {
 public:
  /**
   * 构造函数
   * 初始化组件，设置默认轮询间隔
   */
  SSAP10Sensor();

  /**
   * 组件初始化方法
   * 在ESPHome启动时被调用，用于初始化硬件和设置初始状态
   * 重写自Component::setup()
   */
  void setup() override;

  /**
   * 更新方法
   * 由PollingComponent定期调用，负责从传感器读取数据
   * 这是主要的数据采集逻辑所在
   * 重写自PollingComponent::update()
   */
  void update() override;

  /**
   * 配置转储方法
   * 用于在日志中输出组件的配置信息，便于调试
   * 重写自Component::dump_config()
   */
  void dump_config() override;

  /**
   * 获取设置优先级
   * 返回组件的初始化优先级，决定在启动过程中的初始化顺序
   * 重写自Component::get_setup_priority()
   * @return float 优先级值，数值越小优先级越高
   */
  float get_setup_priority() const override;

 protected:
   // 传感器状态标志
  bool sensor_ready_ = false;           // 传感器是否完成30秒稳定期
  
  // 连接测试方法
  uint8_t data_buffer_[BUFFER_SIZE];
  int buffer_index_ = 0;

  void read_to_buffer();
  bool find_valid_frame(uint8_t* frame_data);
  bool validate_checksum(const uint8_t* data);
  void process_buffer();
  // 注意：这里不需要添加额外的成员变量或方法
  // UARTDevice基类已经提供了所有必要的UART操作方法：
  // - set_uart_parent(): 设置UART总线父级
  // - available(): 检查是否有数据可读
  // - read(): 读取单个字节
  // - read_array(): 读取字节数组
  // - write(): 写入数据等
};

}  // namespace ssap10
}  // namespace esphome