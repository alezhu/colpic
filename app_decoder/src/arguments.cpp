//
// Created by alezhu on 19.12.2023.
//

#include "arguments.h"
#include <args/parser.hpp>

arguments::arguments(int argc, char* argv[])
{
  args::null_translator tr{};
  args::parser parser{
    "Thumbnail decoder",
    args::from_main(argc, argv), &tr
  };


  parser.arg(m_gcode_file)
      .meta("GCODE")
      .help("Path to gcode-file");

  parser.parse();
}

std::filesystem::path arguments::gcodeFile() const
{
  return m_gcode_file;
}
