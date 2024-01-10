//
// Created by alezhu on 18.12.2023.
//

#include "color_table.h"
#include <cmath>

colpic::color_table::color_table(const std::vector<color_table_item>& color_items,
                                 color_index_type max_colors): m_color_items(
                                                                 color_items), m_max_colors(max_colors),
                                                               m_colors_count(0)
{
}

colpic::color_index_type colpic::color_table::get_index_4_color(color_index_type color) const
{
  if (m_color_index.empty())
  {
    _build_color_index();
  }
  const auto pos = m_color_index.find(color);
  if (pos == m_color_index.end())
  {
    return 0;
  }

  return pos->second;
}

colpic::color_index_type colpic::color_table::get_colors_count() const
{
  if (m_color_index.empty())
  {
    _build_color_index();
  }
  return m_colors_count;
}

bool colpic::color_table::fill_output_buffer(void* lpBuffer, size_t buffer_size) const
{
  const auto need_buffer_size = get_colors_count() * sizeof(word);
  if (need_buffer_size > buffer_size || lpBuffer == nullptr)
  {
    return false;
  }
  std::fill_n<byte*>(static_cast<byte*>(lpBuffer), buffer_size, 0);
  auto lpColor = static_cast<word *>(lpBuffer);
  for (int i = 0; i < m_colors_count; ++i)
  {
    *lpColor++ = m_color_items[i].rgb;
  }
  return true;
}

void colpic::color_table::_build_color_index() const
{
  m_color_index.clear();
  const auto real_color_count = m_color_items.size();
  m_colors_count = std::min(real_color_count, m_max_colors);
  for (auto i = 0; i < m_colors_count; ++i)
  {
    m_color_index.emplace(m_color_items.at(i).rgb, i);
  }
  auto iReplaceColor = m_colors_count;
  while (iReplaceColor < real_color_count)
  {
    auto& color_to_replace = m_color_items[iReplaceColor];
    word wColorDeltaMin = 0xffff;
    auto replace_index = 0;
    for (auto i = 0; i < m_colors_count; ++i)
    {
      auto& color_item = m_color_items.at(i);
      const word delta = abs(color_to_replace.r() - color_item.r()) +
                         abs(color_to_replace.g() - color_item.g()) +
                         abs(color_to_replace.b() - color_item.b());
      if (delta < wColorDeltaMin)
      {
        wColorDeltaMin = delta;
        replace_index = i;
      }
    }

    m_color_index.emplace(color_to_replace.rgb, replace_index);

    ++iReplaceColor;
  }
}
