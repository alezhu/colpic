//
// Created by alezhu on 18.12.2023.
//

#pragma once
#include <string>
#include <vector>

#include "global.h"


namespace colpic {
  class str_codec {
    public:
      static std::string encode(void* data, size_t size);
      static std::vector<byte> decode(std::string_view data);
  };
}
