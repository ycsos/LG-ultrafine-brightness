//
// Created by nagato on 9/9/20.
//

#ifndef LG_ULTRAFINE_BRIGHTNESS__BRIGHTNESS_H_
#define LG_ULTRAFINE_BRIGHTNESS__BRIGHTNESS_H_

#include <iostream>
#include <vector>
class Brightness {
 public:
  static const uint16_t max_brightness = 0xd2f0; // max brightness 54000
  static const uint16_t min_brightness = 0x0000;
 private:
  uint16_t lg_brightness;
};

#endif //LG_ULTRAFINE_BRIGHTNESS__BRIGHTNESS_H_
