//
// Created by alezhu on 21.12.2023.
//

#pragma once
#include <filesystem>
#include <fstream>
#include <global.h>
#include <spng.h>
#include <vector>


class png_decoder
{
  public:
    explicit png_decoder(const std::vector<char>& png);

    explicit png_decoder(const std::filesystem::path& png_file_path);

    ~png_decoder();

    [[nodiscard]] std::vector<colpic::byte> get_rgb() const;

    [[nodiscard]] std::tuple<uint32_t, uint32_t> size() const;

  protected:
    spng_ctx* m_ctx{nullptr};
    std::unique_ptr<std::ifstream> m_png_stream{nullptr};
};
