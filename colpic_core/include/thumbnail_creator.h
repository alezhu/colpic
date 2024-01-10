//
// Created by alezhu on 21.12.2023.
//

#pragma once
#include <global.h>
#include <string>
#include <vector>

class thumbnail_creator
{
  public:
    static std::string create_old(const std::vector<colpic::byte>& png_rgb, uint32_t width, uint32_t height,
                                  std::string_view prefix);

    static std::string create_new(const std::vector<colpic::byte>& png_rgb, uint32_t width, uint32_t height,
                                  std::string_view prefix);

  protected:
    static std::vector<colpic::word> _colpic_prepare(const std::vector<colpic::byte>& png_rgb);
};
