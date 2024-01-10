// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com//
// Created by alezhu on 18.12.2023.
//

#include "encoder.h"
#include <cstring>
#include <stdexcept>

namespace colpic {
  encoder::encoder(const word* colors, uint32_t width, uint32_t height, uint32_t max_colors)
    : m_colors(colors),
      m_width(width),
      m_height(height),
      m_header{0},
      m_size{m_width * m_height},
      m_max_colors(std::min(max_colors, 0x400U)),
      m_colors_handler{colors, m_size},
      m_colors_table{m_colors_handler.build(m_max_colors)}

  {
    m_header.version = VERSION;
    m_header.magic_string = MAGIC_STRING;
    m_header.height = height;
    m_header.width = width;
  }

  bool encoder::encode()
  {
    const auto color_count = m_colors_table->get_colors_count();
    m_color_buffer.resize(color_count);
    const auto lpBuffer = static_cast<void *>(m_color_buffer.data());
    m_header.color_table_size = color_count * sizeof(word);
    if (!m_colors_table->fill_output_buffer(lpBuffer, m_header.color_table_size))
    {
      return false;
    }

    size_t iColor = 0;
    pixel px{0};
    byte colorIndexHighPrev{0};
    while (iColor < m_size)
    {
      const auto color = m_colors[iColor++];
      byte iSeqCount = 1;
      while (iColor < m_size && iSeqCount < 255 && m_colors[iColor] == color)
      {
        iSeqCount++;
        iColor++;
      }

      const word wColorIndex = m_colors_table->get_index_4_color(color);
      const byte colorIndexLow = wColorIndex & 0x1f;
      const byte colorIndexHigh = (wColorIndex >> 5) & 0x1f;
      if (colorIndexHigh != colorIndexHighPrev)
      {
        px.count = 7;
        px.color_index = colorIndexHigh;
        m_px_buffer.push_back(px);
        colorIndexHighPrev = colorIndexHigh;
      }
      px.color_index = colorIndexLow;
      if (iSeqCount < 7)
      {
        px.count = iSeqCount;
        m_px_buffer.push_back(px);
      } else
      {
        px.count = 0;
        m_px_buffer.push_back(px);

        px.value = iSeqCount;
        m_px_buffer.push_back(px);
      }
    }

    m_header.px_table_size = m_px_buffer.size() * sizeof(px);

    return true;
  }

  size_t encoder::get_output_buffer_size() const
  {
    return sizeof(m_header) + m_header.color_table_size + m_header.px_table_size;
  }

  bool encoder::fill_output_buffer(void* lpBuffer, size_t buffer_size) const
  {
    if (m_header.px_table_size == 0)
    {
      return false;
    }
    if (get_output_buffer_size() > buffer_size)
    {
      return false;
    }
    auto lpPointer = static_cast<byte *>(lpBuffer);
    std::fill_n(lpPointer, buffer_size, 0);
    std::memcpy(lpPointer, &m_header, sizeof(m_header));

    lpPointer += sizeof(m_header);
    std::memcpy(lpPointer, m_color_buffer.data(), m_header.color_table_size);

    lpPointer += m_header.color_table_size;
    std::memcpy(lpPointer, m_px_buffer.data(), m_header.px_table_size);

    return true;
  }

  std::vector<word> encoder::decode(void* lpBuffer, size_t buffer_size)
  {
    std::vector<word> result;
    auto lpStart = static_cast<byte *>(lpBuffer);
    const header* lpHeader = static_cast<header *>(lpBuffer);
    if (lpHeader->magic_string != MAGIC_STRING
        || lpHeader->version != VERSION
        || lpHeader->color_table_size == 0
        || lpHeader->px_table_size == 0
        || lpHeader->width == 0 || lpHeader->height == 0)
    {
      throw std::logic_error("Invalid header");
    }
    result.reserve(lpHeader->width * lpHeader->height);

    const auto lpColors = static_cast<const word *>(static_cast<void *>(lpStart + sizeof(header)));
    const auto lpPixelStart = lpStart + sizeof(header) + lpHeader->color_table_size;
    const auto lpPixelEnd = static_cast<pixel *>(static_cast<void *>(lpPixelStart + lpHeader->px_table_size));
    auto lpPixel = static_cast<pixel *>(static_cast<void *>(lpPixelStart));
    byte colorIndexHighPrev{0};
    while (lpPixel < lpPixelEnd)
    {
      auto px = *lpPixel++;
      byte count;
      if (px.count == 7)
      {
        colorIndexHighPrev = px.color_index;
        px = *lpPixel++;
      }
      const word wColorIndex = (colorIndexHighPrev << 5) | px.color_index;
      if (px.count == 0)
      {
        px = *lpPixel++;
        count = px.value;
      } else
      {
        count = px.count;
      }
      word wColor = lpColors[wColorIndex];
      for (int i = 0; i < count; ++i)
      {
        result.push_back(wColor);
      }
    }
    return result;
  }
}
