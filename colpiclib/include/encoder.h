//
// Created by alezhu on 18.12.2023.
//

#pragma once
#include <cstdint>
#include "colors_handler.h"

namespace colpic {
  struct header
  {
    unsigned char version;
    unsigned char c_unknown1;
    unsigned char c_unknown2;
    unsigned char c_unknown3;
    uint32_t height;
    uint32_t width;
    uint32_t magic_string;
    uint32_t color_table_size;
    uint32_t px_table_size;
    uint32_t dw_reserved1;
    uint32_t dw_reserved2;
  };

  class encoder
  {
    public:
      encoder(const word* colors, uint32_t width, uint32_t height, uint32_t max_colors = 1024);

      bool encode();

    [[nodiscard]] size_t get_output_buffer_size( ) const;

      bool fill_output_buffer(void* lpBuffer, size_t buffer_size) const;

    static std::vector<word> decode(void* lpBuffer, size_t buffer_size);

    protected:
      const uint16_t* m_colors;
      const uint32_t m_width;
      const uint32_t m_height;
      header m_header;
      uint64_t m_size{};
      uint32_t m_max_colors;
      colors_handler m_colors_handler;
      std::unique_ptr<color_table> m_colors_table;
      std::vector<word> m_color_buffer;

#pragma pack(push,1)
      union pixel
      {
        struct
        {
          unsigned char color_index: 5;
          unsigned char count: 3;
        };

        unsigned char value;
      };
#pragma pack(pop)

      std::vector<pixel> m_px_buffer;
  };
}
