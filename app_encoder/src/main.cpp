#include <fstream>
#include <iostream>
#include <png_decoder.h>
#include <thumbnail_creator.h>

#include "arguments.h"

using namespace std::literals;

int main(int argc, char* argv[])
{
  try
  {
    const arguments args{argc, argv};

    const png_decoder png{args.pngFile()};
    const auto rgb = png.get_rgb();
    auto [width,height] = png.size();

    constexpr auto gimage_prefix = R"(gimage)"sv;
    constexpr auto simage_prefix = R"(simage)"sv;
    const auto prefix = width < 200 ? simage_prefix : gimage_prefix;
    const auto strimage = thumbnail_creator::create_new(rgb, width, height, prefix);

    const auto file_name = args.pngFile().replace_extension("encoded.txt");
    std::ofstream output{file_name.c_str(), std::ios::out};
    if (output.is_open())
    {
      output << strimage;
      output.close();
    }
  } catch (const std::exception& e)
  {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}
