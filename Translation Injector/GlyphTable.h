#pragma once
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include "utility.h"

class GlyphTable {
public:
  GlyphTable(const std::string& fname);

  ByteArray operator[](const std::wstring& token) const;
  std::wstring operator[](const ByteArray& bytes) const;

  std::wstring extractLoToken(ByteArray::const_iterator& iter, ByteArray::const_iterator tail) const;
  std::wstring extractHiToken(ByteArray::const_iterator& iter) const;
  ByteArray translateLoToken(std::wstringstream& is) const;
  ByteArray translateHiToken(std::wstringstream& is) const;

  size_t getMaxBytePhraseLength() const;

  static constexpr byte SHIFT_CHAR = 0xFD;
  static constexpr byte HIGH_BANK_END_CHAR = 0x7F;
  static const std::map<byte, std::wstring> HIGH_BANK_CONTROL_CHARS;

private:
  size_t maxBytePhraseLength;
  std::map<std::wstring, ByteArray> tableToRom;
  std::map<ByteArray, std::wstring> romToTable;

  std::wstring loadToken(std::wstringstream& ss);
  uint8_t loadByte(std::wstringstream& ss);
  std::pair<std::wstring, ByteArray> parseLine(const std::wstring& line);
  std::vector<std::wstring> loadLines(std::wifstream& fs);

};

