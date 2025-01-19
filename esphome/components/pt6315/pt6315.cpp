#include "pt6315.h"

#include "esphome/core/log.h"

#include <cstring>

namespace esphome {
namespace pt6315 {

static const char *const TAG = "pt6315";

static const uint8_t PT6315_UNKNOWN_CHAR = 0b11111111;

// 8 bits for each digit: XABCDEFG
//
//  AAA
// F   B
// F   B
// F   B
//  GGG
// E   C
// E   C
// E   C
//  DDD
//
// Copied from max7219 with modifications to remove unsupported decimal point.
const uint8_t PT6315_ASCII_TO_RAW[95] PROGMEM = {
    0b00000000,            // ' ', ord 0x20
    PT6315_UNKNOWN_CHAR,   // '!', ord 0x21
    0b00100010,            // '"', ord 0x22
    PT6315_UNKNOWN_CHAR,   // '#', ord 0x23
    PT6315_UNKNOWN_CHAR,   // '$', ord 0x24
    0b01001001,            // '%', ord 0x25
    PT6315_UNKNOWN_CHAR,   // '&', ord 0x26
    0b00000010,            // ''', ord 0x27
    0b01001110,            // '(', ord 0x28
    0b01111000,            // ')', ord 0x29
    0b01000000,            // '*', ord 0x2A
    PT6315_UNKNOWN_CHAR,   // '+', ord 0x2B
    0b00010000,            // ',', ord 0x2C
    0b00000001,            // '-', ord 0x2D
    PT6315_UNKNOWN_CHAR,   // '.', ord 0x2E
    PT6315_UNKNOWN_CHAR,   // '/', ord 0x2F
    0b01111110,            // '0', ord 0x30
    0b00110000,            // '1', ord 0x31
    0b01101101,            // '2', ord 0x32
    0b01111001,            // '3', ord 0x33
    0b00110011,            // '4', ord 0x34
    0b01011011,            // '5', ord 0x35
    0b01011111,            // '6', ord 0x36
    0b01110000,            // '7', ord 0x37
    0b01111111,            // '8', ord 0x38
    0b01111011,            // '9', ord 0x39
    0b01001000,            // ':', ord 0x3A
    0b01011000,            // ';', ord 0x3B
    PT6315_UNKNOWN_CHAR,   // '<', ord 0x3C
    0b00001001,            // '=', ord 0x3D
    PT6315_UNKNOWN_CHAR,   // '>', ord 0x3E
    0b01100101,            // '?', ord 0x3F
    0b01101111,            // '@', ord 0x40
    0b01110111,            // 'A', ord 0x41
    0b00011111,            // 'B', ord 0x42
    0b01001110,            // 'C', ord 0x43
    0b00111101,            // 'D', ord 0x44
    0b01001111,            // 'E', ord 0x45
    0b01000111,            // 'F', ord 0x46
    0b01011110,            // 'G', ord 0x47
    0b00110111,            // 'H', ord 0x48
    0b00110000,            // 'I', ord 0x49
    0b00111100,            // 'J', ord 0x4A
    PT6315_UNKNOWN_CHAR,   // 'K', ord 0x4B
    0b00001110,            // 'L', ord 0x4C
    PT6315_UNKNOWN_CHAR,   // 'M', ord 0x4D
    0b00010101,            // 'N', ord 0x4E
    0b01111110,            // 'O', ord 0x4F
    0b01100111,            // 'P', ord 0x50
    0b01110101,            // 'Q', ord 0x51
    0b00000101,            // 'R', ord 0x52
    0b01011011,            // 'S', ord 0x53
    0b00000111,            // 'T', ord 0x54
    0b00111110,            // 'U', ord 0x55
    0b00111110,            // 'V', ord 0x56
    0b00111111,            // 'W', ord 0x57
    PT6315_UNKNOWN_CHAR,   // 'X', ord 0x58
    0b00100111,            // 'Y', ord 0x59
    0b01101101,            // 'Z', ord 0x5A
    0b01001110,            // '[', ord 0x5B
    PT6315_UNKNOWN_CHAR,   // '\', ord 0x5C
    0b01111000,            // ']', ord 0x5D
    PT6315_UNKNOWN_CHAR,   // '^', ord 0x5E
    0b00001000,            // '_', ord 0x5F
    0b00100000,            // '`', ord 0x60
    0b01110111,            // 'a', ord 0x61
    0b00011111,            // 'b', ord 0x62
    0b00001101,            // 'c', ord 0x63
    0b00111101,            // 'd', ord 0x64
    0b01001111,            // 'e', ord 0x65
    0b01000111,            // 'f', ord 0x66
    0b01011110,            // 'g', ord 0x67
    0b00010111,            // 'h', ord 0x68
    0b00010000,            // 'i', ord 0x69
    0b00111100,            // 'j', ord 0x6A
    PT6315_UNKNOWN_CHAR,   // 'k', ord 0x6B
    0b00001110,            // 'l', ord 0x6C
    PT6315_UNKNOWN_CHAR,   // 'm', ord 0x6D
    0b00010101,            // 'n', ord 0x6E
    0b00011101,            // 'o', ord 0x6F
    0b01100111,            // 'p', ord 0x70
    0b01110101,            // 'q', ord 0x71
    0b00000101,            // 'r', ord 0x72
    0b01011011,            // 's', ord 0x73
    0b00000111,            // 't', ord 0x74
    0b00011100,            // 'u', ord 0x75
    0b00011100,            // 'v', ord 0x76
    PT6315_UNKNOWN_CHAR,   // 'w', ord 0x77
    PT6315_UNKNOWN_CHAR,   // 'x', ord 0x78
    0b00100111,            // 'y', ord 0x79
    PT6315_UNKNOWN_CHAR,   // 'z', ord 0x7A
    0b00110001,            // '{', ord 0x7B
    0b00000110,            // '|', ord 0x7C
    0b00000111,            // '}', ord 0x7D
    0b01100011,            // '~', ord 0x7E (degree symbol)
};

void PT6315Display::setup() {
  this->stb_pin_->digital_write(true);
  this->clk_pin_->digital_write(true);
}

void PT6315Display::update() {
  memset(this->buffer_, 0, 16);

  if (this->writer_.has_value())
    (*this->writer_)(*this);
  this->display();
}

uint8_t PT6315Display::remap_bits(const char *from, const char *to, uint8_t value) {
  uint8_t result = 0;
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (from[i] == to[j]) {
        result |= ((value & (1 << (7 - i))) >> (7 - i)) << (7 - j);
      }
    }
  }
  return result;
}

