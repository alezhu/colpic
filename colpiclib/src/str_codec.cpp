// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com//
// Created by alezhu on 18.12.2023.
//

#include "str_codec.h"

#include <array>

namespace colpic {
  struct trinity
  {
    byte b1;
    byte b2;
    byte b3;
  };

  constexpr char slash = '\\';
  constexpr char tilda = '~';
  constexpr char zero = '0';


  static void _encode3to4(const colpic::trinity* lpTrinity, std::string& result)
  {
    char symbol = 0;
    for (int j = 0; j < 4; ++j)
    {
      switch (j)
      {
        case 0:
          symbol = static_cast<char>(lpTrinity->b1 >> 2);
          break;
        case 1:
          symbol = static_cast<char>((lpTrinity->b1 & 0b11) << 4 | lpTrinity->b2 >> 4);
          break;
        case 2:
          symbol = static_cast<char>((lpTrinity->b2 & 0xf) << 2 | lpTrinity->b3 >> 6);
          break;
        case 3:
          symbol = static_cast<char>(lpTrinity->b3 & 0x3f);
          break;
        default:
          break;
      }

      symbol += zero;
      if (symbol == slash) symbol = tilda;
      result.push_back(symbol);
    }
  }
}

std::string colpic::str_codec::encode(void* data, size_t size)
{
  const auto div = size / 3;
  const auto mod = size % 3;
  auto lpSrc = static_cast<trinity *>(data);

  const auto out_size = (mod == 0 ? div : div + 1) * 4;
  std::string result{};
  result.reserve(out_size);
  for (int i = 0; i < div; ++i, ++lpSrc)
  {
    _encode3to4(lpSrc, result);
  }
  if (mod > 0)
  {
    trinity t{0};
    t.b1 = lpSrc->b1;
    if (mod > 1)
    {
      t.b2 = lpSrc->b2;
    }
    _encode3to4(&t, result);
  }

  return result;
}

std::vector<colpic::byte> colpic::str_codec::decode(std::string_view data)
{
  std::vector<colpic::byte> result;
  std::array<colpic::byte, 4> octet{0};
  trinity tr{0};
  result.reserve(data.size() * 3 / 4);

  for (int i = 0; i < data.size(); i += 4)
  {
    for (int j = 0; j < 4; ++j)
    {
      octet[j] = data[i + j];
      if (octet[j] == tilda) octet[j] = slash;
      octet[j] -= zero;
    }
    //0       1       2       3
    //xx543210xx543210xx543210xx543210
    //00111111001122220022223300333333
    //1       2       3
    //765432107654321076543210
    //111111112222222233333333
    tr.b1 = octet[0] << 2 | octet[1] >> 4 & 0b11;
    tr.b2 = octet[1] << 4 | octet[2] >> 2 & 0b1111;
    tr.b3 = octet[2] << 6 | octet[3] & 0b111111;
    result.push_back(tr.b1);
    result.push_back(tr.b2);
    result.push_back(tr.b3);
  }
  return result;
}
