//
// Created by alezhu on 21.12.2023.
//

#include "../../colpic_core/include/image_scaler.h"
#include "avir.h"
#include "lancir.h"
//#include <vips/vips.h>

std::vector<colpic::byte> image_scaler::scale(const std::vector<colpic::byte>& src, size_t fromWidth, size_t fromHeight,
                                              size_t toWidth, size_t toHeight)
{
  if(fromWidth == toWidth && fromHeight == toHeight) return src;
  std::vector<colpic::byte> result;
  const auto out_size = toWidth * toHeight * 3;
  result.resize(out_size);

   const avir::CImageResizer<> resizer(8);
  //avir::CLancIR resizer;
  resizer.resizeImage(src.data(), static_cast<int>(fromWidth), static_cast<int>(fromHeight), 0,
                      result.data(), static_cast<int>(toWidth), static_cast<int>(toHeight),
                      3, 0);


  return result;
}
