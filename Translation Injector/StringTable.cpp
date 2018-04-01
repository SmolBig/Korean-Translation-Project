#include "StringTable.h"
#include "GlyphTable.h"
#include <locale>
#include <codecvt>
#include <fstream>
#include "ROM.h"

const std::unordered_map<StringTable::HiBankIndex, size_t> StringTable::hiBankIndices{
  { HiBankIndex::PEDESTAL,           0 },
  { HiBankIndex::TRIFORCE,           1 },
  { HiBankIndex::UNCLE,              2 },
  { HiBankIndex::GANON1,             3 },
  { HiBankIndex::GANON2,             4 },
  { HiBankIndex::BLIND,              5 },
  { HiBankIndex::PYRAMID_FAIRY,      6 },
  { HiBankIndex::SAHASRALAH1,        7 },
  { HiBankIndex::SAHASRALAH2,        8 },
  { HiBankIndex::TAVERN_MAN,         9 },
  { HiBankIndex::BOMB_SHOP1,        11 },
  { HiBankIndex::BOMB_SHOP2,        10 },
  { HiBankIndex::ETHER,             12 },
  { HiBankIndex::BOMBOS,            13 },
  { HiBankIndex::GANON_INVINCIBLE1, 14 },
  { HiBankIndex::GANON_INVINCIBLE2, 15 }
};

StringTable::StringTable(const std::string& fname) {
  std::wifstream file(fname, std::ios::binary);
  if(!file) { throw std::runtime_error("StringTable::loadFromTableFile() - Failed to open string table file."); }
  file.imbue(Utility::getUTF16Locale());

  while(file.good() && lines.size() < ROM::LOW_BANK_LINE_CT) {
    std::wstring str;
    std::getline(file, str);
    if(str.empty()) { throw std::runtime_error("StringTable::StringTable(fname) - Encountered empty line before file end"); }
    lines.push_back(str);
  }

  if(lines.size() != ROM::LOW_BANK_LINE_CT) {
    throw std::runtime_error("StringTable::StringTable(file) - Incorrect low-bank line count");
  }

  while(file.good() && hiLines.size() < ROM::HIGH_BANK_LINE_CT) {
    std::wstring str;
    std::getline(file, str);
    if(!str.empty()) { hiLines.push_back(str); }
  }

  if(hiLines.size() != ROM::HIGH_BANK_LINE_CT) {
    throw std::runtime_error("StringTable::StringTable(file) - Incorrect high-bank line count");
  }
}

std::wstring& StringTable::operator[](size_t i) {
 return lines[i];
}

const std::wstring& StringTable::operator[](size_t i) const {
  return lines[i];
}

std::wstring& StringTable::operator[](HiBankIndex i) {
  return hiLines[hiBankIndices.at(i)];
}

const std::wstring& StringTable::operator[](HiBankIndex i) const {
  return hiLines[hiBankIndices.at(i)];
}

void StringTable::saveAs(const std::string& fname) const {
  std::wofstream file(fname, std::ios::binary|std::ios::app);
  if(!file) { throw std::runtime_error("StringTable::saveAs() - unable to create/open file"); }
  file.imbue(Utility::getUTF16Locale());
  file.write(&Utility::UTF16BOM, sizeof(Utility::UTF16BOM));

  for(auto& line : lines) {
    file << line << L"\n";
  }

  for(auto& line : hiLines) {
    file << line << L"\n";
  }

}

StringTable::StringTable(const GlyphTable& gt, const ByteArray& loData, const ByteArray& hiData) {
  loadLoData(gt, loData);
  loadHiData(gt, hiData);
}

void StringTable::loadLoData(const GlyphTable& gt, const ByteArray& data) {
  for(auto iter = data.begin(); iter != data.end(); ) {
    lines.emplace_back(unpackLoLine(gt, iter, data.end()));
    if(lines.size() == ROM::LOW_BANK_LINE_CT) { break; }
  }
}

void StringTable::loadHiData(const GlyphTable& gt, const ByteArray& data) {
  for(auto iter = data.begin(); iter != data.end(); iter += ROM::HIGH_BANK_STRIDE) {
    auto line = unpackHiLine(gt, iter);
    hiLines.push_back(line);
    if(hiLines.size() == ROM::HIGH_BANK_LINE_CT) { break; }
  }
}

std::wstring StringTable::unpackLoLine(const GlyphTable& gt, ByteArray::const_iterator& iter, ByteArray::const_iterator tail) {
  std::wstring str;
  while(iter != tail) {
    auto token = gt.extractLoToken(iter, tail);
    Utility::append(str, token);
    if(token == L"[END]") { return str; }
  }

  //there's usually unused space at the end of the
  //block, and if we arrive here that's probably why
  return std::wstring();
}

ByteArray StringTable::exportLoBank(const GlyphTable& gt) const {
  std::wstringstream ss;
  for(auto line : lines) { ss << line; }

  ByteArray data;
  while(ss.good()) {
    auto bytes = gt.translateLoToken(ss);
    Utility::append(data, bytes);
  }

  return data;
}

std::wstring StringTable::unpackHiLine(const GlyphTable& gt, ByteArray::const_iterator iter) {
  ByteArray::const_iterator tail = iter + ROM::HIGH_BANK_STRIDE;

  std::wstring str;
  while(iter != tail) {
    auto next = gt.extractHiToken(iter);
    str.append(next);
    if(next == L"[END]") { return str; }
  }

  throw std::runtime_error("StringTable::unpackHiLine() - Unterminated string.");
}

ByteArray StringTable::exportHiBank(const GlyphTable& gt) const {
  std::vector<ByteArray> packedLines;
  for(const auto& line : hiLines) {
    packedLines.emplace_back();

    std::wstringstream ss(line);
    while(ss.good()) {
      auto bytes = gt.translateHiToken(ss);
      Utility::append(packedLines.back(), bytes);
    }

    if(packedLines.back().size() > ROM::HIGH_BANK_STRIDE) { throw std::runtime_error("StringTable::exportHiBanks() - A line is too long to fit in the bank."); }
  }

  ByteArray block(ROM::HIGH_BANK_LENGTH, 0);
  auto iter = block.begin();
  for(const auto& line : packedLines) {
    std::copy(line.cbegin(), line.cend(), iter);
    iter += ROM::HIGH_BANK_STRIDE;
  }

  return block;
}

