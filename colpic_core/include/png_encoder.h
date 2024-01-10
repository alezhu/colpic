//
// Created by alezhu on 21.12.2023.
//

#pragma once
#include <filesystem>
#include <fstream>
#include <global.h>
#include <spng.h>
#include <vector>


class png_encoder
{
  public:
    explicit png_encoder();
    explicit png_encoder(const std::vector<colpic::byte>& rgb, uint32_t width, uint32_t height, spng_ihdr* header = nullptr);
    explicit png_encoder(colpic::byte* rgb, size_t rgb_size, uint32_t width, uint32_t height, spng_ihdr* header = nullptr);

    void encode(const std::vector<colpic::byte>& rgb, uint32_t width, uint32_t height, spng_ihdr* header = nullptr);
    void encode(const colpic::byte* rgb, size_t rgb_size, uint32_t width, uint32_t height, spng_ihdr* header = nullptr);

    [[nodiscard]] std::vector<colpic::byte> get_png() const;

    void save(const std::filesystem::path& path) const;

    void save(std::ostream& stream) const;

    ~png_encoder();

  protected:
    spng_ctx* m_ctx{nullptr};
};
