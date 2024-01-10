//
// Created by alezhu on 21.12.2023.
//

#include "thumbnail_creator.h"

#include <encoder.h>
#include <format>
#include <sstream>
#include <str_codec.h>

#include "rgb_converter.h"

using namespace std::literals;

std::string thumbnail_creator::create_old(const std::vector<colpic::byte>& png_rgb, uint32_t width,
                                          uint32_t height,
                                          std::string_view prefix)
{
  std::stringstream result{};
  const auto image = _colpic_prepare(png_rgb);
  result << ";" << prefix;
  uint32_t x = 0;
  for (auto pixel: image)
  {
    auto hex = std::format("{:04x}", pixel);
    result << hex.substr(2, 2) << hex.substr(0, 2);
    if (++x == width)
    {
      result << "\rM10086 ;";
      x = 0;
    }
  }

  result << "\r";

  return result.str();
}

std::string thumbnail_creator::create_new(const std::vector<colpic::byte>& png_rgb, uint32_t width,
                                          uint32_t height,
                                          std::string_view prefix)
{
  constexpr std::string_view SC{";"sv};
  constexpr std::string_view CR{"\r"sv};

  const auto image = _colpic_prepare(png_rgb);
  std::stringstream result{};
  auto row_prefix = std::string(SC) + std::string(prefix) + ":";
  std::vector<colpic::byte> out{0};
  size_t out_size{0};
  //Fill
  {
    colpic::encoder encoder{image.data(), width, height, 1024};
    encoder.encode();
    out_size = encoder.get_output_buffer_size();
    out.resize(out_size);
    encoder.fill_output_buffer(out.data(), out_size);
  }


  const auto encoded = colpic::str_codec::encode(out.data(), out_size);
  const auto row_size = 1024 - 1 - row_prefix.size();
  auto it = encoded.cbegin();
  const auto end = encoded.cend();
  while (it < end)
  {
    auto count = end - it;
    if (row_size < count)
    {
      const std::string_view view{it.operator->(), row_size};
      result << row_prefix << view << CR;
    } else
    {
      const std::string_view view{it.operator->(), static_cast<std::string_view::size_type>(count)};
      result << SC << row_prefix << view << CR;
      const std::string::size_type append_len = row_size - 3 - (encoded.size() % row_size);
      std::string append(append_len, '0');
      result << SC << append << CR;
      break;
    }
    it += static_cast<std::string::const_iterator::difference_type>(row_size);
  }

  return result.str();
}

std::vector<colpic::word> thumbnail_creator::_colpic_prepare(const std::vector<colpic::byte>& png_rgb)
{
  return rgb_converter::rgb2word(png_rgb);
}
