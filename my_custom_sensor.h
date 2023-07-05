#include "esphome.h"

class MyCustomSensor : public PollingComponent, public Sensor
{
public:
    Sensor *pm2_5_sensor = new Sensor();

    // constructor
    MyCustomSensor() : PollingComponent(5000) {}

    float get_setup_priority() const override { return esphome::setup_priority::HARDWARE; }

    void setup() override
    {
        // This will be called by App.setup()
        Serial2.begin(9600, SERIAL_8N1, 21, 22);
    }
    void update() override
    {
        // This will be called every "update_interval" milliseconds.

        char data[32] = {0};
        Serial2.readBytes(data, 32);

        if ((data[0] == 0x42) && (data[1] = 0x4d))
        {

            int value = (int)data[6] * 256 + (int)data[7];
            pm2_5_sensor->publish_state(value);
        }

        String hex_str = "";
        for (int i = 0; i < 32; i++)
        {
            char temp[5];
            sprintf(temp, "%02X", data[i]);
            hex_str += temp;
        }

        Serial2.println(hex_str);
        Serial.println(hex_str);
        while (Serial2.read() >= 0)
            ;
    }
};