#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace pt6315 {

class PT6315Display;

enum DayOfWeek : uint8_t {
    DAY_OF_WEEK_MONDAY = 1 << 0,
    DAY_OF_WEEK_TUESDAY = 1 << 1,
    DAY_OF_WEEK_WEDNESDAY = 1 << 2,
    DAY_OF_WEEK_THURSDAY = 1 << 3,
    DAY_OF_WEEK_FRIDAY = 1 << 4,
    DAY_OF_WEEK_SATURDAY = 1 << 5,
    DAY_OF_WEEK_SUNDAY = 1 << 6,
    DAY_OF_WEEK_BLANK = 1 << 7,
};

enum Icon : uint8_t {
    ICON_STARS = 1 << 0,
    ICON_SUNRISE = 1 << 1,
    ICON_G_SUN = 1 << 2,
    ICON_MOON = 1 << 3,
};

using pt6315_writer_t = std::function<void(PT6315Display &)>;

class PT6315Display : public PollingComponent {
 public:
  void setup() override;
  void update() override;

  void set_din_pin(GPIOPin *pin) { this->din_pin_ = pin; }
  void set_clk_pin(GPIOPin *pin) { this->clk_pin_ = pin; }
  void set_stb_pin(GPIOPin *pin) { this->stb_pin_ = pin; }
  void set_writer(pt6315_writer_t &&writer) { this->writer_ = writer; }

  uint8_t remap_bits(const char *from, const char *to, uint8_t value);
  void print(const char *str);
  void print_day(uint8_t day);
  void print_colon(bool top = true, bool bottom = true);
  void print_icon(uint8_t icon);

  void set_brightness(uint8_t brightness) { this->brightness_ = brightness; }

  void write(uint8_t *data, size_t length);
  void write_byte(uint8_t byte);
  void display();

 protected:

  GPIOPin *din_pin_;
  GPIOPin *clk_pin_;
  GPIOPin *stb_pin_;
  optional<pt6315_writer_t> writer_{};

  uint8_t buffer_[16];
  uint8_t brightness_;
};

} // namespace pt6315
} // namespace esphome