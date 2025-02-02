![Photo of LGL VCK CCCP 2024 clock](images/clock.jpg)

# LGL VCK CCCP 2024
The LGL VCK CCCP 2024 is a VFD clock powered by an ESP8285, available from AliExpress and LGL Studio.

This repository contains reverse engineered information about the clock and an ESPHome configuration to allow controlling the clock from Home Assistant.

## Hardware
* DOIT ESP-M2 (ESP8285)
* Epson RX-8025 RTC
* PTC PT6315 VFD controller
* Light-dependent resistor (for ambient brightness)
* 2 buttons

## Pinout
* ADC - Light-dependent resistor
* GPIO0 - Left button (requires pull-up)
* GPIO1 (TX) - Pad 2
* GPIO2 - LED
* GPIO3 (RX) - Pad 1
* GPIO4 - RX-8025 SDA
* GPIO5 - RX-8025 SCL
* GPIO12 - Right button (requires pull-up)
* GPIO13 - PT6315 DIN
* GPIO14 - PT6315 CLK
* GPIO15 - PT6315 STB
* GPIO16 - Buzzer
* 5V - Pad 3
* GND - Pad 4

**Note**: pads are numbered 1 to 4 from left to right, with the clock face facing away from you.

## Flashing
To flash the board, connect your serial adapter to pads 1 (TX), 2 (RX) and 4 (GND) and hold the button nearest the USB-C connector whilst plugging the clock in.

The provided ESPHome configuration can be found in the [esphome](esphome) directory. This includes a custom component to drive the PT6315, as this is not currently supported by ESPHome.
