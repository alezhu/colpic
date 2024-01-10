//
// Created by alezhu on 18.12.2023.
//


#include "colors_handler.h"

#include <algorithm>
#include <numeric>
#include <sstream>


colpic::colors_handler::colors_handler(const word colors[], size_t colors_count): m_colorsCount(colors_count),
  m_colors(colors)
{
}

void colpic::colors_handler::_build_color_items()
{
  std::unordered_map<word, size_t> _color_map;
  std::stringstream test{};

  for (int i = 0; i < m_colorsCount; ++i)
  {
    auto value = m_colors[i];
    auto found = _color_map.find(value);
    if (found == _color_map.end())
    {
      _color_map.emplace(value, 1);
    } else
    {
      found->second++;
    }
    test << value<< std::endl;
  }
  m_color_items.clear();
  m_color_items.reserve(_color_map.size());
  for (auto [color, count]: _color_map)
  {
    m_color_items.emplace_back(color, count);
  }

  std::ranges::sort(m_color_items,
                    [&](const auto& a, const auto& b) -> bool {
                      return a.count > b.count;
                    });

}

std::unique_ptr<colpic::color_table> colpic::colors_handler::build(word max_colors)
{
  if (m_color_items.empty())
  {
    _build_color_items();
  }
  return std::make_unique<color_table>(m_color_items, max_colors);
}
