//
// Created by alezhu on 19.12.2023.
//

#include "arguments.h"

#include <format>
#include <args/parser.hpp>

#include "global.h"

arguments::arguments(int argc, char* argv[])
{
  args::null_translator tr{};
  args::parser parser{
    std::format("{}\n\rA post processing script to add Elegoo Neptune thumbnails to gcode", PROGRAM_NAME),
    args::from_main(argc, argv), &tr
  };

  parser.arg(m_printer, "p", "printer")
      .opt()
      .meta("PRINTER")
      .help("Printer model to generate for");

  parser.arg(m_size, "s", "size")
      .opt()
      .meta("SIZE")
      .help("SIZE of thumbnail for convert (default 600x600)");

  parser.set<std::true_type>(m_delete_thumbnails, "d", "delete-thumbnails")
      .opt()
      .help("Delete all existing thumbnails in gcode file");

  parser.set<std::true_type>(m_make_backup, "b", "backup")
      .opt()
      .help("Make backup of original gcode file");

  parser.set<std::true_type>(m_no_gimage, "g", "no-gimage")
      .opt()
      .help("Do not create gimage thumbnail");

  parser.arg(m_extractPNG, "e", "extract")
      .opt()
      .meta("PNGFILE")
      .help("Save decoded PNG to PNGFILE file ( pass . for PNGFILE to use GCODE filename)");

  parser.arg(m_gcode_file)
      .meta("GCODE")
      .help("Gcode path provided by PrusaSlicer");

  parser.parse();
}

std::optional<std::string> arguments::printer() const
{
  return m_printer;
}

std::filesystem::path arguments::gcodeFile() const
{
  return m_gcode_file;
}

std::optional<std::filesystem::path> arguments::extractPNG() const
{
  return m_extractPNG;
}

bool arguments::makeBackup() const
{
  return m_make_backup;
}

bool arguments::noGimage() const
{
  return m_no_gimage;
}

std::optional<std::string> arguments::size() const
{
  return m_size;
}

bool arguments::deleteThumbnails() const
{
  return m_delete_thumbnails;
}
