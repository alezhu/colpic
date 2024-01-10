//
// Created by alezhu on 18.12.2023.
//

#pragma once
#include <cstdint>

namespace colpic {
  using word = uint16_t;
  using byte = unsigned char;
  inline constexpr byte VERSION = 3;
  inline constexpr uint32_t MAGIC_STRING = 0x5ddc33c;
}
