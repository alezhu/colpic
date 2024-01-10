//
// Created by alezhu on 18.12.2023.
//


#include "color_table_item.h"

colpic::byte initial_color_part{static_cast<colpic::byte>(-1)};

colpic::color_table_item::color_table_item(word rgb, size_t count): rgb(rgb),
                                                                   count(count), m_r(initial_color_part),
                                                                   m_g(initial_color_part), m_b(initial_color_part)
{
}

colpic::byte colpic::color_table_item::r() const
{
  if (m_r == initial_color_part)
  {
    _parse_rgb();
  }
  return m_r;
}

colpic::byte colpic::color_table_item::g() const
{
  if (m_g == initial_color_part)
  {
    _parse_rgb();
  }
  return m_g;
}

colpic::byte colpic::color_table_item::b() const
{
  if (m_b == initial_color_part)
  {
    _parse_rgb();
  }
  return m_b;
}

void colpic::color_table_item::_parse_rgb() const
{
  m_r = rgb >> 11 & 0x1f;
  m_g = (rgb >> 5 ) & 0b111111;
  m_b = rgb & 0x1f;
}
