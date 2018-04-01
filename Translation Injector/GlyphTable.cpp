#include "GlyphTable.h"
#include "utility.h"
#include <locale>
#include <codecvt>

const std::map<byte, std::wstring> GlyphTable::HIGH_BANK_CONTROL_CHARS = {
  { 0x74, L"[L1]" },
  { 0x75, L"[L2]" },
  { 0x76, L"[L3]" },
  { 0x73, L"[CR]" },
  { 0x7E, L"[LF]" },
  { HIGH_BANK_END_CHAR, L"[END]" },
};

GlyphTable::GlyphTable(const std::string& fname) {
  std::wifstream file(fname, std::ios::binary);
  if(!file) { throw std::runtime_error("GlyphTable::GlyphTable() - Failed to open file."); }
  file.imbue(Utility::getUTF16Locale());

  maxBytePhraseLength = 0;
  for(auto line : loadLines(file)) {
    auto kv = parseLine(line);
    tableToRom[kv.first] = kv.second;
    romToTable[kv.second] = kv.first;
    if(kv.second.size() > maxBytePhraseLength) { maxBytePhraseLength = kv.second.size(); }
  }
}

std::wstring GlyphTable::extractLoToken(ByteArray::const_iterator& iter, ByteArray::const_iterator tail) const {
  auto minIterMotion = iter + 1;

  size_t spanLength = tail - iter;
  if(spanLength > 3) { spanLength = 3; }
  tail = iter + spanLength;

  ByteArray bytes;
  while(iter != tail) {
    bytes.push_back(*iter++);
    if(romToTable.count(bytes)) {
      return romToTable.at(bytes);
    }
  }

  iter = minIterMotion;
  return Utility::stringf(L"[%02X]", bytes[0]);
}

std::wstring GlyphTable::extractHiToken(ByteArray::const_iterator& iter) const {
  ByteArray bytes = { *iter++ };
  if(HIGH_BANK_CONTROL_CHARS.count(bytes[0])) { return HIGH_BANK_CONTROL_CHARS.at(bytes[0]); }

  bytes.push_back(*iter++);
  if(bytes[0] == 0) { bytes = ByteArray{ bytes[1] }; }
  else if(bytes[0] == 1) { bytes[0] = SHIFT_CHAR; }

  auto str = operator[](bytes);

  if(str.empty()) {
    for(byte b : bytes) { str.append(Utility::stringf(L"[%02X]", b)); }
  }

  return str;
}

ByteArray GlyphTable::translateLoToken(std::wstringstream& is) const {
  std::wstring token;
  static const wchar_t eof = std::char_traits<wchar_t>::eof();

  wchar_t c = is.get();
  if(c == eof) { return ByteArray(); }
  token.push_back(c);

  if(token[0] == L'[') {
    while(token.back() != L']') {
      token.push_back(is.get());
    }
  }

  if(tableToRom.count(token)) { return tableToRom.at(token); }

  if(token[0] == L'[' && token.size() == 4) {
    return ByteArray{ (byte)wcstoul(token.c_str() + 1, nullptr, 16) };
  }

  throw std::runtime_error("translateLoToken() - Unrecognized token");
}

ByteArray GlyphTable::translateHiToken(std::wstringstream& is) const {
  std::wstring token;
  static const wchar_t eof = std::char_traits<wchar_t>::eof();

  wchar_t c = is.get();
  if(c == eof) { return ByteArray(); }
  token.push_back(c);

  if(token[0] == L'[') {
    while(token.back() != L']') {
      token.push_back(is.get());
    }

    for(auto kv : HIGH_BANK_CONTROL_CHARS) {
      if(token == kv.second) { return ByteArray{ kv.first }; }
    }

    if(token.size() == 4) {
      return ByteArray{ (byte)wcstoul(token.c_str() + 1, nullptr, 16) };
    }

    throw std::runtime_error("translateHiToken() - Unrecognized token");
  }

  if(!tableToRom.count(token)) { throw std::runtime_error("translateHiToken() - Unrecognized token"); }

  auto bytes = tableToRom.at(token);

  if(bytes.size() == 1) { return ByteArray{ 0 , bytes[0] }; }

  if(bytes.size() == 2 && (bytes[0] == SHIFT_CHAR)) {
    bytes[0] = 1;
    return bytes;
  }

  throw std::runtime_error("translateHiToken() - Unrecognized token");
}

ByteArray GlyphTable::operator[](const std::wstring& token) const {
  if(tableToRom.count(token)) { return tableToRom.at(token); }
  return ByteArray();
}

std::wstring GlyphTable::operator[](const ByteArray& bytes) const {
  if(romToTable.count(bytes)) { return romToTable.at(bytes); }
  return std::wstring();
}

size_t GlyphTable::getMaxBytePhraseLength() const {
  return maxBytePhraseLength;
}

std::wstring GlyphTable::loadToken(std::wstringstream& ss) {
  std::wstring token;
  wchar_t c = ss.get();
  token.push_back(c);

  while(true) {
    c = ss.get();
    if(c == L',' || c == L'\n' || c == std::char_traits<wchar_t>::eof()) { break; }
    token.push_back(c);
  }

  return token;
}

byte GlyphTable::loadByte(std::wstringstream& ss) {
  auto tok = loadToken(ss);
  return (byte)wcstoul(tok.c_str(), nullptr, 16);
}

std::pair<std::wstring, ByteArray> GlyphTable::parseLine(const std::wstring& line) {
  std::wstringstream ss(line);
  std::pair<std::wstring, ByteArray> retval;
  retval.first = loadToken(ss);
  while(ss.good()) { retval.second.push_back(loadByte(ss)); }
  return retval;
}

std::vector<std::wstring> GlyphTable::loadLines(std::wifstream& fs) {
  std::vector<std::wstring> lines;

  std::wstring buffer;
  while(std::getline(fs, buffer)) { lines.push_back(buffer); }

  return lines;
}

