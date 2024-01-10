//
// Created by alezhu on 21.12.2023.
//

#pragma once
#include <global.h>
#include <vector>


class image_scaler
{
  public:
    static std::vector<colpic::byte> scale(const std::vector<colpic::byte>& src, size_t fromWidth, size_t fromHeight,
                                           size_t toWidth, size_t toHeight);
};
