//
// Created by alezhu on 10.01.2024.
//


#include "rgb_converter.h"

std::vector<colpic::byte> rgb_converter::word2rgb(const std::vector<colpic::word>& words)
{
  std::vector<colpic::byte> result;
  result.reserve(words.size() / 2 * 3);
  for (const auto value: words)
  {
    colpic::byte r = (value >> 11 & 0xff) << 3;
    colpic::byte g = (value >> 5 & 0xff) << 2;
    colpic::byte b = (value & 0xff) << 3;
    result.push_back(r);
    result.push_back(g);
    result.push_back(b);
  }
  return result;
}

std::vector<colpic::word> rgb_converter::rgb2word(const std::vector<colpic::byte>& rgb)
{
  const auto size = rgb.size();
  const auto count = size / 3;
  std::vector<colpic::word> result{};
  result.reserve(count);

  for (int i = 0; i < size; i += 3)
  {
    const colpic::byte r = rgb[i];
    const colpic::byte g = rgb[i + 1];
    const colpic::byte b = rgb[i + 2];
    colpic::word value = (static_cast<colpic::word>(r >> 3) << 11) | (static_cast<colpic::word>(g >> 2) << 5) |
                       static_cast<colpic::word>(b >> 3);
    result.push_back(value);
  }

  return result;
}
