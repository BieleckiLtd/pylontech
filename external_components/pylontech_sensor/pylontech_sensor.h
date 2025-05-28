#pragma once
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"
#include "esphome/core/component.h"

namespace esphome {
namespace pylontech_sensor {

class PylontechSensor : public PollingComponent, public sensor::Sensor {
 public:
  // called by sensor.py
  void set_uart(uart::UARTComponent *uart) { this->uart_ = uart; }

  // poll every Component::update_interval (default 60 s, we ignore it)
  void update() override {
    std::string buf;
    uint8_t byte;

    // drain all bytes that arrived since last call
    while (uart_->available()) {
      if (!uart_->read_byte(&byte))          // read one byte
        break;
      if (byte == '\r') break;               // end-of-frame
      buf.push_back(static_cast<char>(byte));
    }

    if (buf.length() < 60) return;           // too short

    auto pos = buf.find("C7");
    if (pos == std::string::npos || pos + 4 > buf.length()) return;

    uint16_t mV = strtol(buf.substr(pos, 4).c_str(), nullptr, 16);
    publish_state(mV / 1000.0f);
  }


 protected:
  uart::UARTComponent *uart_{nullptr};
};

}  // namespace pylontech_sensor
}  // namespace esphome
