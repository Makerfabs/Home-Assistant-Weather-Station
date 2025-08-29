// ssap10_espidf.cpp
// ESP-IDF implementation for the SSAP10 sensor

#include "ssap10.h"
#include "esphome/core/log.h"
// #include "esp_log.h"
// #include "driver/uart.h"
// #include "driver/gpio.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"

static const char *TAG = "SSAP10";

#define SSAP10_FRAME_LENGTH 32


namespace esphome {
namespace ssap10 {

SSAP10Sensor::SSAP10Sensor() : PollingComponent(15000) {
    this->buffer_index_ = 0;
    memset(this->data_buffer_, 0, BUFFER_SIZE);
}

void SSAP10Sensor::setup() 
{
    ESP_LOGCONFIG(TAG, "Setting up SSAP10 sensor...");

    this->sensor_ready_ = false;
    this->buffer_index_ = 0;

    this->set_timeout(30000, [this]() {
        ESP_LOGCONFIG(TAG, "SSAP10 sensor 30-second stabilization completed");
        this->sensor_ready_ = true;
    });
}

void SSAP10Sensor::update()
{
    // 传感器稳定判断
    if(!this->sensor_ready_) {
        ESP_LOGW(TAG, "Sensor not ready yet, still in 30-second stabilization period");
        return;        
    }

    this->read_to_buffer();
    this->process_buffer();
#if 0
    while (this->available()) {
        this->read();
    }
    delay(1000);

    uint8_t data[SSAP10_FRAME_LENGTH];
    int length = 0;

    while (this->available() < SSAP10_FRAME_LENGTH) {
        ESP_LOGD(TAG, "Waiting for data... have %d bytes, need %d", 
                 this->available(), SSAP10_FRAME_LENGTH);
        delay(100);
    }

    if (!this->read_array(data, SSAP10_FRAME_LENGTH)) {
        ESP_LOGE(TAG, "Failed to read complete data frame");
        this->publish_state(NAN);
        return;
    }
    if (data[0] != 0x42 || data[1] != 0x4d) {
        ESP_LOGW(TAG, "Invalid frame header: 0x%02X 0x%02X", data[0], data[1]);
        this->publish_state(NAN);
        return;
    }
    uint16_t checksum_calc = 0;
    for (int i = 0; i < SSAP10_FRAME_LENGTH - 2; i++) {
        checksum_calc += data[i];
    }

      uint16_t checksum_recv = ((uint16_t)data[SSAP10_FRAME_LENGTH - 2] << 8) | 
                             data[SSAP10_FRAME_LENGTH - 1];
    
    if (checksum_calc != checksum_recv) {
        ESP_LOGW(TAG, "Checksum mismatch: calculated 0x%04X, received 0x%04X", 
                 checksum_calc, checksum_recv);
    }
    
    int pm25_value = (data[6] << 8) | data[7];
    ESP_LOGI(TAG, "PM2.5 Concentration: %d μg/m³", pm25_value);
    this->publish_state(pm25_value);
#endif    
#if 0
    // Read data from UART
    length = uart_read_bytes(UART_PORT_NUM, data, SSAP10_FRAME_LENGTH, pdMS_TO_TICKS(100));
    if (length < SSAP10_FRAME_LENGTH) {
        ESP_LOGW(TAG, "Incomplete data frame received len = %d", length);
        return;
    }

    // Validate frame header
    if (data[0] != 0x42 || data[1] != 0x4d) {
        ESP_LOGW(TAG, "Invalid frame header: %02X %02X", data[0], data[1]);
        return;
    }

    // Validate checksum
    uint8_t received_checksum = data[SSAP10_FRAME_LENGTH - 1];
    uint8_t calculated_checksum = 0;
    for (int i = 0; i < SSAP10_FRAME_LENGTH - 1; i++) {
        calculated_checksum += data[i];
    }

    if (received_checksum != calculated_checksum) {
        ESP_LOGW(TAG, "Checksum mismatch: calculated %u, received %u", calculated_checksum, received_checksum);
        return;
    }

    // Extract sensor data (e.g., PM2.5 value)
    int sensor_value = (data[6] << 8) | data[7];
    ESP_LOGI(TAG, "SSAP10 sensor value: %d", sensor_value);

    // Publish data to Home Assistant
    this->publish_state(sensor_value);
#endif
}

void SSAP10Sensor::read_to_buffer() {
    while (this->available()) {
        uint8_t byte = this->read();
        this->data_buffer_[this->buffer_index_] = byte;
        this->buffer_index_ = (this->buffer_index_ + 1) % BUFFER_SIZE;
    }
}

bool SSAP10Sensor::find_valid_frame(uint8_t* frame_data) {
    // 从当前位置开始，向后搜索有效的帧头
    for (int search_offset = 0; search_offset < BUFFER_SIZE - SSAP10_FRAME_LENGTH; search_offset++) {
        int start_pos = (this->buffer_index_ - search_offset - SSAP10_FRAME_LENGTH + BUFFER_SIZE) % BUFFER_SIZE;
        
        // 检查帧头
        uint8_t header1 = this->data_buffer_[start_pos];
        uint8_t header2 = this->data_buffer_[(start_pos + 1) % BUFFER_SIZE];
        
        if (header1 == 0x42 && header2 == 0x4D) {
            // 提取完整帧数据
            for (int i = 0; i < SSAP10_FRAME_LENGTH; i++) {
                frame_data[i] = this->data_buffer_[(start_pos + i) % BUFFER_SIZE];
            }
            
            // 验证校验和
            if (this->validate_checksum(frame_data)) {
                ESP_LOGD(TAG, "Found valid frame at offset %d", search_offset);
                return true;
            } else {
                ESP_LOGD(TAG, "Found frame header but checksum invalid at offset %d", search_offset);
            }
        }
    }
    
    return false;
}

bool SSAP10Sensor::validate_checksum(const uint8_t* data) {
    uint16_t checksum_calc = 0;
    for (int i = 0; i < SSAP10_FRAME_LENGTH - 2; i++) {
        checksum_calc += data[i];
    }
    
    uint16_t checksum_recv = ((uint16_t)data[SSAP10_FRAME_LENGTH - 2] << 8) | 
                            data[SSAP10_FRAME_LENGTH - 1];
    
    if (checksum_calc != checksum_recv) {
        ESP_LOGD(TAG, "Checksum mismatch: calculated 0x%04X, received 0x%04X", 
                 checksum_calc, checksum_recv);
        return false;
    }
    
    return true;
}

void SSAP10Sensor::process_buffer() {
    uint8_t frame_data[SSAP10_FRAME_LENGTH];
    
    if (this->find_valid_frame(frame_data)) {
        // 提取PM2.5数值
        int pm25_value = (frame_data[6] << 8) | frame_data[7];
        ESP_LOGI(TAG, "PM2.5 Concentration: %d μg/m³", pm25_value);
        this->publish_state(pm25_value);
    } else {
        ESP_LOGW(TAG, "No valid frame found in buffer");
        // 不发布 NAN，保持上一次的值
    }
}

void SSAP10Sensor::dump_config() {
    ESP_LOGCONFIG(TAG, "SSAP10 Sensor:");
    ESP_LOGCONFIG(TAG, "  Update Interval: %d ms", this->get_update_interval());
}

float SSAP10Sensor::get_setup_priority() const {
    return setup_priority::DATA;
}

}  // namespace ssap10
}  // namespace esphome
