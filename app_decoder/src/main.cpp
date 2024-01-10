#include <encoder.h>
#include <fstream>
#include <iostream>
#include <png_encoder.h>
#include <spng.h>

#include "arguments.h"
#include "rgb_converter.h"
#include "str_codec.h"

enum state
{
  NOT_STARTED,
  STARTED,
  FINISHED
};

using namespace std::literals;

std::vector<colpic::word> decode_old(std::stringstream& stream)
{
  std::vector<uint16_t> result;
  std::uint8_t low;
  std::uint8_t high;
  stream.seekg(0);
  while (stream.eof())
  {
    stream >> std::hex >> low >> high;
    result.emplace_back(static_cast<uint16_t>(high) << 8 | low);
  }
  return result;
}

std::vector<colpic::word> decode_new(const std::stringstream& stream)
{
  auto cp_image = colpic::str_codec::decode(stream.view());
  auto result = colpic::encoder::decode(cp_image.data(), cp_image.size());
  return result;
}

void save2Png(std::stringstream& simage, const bool old, const uint32_t size, const std::filesystem::path& png_file)
{
  const auto words = old ? decode_old(simage) : decode_new(simage);
  const auto rgb = rgb_converter::word2rgb(words);
  const png_encoder png{rgb, size, size};
  png.save(png_file);
}


int main(int argc, char* argv[])
{
  try
  {
    const arguments args{argc, argv};

    std::stringstream simage;
    std::stringstream gimage;
    bool old = false;

    state simage_state{NOT_STARTED};
    state gimage_state{NOT_STARTED};

    std::ifstream stream{args.gcodeFile().c_str()};
    if (stream.is_open())
    {
      std::string line;
      constexpr auto marker_gimage{";gimage:"sv};
      constexpr auto marker_gimage_end{";;gimage:"sv};
      constexpr auto marker_simage{";simage:"sv};
      constexpr auto marker_simage_end{";;simage:"sv};

      while (std::getline(stream, line, '\r') && (simage_state != FINISHED || gimage_state != FINISHED))
      {
        if (line.starts_with("M10086"))
        {
          old = true;
          continue;
        }

        if (line.starts_with(marker_simage))
        {
          if (simage_state == NOT_STARTED) simage_state = STARTED;
          simage << line.substr(marker_simage.size());
        } else if (line.starts_with(marker_simage_end))
        {
          simage << line.substr(marker_simage_end.size());
          simage_state = FINISHED;
        } else if (simage_state == STARTED) simage_state = FINISHED;

        if (line.starts_with(marker_gimage))
        {
          if (gimage_state == NOT_STARTED) gimage_state = STARTED;
          gimage << line.substr(marker_gimage.size());
        } else if (line.starts_with(marker_gimage_end))
        {
          gimage << line.substr(marker_gimage_end.size());
          gimage_state = FINISHED;
        } else if (gimage_state == STARTED) gimage_state = FINISHED;
      }
      stream.close();
    }

    if (!simage.view().empty())
    {
      while (simage.view().size() % 4 != 00)
      {
        simage << "0";
      }
      auto png_file = args.gcodeFile().replace_extension("simage.png");
      save2Png(simage, old, old ? 100 : 160, png_file);
    }

    if (!gimage.view().empty())
    {
      while (gimage.view().size() % 4 != 00)
      {
        gimage << "0";
      }
      auto png_file = args.gcodeFile().replace_extension("gimage.png");
      save2Png(gimage, old, 200, png_file);
    }
  } catch (const std::exception& e)
  {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}
