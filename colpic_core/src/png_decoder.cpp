//
// Created by alezhu on 21.12.2023.
//

#include "png_decoder.h"

png_decoder::png_decoder(const std::vector<char>& png)
{
  /* Create a decoder context */
  m_ctx = spng_ctx_new(0);
  /* Set an input buffer */
  spng_set_png_buffer(m_ctx, png.data(), png.size());
}

png_decoder::png_decoder(const std::filesystem::path& png_file_path)
  : m_png_stream(std::make_unique<std::ifstream>(png_file_path.c_str(), std::ios::binary))
{
  /* Create a decoder context */
  m_ctx = spng_ctx_new(0);
  if (m_png_stream->is_open())
  {
    auto read_fn = [](spng_ctx* /*ctx*/, void* user, void* dst_src, size_t length) -> int {
      const auto png_stream = static_cast<std::ifstream*>(user);
      if (png_stream->eof()) return SPNG_IO_EOF;
      png_stream->read(static_cast<char *>(dst_src), length);
      return 0;
    };
    spng_set_png_stream(m_ctx,read_fn, m_png_stream.get());
  }
}

png_decoder::~png_decoder()
{
  //m_png_stream->close();
  spng_ctx_free(m_ctx);
}

std::vector<colpic::byte> png_decoder::get_rgb() const
{
  /* Determine output image size */
  size_t out_size = 0;
  spng_decoded_image_size(m_ctx, SPNG_FMT_RGB8, &out_size);

  std::vector<colpic::byte> out{0};
  out.resize(out_size);
  /* Decode to 8-bit RGB */
  spng_decode_image(m_ctx, out.data(), out_size, SPNG_FMT_RGB8, 0);

  return out;
}

std::tuple<uint32_t, uint32_t> png_decoder::size() const
{
  spng_ihdr ihdr{0};
  spng_get_ihdr(m_ctx, &ihdr);
  return {ihdr.width, ihdr.height};
}
