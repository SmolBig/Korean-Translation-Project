#include "utility.h"
#include <cstdarg>
#include <fstream>
#include <locale>
#include <codecvt>

std::wstring Utility::stringf(const wchar_t* format, ...) {
  va_list args;
  va_start(args, format);
  std::vector<wchar_t> buffer(_vscwprintf(format, args) + 1);
  _vsnwprintf_s(buffer.data(), buffer.size(), buffer.size(), format, args);
  va_end(args);

  return buffer.data();
}

ByteArray Utility::loadFile(const std::string& fname) {
  std::ifstream file(fname, std::ifstream::binary);
  if(!file) { throw std::runtime_error("loadFile() - File could not be opened."); }

  file.seekg(0, file.end);
  int len = (int)file.tellg();
  file.seekg(0, file.beg);

  ByteArray data;
  data.resize(len);
  file.read((char*)&data[0], len);

  return data;
}

void Utility::saveFile(const std::string& fname, const ByteArray& data) {
  std::ofstream file(fname, std::ofstream::binary);
  if(!file) { throw std::runtime_error("saveFile() - File could not be opened."); }
  file.write((const char*)data.data(), data.size());
}

std::locale& Utility::getUTF16Locale() {
  //this is not a memory leak - the locale frees the facet when it destructs
  static std::locale loc(std::locale::classic(), new std::codecvt_utf16<wchar_t, 0x10FFFF, std::consume_header>);
  return loc;
}
