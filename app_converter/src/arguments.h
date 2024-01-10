//
// Created by alezhu on 19.12.2023.
//

#pragma once
#include <filesystem>
#include <optional>
#include <string>


class arguments
{
  public:
    arguments(int argc, char* argv[]);

    [[nodiscard]] std::optional<std::string> printer() const;

    [[nodiscard]] std::filesystem::path gcodeFile() const;

    [[nodiscard]] std::optional<std::filesystem::path> extractPNG() const;

    [[nodiscard]] bool makeBackup() const;

    [[nodiscard]] bool noGimage() const;

    [[nodiscard]] std::optional<std::string> size() const;

    [[nodiscard]] bool deleteThumbnails() const;

  protected:
    std::optional<std::filesystem::path> m_extractPNG;
    std::filesystem::path m_gcode_file;
    std::optional<std::string> m_printer;
    std::optional<std::string> m_size;
    bool m_make_backup{false};
    bool m_no_gimage{false};
    bool m_delete_thumbnails{false};
};
