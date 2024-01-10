#include <filesystem>
#include <fstream>
#include <global.h>
#include <image_scaler.h>
#include <iostream>
#include <args/parser.hpp>

#include "png_decoder.h"
#include "png_encoder.h"

using namespace std::literals;


int main(int argc, char* argv[])
{
  try
  {
    std::filesystem::path png_file{};

    //Parse arguments
    {
      args::null_translator tr{};
      args::parser parser{
        "PNG Resizer via avir",
        args::from_main(argc, argv), &tr
      };
      parser.arg(png_file)
          .meta("PNG")
          .help("Path to png-file");

      parser.parse();
    }

    constexpr auto toWidth = 160;
    constexpr auto toHeight = 160;

    //Read PNG
    {
      struct
      {
        std::vector<colpic::byte> rgb{0};
        size_t width{0};
        size_t height{0};
      } png_info;
      //
      {
        png_decoder png{png_file};
        png_info.rgb = png.get_rgb();
        std::tie(png_info.width, png_info.height) = png.size();
      }
      //Resize image
      {
        auto resized = image_scaler::scale(png_info.rgb, png_info.width, png_info.height, toWidth, toHeight);
        //Save PNG
        const auto new_png_file = png_file.replace_extension("160x160.png");
        png_encoder png{resized.data(), resized.size(), toWidth, toHeight};
        png.save(new_png_file);
      }
    }
  } catch (const std::exception& e)
  {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}
