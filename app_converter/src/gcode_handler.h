//
// Created by alezhu on 19.12.2023.
//

#pragma once
#include <filesystem>
#include <optional>
#include <vector>


class gcode_handler
{
  public:
    explicit gcode_handler(std::filesystem::path path);

    [[nodiscard]] std::string read_base64_thumbnail(std::optional<std::string> size) const;

    [[nodiscard]] std::string get_printer() const;

    [[nodiscard]] bool has_gimage_or_simage() const;

    void add_gimage(std::string_view data);

    void add_simage(std::string_view data);

    void save(bool exclude_thumbnails) const;

  protected:
    std::filesystem::path m_path;

    std::vector<std::unique_ptr<std::stringstream>> m_chunks;
    std::vector<size_t> m_thumbinails;
    int m_printer_model_index;
    int m_gimage_index;
    int m_simage_index;
    std::string m_gimage;
    std::string m_simage;

    void _parse();

    std::stringstream* _new_chunk();
};
