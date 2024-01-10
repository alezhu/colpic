//
// Created by alezhu on 19.12.2023.
//

#pragma once
#include <filesystem>


class arguments
{
  public:
    arguments(int argc, char* argv[]);

    [[nodiscard]] std::filesystem::path pngFile() const;

  protected:
    std::filesystem::path m_png_file;
};
