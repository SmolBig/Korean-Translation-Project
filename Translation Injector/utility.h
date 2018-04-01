#pragma once
#include <vector>
#include <string>

using byte = uint8_t;
using ByteArray = std::vector<byte>;

namespace Utility {
  std::wstring stringf(const wchar_t* format, ...);
  ByteArray loadFile(const std::string& fname);
  void saveFile(const std::string& fname, const ByteArray& data);
  std::locale& getUTF16Locale();

  static constexpr wchar_t UTF16BOM = 0xFFFE;

  template<class T>
  void append(T& base, const T& added) {
    base.insert(std::end(base), std::begin(added), std::end(added));
  }

  template<class A, class B>
  bool ezEqual(const A& a, const B& b) {
    return std::equal(std::begin(a), std::end(a), std::begin(b));
  }

  template<class T, class RAND>
  T sample(const std::vector<T>& vec, RAND& rand) {
    std::uniform_int_distribution<size_t> dist(0, vec.size() - 1);
    return vec[dist(rand)];
  }
}