// Digit, colon and icons:
//
//  AAA         J K L M
// F   B   HH
// F   B   HH
// F   B   HH
//  GGG
// E   C   II
// E   C   II
// E   C   II
//  DDD
//
//  0: XABFGCED (first digit)
//  3: XABFGCED (second digit)
//  9: XABFGCED (third digit)
// 12: XABFGHEI (fifth digit and colon)
// 13: AFCDMLKJ (fifth and sixth digits and icons)
// 14: XXXBGCDE (sixth digit)
// 15: XABFGCED (fourth digit)
//
// Day of week:
//
// A B
// C D
// E F
// G H
//
// 5: HABCDEFG

void PT6315Display::print(const char *str) {
  uint8_t index = 0;
  while (str[index] != '\0') {
    uint8_t data = PT6315_UNKNOWN_CHAR;
    if (str[index] >= ' ' && str[index] <= '~')
      data = progmem_read_byte(&PT6315_ASCII_TO_RAW[str[index] - ' ']);

    if (data == PT6315_UNKNOWN_CHAR) {
      ESP_LOGW(TAG, "Encountered character '%c' with no PT6315 representation while translating string!", str[index]);
    } else {
      // Almost every segment has a different layout.
      switch (index) {
        case 0:
          this->buffer_[0] = this->remap_bits("XABCDEFG", "XABFGCED", data);
          break;
        case 1:
          this->buffer_[3] = this->remap_bits("XABCDEFG", "XABFGCED", data);
          break;
        case 2:
          this->buffer_[9] = this->remap_bits("XABCDEFG", "XABFGCED", data);
          break;
        case 3:
          this->buffer_[15] = this->remap_bits("XABCDEFG", "XABFGCED", data);
          break;
        case 4:
          // Don't touch colon bits.
          this->buffer_[12] = (this->buffer_[12] & 0b00000101) | this->remap_bits("XABCDEFG", "XABFGHEI", data);
          // Don't touch sixth digit or icons.
          this->buffer_[13] = (this->buffer_[13] & 0b11001111) | this->remap_bits("XABCDEFG", "XXCDXXXX", data);
          break;
        case 5:
          // Don't touch fifth digit or icons.
          this->buffer_[13] = (this->buffer_[13] & 0b00111111) | this->remap_bits("XABCDEFG", "AFXXMLKJ", data);
          this->buffer_[14] = this->remap_bits("XABCDEFG", "XXXBGCDE", data);
          break;
      }
    }

    index++;
  }
}

void PT6315Display::print_day(uint8_t day) {
  this->buffer_[6] =
    (day & DAY_OF_WEEK_BLANK) |
    ((day & DAY_OF_WEEK_MONDAY) << 6) |
    ((day & DAY_OF_WEEK_TUESDAY) << 4) |
    ((day & DAY_OF_WEEK_WEDNESDAY) << 2) |
    (day & DAY_OF_WEEK_THURSDAY) |
    ((day & DAY_OF_WEEK_FRIDAY) >> 2) |
    ((day & DAY_OF_WEEK_SATURDAY) >> 4) |
    ((day & DAY_OF_WEEK_SUNDAY) >> 6);
}

void PT6315Display::print_colon(bool top, bool bottom) {
  this->buffer_[12] = (this->buffer_[12] & 0b11111010) | (top << 2) | bottom;
}

void PT6315Display::print_icon(uint8_t icon) {
  this->buffer_[13] = (this->buffer_[13] & 0b11110000) | icon;
}

void PT6315Display::write(uint8_t *data, size_t length) {
  this->stb_pin_->digital_write(false);
  for (size_t i = 0; i < length; i++) {
    uint8_t byte = data[i];
    for (int j = 0; j < 8; j++) {
      this->clk_pin_->digital_write(false);
      if (byte & (1 << j)) {
        this->din_pin_->digital_write(true);
      } else {
        this->din_pin_->digital_write(false);
      }
      this->clk_pin_->digital_write(true);
    }
  }
  this->din_pin_->digital_write(false);
  this->stb_pin_->digital_write(true);
}

void PT6315Display::write_byte(uint8_t byte) {
  this->write(&byte, 1);
}

void PT6315Display::display() {
  this->write_byte(0b00000010); // 6 digits, 22 segments
  uint8_t data[17] = { 0b11000000 }; // Address zero.
  memcpy(&data[1], this->buffer_, 16);
  this->write(data, 17);

  uint8_t brightness = this->brightness_;
  if (brightness > 7) {
    brightness = 7;
  }

  this->write_byte(0b10001000 | brightness); // Display on.
}

} // namespace pt6315
} // namespace esphome