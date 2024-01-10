//
// Created by alezhu on 18.12.2023.
//

#pragma once
#include <memory>
#include <vector>

#include "color_table.h"
#include "color_table_item.h"
#include "global.h"

namespace colpic {


  class colors_handler
  {
    public:
      colors_handler(const word colors[], size_t colors_count);

      std::unique_ptr<color_table> build(word max_colors);

    protected:
      const size_t m_colorsCount;
      const word* m_colors;
      std::vector<color_table_item> m_color_items;

      void _build_color_items();

  };
} // colpic
