#pragma once
#include <wincodec.h>
#include <atlbase.h>
#include <vector>
#include <string>

struct Bitmap {
  size_t width, height;
  std::vector<uint32_t> data;
};

class ImageCodec {
public:
  static Bitmap loadFromFile(const std::string& fname);
  static void saveToFile(const std::string& fname, Bitmap texture);

private:
  static void initialize();
  static CComPtr<IWICImagingFactory> factory;
  static const WICPixelFormatGUID format;
  static const size_t BYTES_PER_PIXEL;

};

