//
// Created by alezhu on 18.12.2023.
//

#pragma once
#include <unordered_map>
#include <vector>
#include "color_table_item.h"

namespace colpic {
  using color_index_type  = std::vector<color_table_item>::size_type;

  class color_table
  {
    public:
      color_table(const std::vector<color_table_item>& color_items, color_index_type max_colors);

      color_index_type get_index_4_color(color_index_type color) const;

      color_index_type get_colors_count() const;

      bool fill_output_buffer(void* lpBuffer, size_t buffer_size ) const;

    protected:
      mutable std::unordered_map<word, color_index_type> m_color_index;
      const std::vector<color_table_item>& m_color_items;
      const color_index_type m_max_colors;
      mutable color_index_type m_colors_count;


      void _build_color_index() const;
  };
} // colpic
