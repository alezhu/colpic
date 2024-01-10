#include <fstream>
#include <future>
#include <iostream>
#include <set>

#include "arguments.h"
#include "gcode_handler.h"
#include "../../colpic_core/include/image_scaler.h"
#include "libbase64.h"
#include "png_decoder.h"
#include "thumbnail_creator.h"

std::set<std::string> OLD_MODELS{
  "NEPTUNE2",
  "NEPTUNE2D",
  "NEPTUNE2S",
  "NEPTUNEX",
};
std::set<std::string> NEW_MODELS{
  "NEPTUNE4",
  "NEPTUNE4PRO",
  "NEPTUNE4PLUS",
  "NEPTUNE4MAX",
  "NEPTUNE3PRO",
  "NEPTUNE3PLUS",
  "NEPTUNE3MAX",
};
using namespace std::literals;

int main(int argc, char* argv[])
{
  try
  {
    const arguments args{argc, argv};

    gcode_handler gcode{args.gcodeFile()};
    if (gcode.has_gimage_or_simage())
    {
      return 2;
    }

    const auto sBase64 = gcode.read_base64_thumbnail(args.size());

    std::vector<char> image;
    size_t image_size = (sBase64.size() / 4 + 1) * 3;
    image.resize(image_size);
    char* lpImage = image.data();
    std::fill_n(lpImage, image_size, 0);
    auto rc = base64_decode(sBase64.c_str(), sBase64.size(), lpImage, &image_size, 0);
    if (rc != 1)
    {
      throw std::logic_error{"Can not decode base64 thumbinail"};
    }
    image.resize(image_size);


    //Find printer model from gcode if not set
    bool is_old_printer = false;
    bool is_new_printer = false;
    while (true)
    {
      auto printerOpt = args.printer();
      if (printerOpt.has_value())
      {
        auto printer = printerOpt.value();
        if ((is_old_printer = OLD_MODELS.contains(printer), is_old_printer)
            || (is_new_printer = NEW_MODELS.contains(printer), is_new_printer))
        {
          break; //Printer passed as argument and supported
        }
      }
      auto printer = gcode.get_printer();
      if ((is_old_printer = OLD_MODELS.contains(printer), is_old_printer)
          || (is_new_printer = NEW_MODELS.contains(printer), is_new_printer))
      {
        break; //Printer extracted from gcode  and supported
      }

      throw std::logic_error("Unsupported printer");
    }
    std::forward_list<std::future<void>> tasks;
    if (args.extractPNG().has_value())
    {
      auto extractPngTask = std::async(std::launch::async, [&] {
        const auto pngFileName = (args.extractPNG().value() == ".")
                                   ? args.gcodeFile().replace_extension("png")
                                   : args.extractPNG().value();
        std::ofstream pngStream{pngFileName.c_str(), std::ios_base::out | std::ios_base::binary};
        pngStream.write(image.data(), image.size());
        pngStream.close();
      });
      tasks.emplace_front(std::move(extractPngTask));
    }

    if (args.makeBackup())
    {
      auto backupTask = std::async(std::launch::async, [&] {
        const auto bakFile = args.gcodeFile().replace_extension("bak");
        copy_file(args.gcodeFile(), bakFile, std::filesystem::copy_options::overwrite_existing);
      });
      tasks.emplace_front(std::move(backupTask));
    }

    auto png = png_decoder{image};
    auto png_rgb = png.get_rgb();
    auto [width,height] = png.size();

    constexpr auto gimage_prefix = R"(gimage)"sv;
    constexpr auto simage_prefix = R"(simage)"sv;
    if (is_old_printer)
    {
      auto png_rgb_200x200 = image_scaler::scale(png_rgb, width, height, 200, 200);
      if (!args.noGimage())
      {
        tasks.emplace_front(std::async(std::launch::async, [&] {
          const auto gimage = thumbnail_creator::create_old(png_rgb_200x200, 200, 200, gimage_prefix);
          gcode.add_gimage(gimage);
        }));
      }
      auto png_rgb_100x100 = image_scaler::scale(png_rgb_200x200, 200, 200, 100, 100);
      auto simage = thumbnail_creator::create_old(png_rgb_100x100, 100, 100, simage_prefix);
      gcode.add_simage(simage);
    } else
    {
      if (!args.noGimage())
      {
        tasks.emplace_front(std::async(std::launch::async, [&] {
          const auto png_rgb_200x200 = image_scaler::scale(png_rgb, width, height, 200, 200);
          const auto gimage = thumbnail_creator::create_new(png_rgb_200x200, 200, 200, gimage_prefix);
          gcode.add_gimage(gimage);
        }));
      }
      const auto png_rgb_160x160 = image_scaler::scale(png_rgb, width, height, 160, 160);
      const auto simage = thumbnail_creator::create_new(png_rgb_160x160, 160, 160, simage_prefix);
      gcode.add_simage(simage);
    }

    for (auto &task: tasks)
    {
      task.wait();
    }

    gcode.save(args.deleteThumbnails());

    return 0;
  } catch (const std::exception& e)
  {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}
