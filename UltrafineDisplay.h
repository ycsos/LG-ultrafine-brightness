//
// Created by nagato on 9/9/20.
//

#ifndef LG_ULTRAFINE_BRIGHTNESS__ULTRAFINEDISPLAY_H_
#define LG_ULTRAFINE_BRIGHTNESS__ULTRAFINEDISPLAY_H_

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <libusb.h>
#include <unordered_map>
using std::unordered_map;

// From the HID spec:

static const int HID_GET_REPORT = 0x01;
static const int HID_SET_REPORT = 0x09;
static const int HID_REPORT_TYPE_INPUT = 0x01;
static const int HID_REPORT_TYPE_OUTPUT = 0x02;
static const int HID_REPORT_TYPE_FEATURE = 0x03;

extern unordered_map<uint16_t, char *> support_device;

class UltrafineDisplay {
 public:
  UltrafineDisplay(libusb_device * lg_usb);
  ~UltrafineDisplay();
  uint16_t get_brightness();
  void set_brightness(uint16_t val);
  uint8_t get_brightness_level();
  void set_brightness_level(uint8_t);
  char * getDisplayName();
  void interactive();
 private:
  char DisplayType[128];
  uint16_t product_id;
  uint16_t vendor_id;
  libusb_device * lg_dev;
  libusb_device_handle * lg_handle;
  int lg_iface = 1;
  int libusb_success;
  uint8_t brightness_step = 5;
};

#endif //LG_ULTRAFINE_BRIGHTNESS__ULTRAFINEDISPLAY_H_
