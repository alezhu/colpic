#include <iostream>
#include "colpiclib.h"
#include "encoder.h"
#include "str_codec.h"
#include <format>

int main()
{
  std::cout << "Hello, World!" << std::endl;
  constexpr int width = 3;
  constexpr int height = 3;
  constexpr int size = width * height;
  uint16_t fromcolor16[size] = {0};

#pragma pack(push,1)
  struct OutputData
  {
    colpic::header head;
    unsigned short color_table[size];
    char px_table[size];
    uint32_t align;
  } outputdata = {0};
#pragma pack(pop)
  OutputData outputdata2 = {0};
  constexpr int out_size = sizeof(outputdata) * 4  / 3  + 4;
  uint8_t outEncoded[82] = {0};

  auto hex = std::format("{:04x}", 1);

  for(int i=0;i<size;i++)
  {
    fromcolor16[i] = i;
  }
  const auto lpOutPutData = reinterpret_cast<uint8_t *>(&outputdata);
  auto result = ColPic_EncodeStr(fromcolor16, width, height,lpOutPutData , sizeof(outputdata), size);
  std::cout << "Result Size: " << result << std::endl;

  result = ColPic_EncodeStr(fromcolor16, width, height,outEncoded , out_size, 1024);
  std::cout << "Result Size: " << result << std::endl;
  std::string resultStr{reinterpret_cast<char *>(outEncoded),static_cast<std::string::size_type>(result)};

  colpic::encoder encoder(fromcolor16,width,height,size);
  if(encoder.encode())
  {
    const auto bufferSize = encoder.get_output_buffer_size();
    if(bufferSize == sizeof(outputdata2))
    {
      if( encoder.fill_output_buffer(&outputdata2,bufferSize) )
      {
        auto result2 = colpic::str_codec::encode(&outputdata2,bufferSize);
      };
    }
  };


  return 0;
}
