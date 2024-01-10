//
// Created by alezhu on 16.12.2023.
//

#pragma once
#include <cstdint>

struct ColPicHead {
  unsigned long ver;
  unsigned long Height;
  unsigned long Width;
  unsigned long Magicstring;
  unsigned long color_table_size;
  unsigned long px_table_size;
  unsigned long field6_0x18;
  unsigned long field7_0x1c;
};




extern "C" {
int __declspec(dllimport) __stdcall ColPic_EncodeStr(uint16_t* fromcolor16, int picw, int pich, uint8_t* outputdata,
                                                     size_t outputmaxtsize,
                                                     size_t colorsmax);
}
