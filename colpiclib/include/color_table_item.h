//
// Created by alezhu on 18.12.2023.
//

#pragma once
#include <utility>

#include "global.h"

namespace colpic {

  class color_table_item
  {
    public:
      color_table_item(const color_table_item& other) = default;

      color_table_item(color_table_item&& other) noexcept = default;

      color_table_item& operator=(color_table_item other)
      {
        using std::swap;
        swap(*this, other);
        return *this;
      }

    private:
      friend void swap(color_table_item& lhs, color_table_item& rhs) noexcept
      {
        using std::swap;
        swap(lhs.rgb, rhs.rgb);
        swap(lhs.count, rhs.count);
        swap(lhs.m_r, rhs.m_r);
        swap(lhs.m_g, rhs.m_g);
        swap(lhs.m_b, rhs.m_b);
      }

    public:
      explicit color_table_item(word rgb, size_t count);


      word rgb;
      size_t count;

      byte r() const;

      byte g() const;

      byte b() const;

    protected:
      mutable byte m_r;
      mutable byte m_g;
      mutable byte m_b;

      void _parse_rgb() const;
  };
} // colpic
