//
// Created by alezhu on 19.12.2023.
//

#include "arguments.h"
#include <args/parser.hpp>

arguments::arguments(int argc, char* argv[])
{
  args::null_translator tr{};
  args::parser parser{
    "PNG thumbnail encoder",
    args::from_main(argc, argv), &tr
  };


  parser.arg(m_png_file)
      .meta("PNG")
      .help("Path to PNG-file");

  parser.parse();
}

std::filesystem::path arguments::pngFile() const
{
  return m_png_file;
}
