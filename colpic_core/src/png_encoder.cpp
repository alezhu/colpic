//
// Created by alezhu on 21.12.2023.
//

#include "png_encoder.h"

png_encoder::png_encoder()
{
  /* Creating an encoder context requires a flag */
  m_ctx = spng_ctx_new(SPNG_CTX_ENCODER);

  /* Encode to internal buffer managed by the library */
  spng_set_option(m_ctx, SPNG_ENCODE_TO_BUFFER, 1);
}

png_encoder::png_encoder(const std::vector<colpic::byte>& rgb, uint32_t width, uint32_t height, spng_ihdr* header)
  : png_encoder()
{
  encode(rgb, width, height, header);
}

png_encoder::png_encoder(colpic::byte* rgb, size_t rgb_size, uint32_t width, uint32_t height, spng_ihdr* header)
{
  encode(rgb, rgb_size, width, height, header);
}

void png_encoder::encode(const std::vector<colpic::byte>& rgb, uint32_t width, uint32_t height, spng_ihdr* header)
{
  encode(rgb.data(), rgb.size(), width, height, header);
}

void png_encoder::encode(const colpic::byte* rgb, size_t rgb_size, uint32_t width, uint32_t height, spng_ihdr* header)
{
  spng_ihdr ihdr{
    .bit_depth = 8,
    .color_type = SPNG_COLOR_TYPE_TRUECOLOR
  };
  /* Specify image dimensions, PNG format */
  if (header == nullptr)
  {
    header = &ihdr;
  }

  header->width = width;
  header->height = height;
  /* Image will be encoded according to ihdr.color_type, .bit_depth */
  spng_set_ihdr(m_ctx, header);

  /* SPNG_FMT_PNG is a special value that matches the format in ihdr,
     SPNG_ENCODE_FINALIZE will finalize the PNG with the end-of-file marker */
  const int error = spng_encode_image(m_ctx, rgb, rgb_size, SPNG_FMT_PNG, SPNG_ENCODE_FINALIZE);
  if (error != 0)
  {
    throw std::logic_error{spng_strerror(error)};
  }
}

std::vector<colpic::byte> png_encoder::get_png() const
{
  std::vector<colpic::byte> result;

  /* PNG is written to an internal buffer by default */
  size_t png_size;
  int error{0};
  const auto png = static_cast<colpic::byte *>(spng_get_png_buffer(m_ctx, &png_size, &error));
  if (error != 0)
  {
    throw std::logic_error{spng_strerror(error)};
  }

  if (png != nullptr)
  {
    result.resize(png_size);
    result.assign(png, png + png_size);

    /* User owns the buffer after a successful call */
    free(png);
  }
  return result;
}

void png_encoder::save(const std::filesystem::path& path) const
{
  std::ofstream pngStream{path.c_str(), std::ios_base::out | std::ios_base::binary};
  save(pngStream);
  pngStream.close();
}

void png_encoder::save(std::ostream& stream) const
{
  auto png = get_png();
  stream.write(static_cast<const char *>(static_cast<void *>(png.data())), png.size());
}

png_encoder::~png_encoder()
{
  spng_ctx_free(m_ctx);
}
