//
// Created by alezhu on 10.01.2024.
//

#pragma once
#include <global.h>
#include <vector>


class rgb_converter
{
  public:
    static std::vector<colpic::byte> word2rgb(const std::vector<colpic::word>& words);

    static std::vector<colpic::word> rgb2word(const std::vector<colpic::byte>& rgb);
};
