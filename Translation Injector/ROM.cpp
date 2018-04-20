#include "ROM.h"
#include <fstream>
#include "StringTable.h"
#include "GlyphTable.h"
#include "SaveMenu.h"

ROM::ROM(const std::string& fname) {
  loadData(fname);
  validateData();
  captureSeedString();
}

void ROM::saveAs(const std::string& fname) {
  std::ofstream file(fname, std::ios::binary);
  if(!file) { throw std::runtime_error("ROM::saveAs() - Failed to open/create ROM."); }

  file.write((char*)data.data(), data.size());
}

StringTable ROM::extractStringTable(const GlyphTable& glyphs) const {
  auto loBankBytes = extract(LOW_BANK_OFFSET, LOW_BANK_LENGTH);
  auto hiBankBytes = extract(HIGH_BANK_OFFSET, HIGH_BANK_LENGTH);
  return StringTable(glyphs, loBankBytes, hiBankBytes);
}

void ROM::injectStringTable(const GlyphTable& glyphs, const StringTable& table) {
  auto loBinTable = table.exportLoBank(glyphs);
  if(loBinTable.size() > LOW_BANK_LENGTH) { throw std::runtime_error("ROM::injectStringTable() - Compressed StringTable contents are still too large."); }

  inject(loBinTable, LOW_BANK_OFFSET);

  auto hiBinTable = table.exportHiBank(glyphs);
  if(hiBinTable.size() != HIGH_BANK_LENGTH) { throw std::runtime_error("ROM::injectStringTable() - High bank StringTable length mismatch"); }

  inject(hiBinTable, HIGH_BANK_OFFSET);
}

void ROM::injectSaveMenu(const SaveMenu& menu) {
  auto dump = menu.dump();
  for(auto kv : dump) {
    auto dst = data.begin() + kv.first;
    std::copy(kv.second.begin(), kv.second.end(), dst);
  }
}

void ROM::useAuxFontForWishes() {
  auto dst = data.begin() + WISHES_FONT_CALL_OFFSET;
  std::copy(std::begin(V29_KARKAT_FONT_LOAD_ADDR_BYTES), std::end(V29_KARKAT_FONT_LOAD_ADDR_BYTES), dst);
}

ROM::Type ROM::getType() const {
  return type;
}

std::string ROM::getSeed() const {
  return seed;
}

byte ROM::operator[](size_t offset) const {
  return data[offset];
}

ByteArray ROM::extract(size_t offset, size_t length) const {
  ByteArray bytes(length);
  auto a = data.begin() + offset;
  auto b = a + length;
  std::copy(a, b, bytes.begin());
  return bytes;
}

void ROM::inject(const ByteArray& table, size_t offset) {
  std::copy(table.begin(), table.end(), data.begin() + offset);
}

void ROM::loadData(const std::string& fname) {
  std::ifstream file(fname, std::ios::binary);
  if(!file) { throw std::runtime_error("ROM::loadData() - Failed to open ROM file."); }

  file.seekg(0, SEEK_END);
  data.resize((size_t)file.tellg());
  file.seekg(0, SEEK_SET);

  file.read((char*)data.data(), data.size());
}

void ROM::validateData() {
  if(data.size() != ROM_EXPECTED_LENGTH) { throw std::runtime_error("ROM::validateData() - Incorrect ROM size."); }

  auto a = data.begin() + STAMP_OFFSET;
  auto b = a + STAMP_LENGTH;

  std::string stamp(a, b);
  byte romType = *b;

  if(romType != HIROM_BYTE) { throw std::runtime_error("ROM::validateData() - This ROM doesn't appear to be a rando."); }

  if(stamp == VANILLA_STAMP) { type = Type::VANILLA; }
  else if(stamp.substr(0, strlen(TOURNEY_STAMP_PREFIX)) == TOURNEY_STAMP_PREFIX) { type = Type::TOURNEY; }
  else if(stamp.substr(0, strlen(NORMAL_STAMP_PREFIX) ) == NORMAL_STAMP_PREFIX ) { type = Type::NORMAL; }
  else { throw std::runtime_error("ROM::validateData() - Unrecognized ROM type."); }

  if(type == Type::NORMAL) {
    a = data.begin() + VERSION_OFFSET;
    b = a + strlen(EXPECTED_VERSION);
    if(!std::equal(a, b, EXPECTED_VERSION)) { throw std::runtime_error("ROM::validateData() - Rando version mismatch."); }
  }
}

void ROM::captureSeedString() {
  ByteArray::iterator a, b;

  switch(type) {
  case Type::VANILLA:
    seed = "VANILLA";
    break;
  case Type::NORMAL:
    a = data.begin() + NORMAL_SEED_OFFSET;
    b = a + NORMAL_SEED_LENGTH;
    seed = std::string(a, b);
    break;
  case Type::TOURNEY:
    a = data.begin() + TOURNEY_SEED_OFFSET;
    b = a + TOURNEY_SEED_LENGTH;
    seed = std::string(a, b);
    break;
  default:
    throw std::exception("ROM::captureSeedString() - Unhandled ROM type.");
  }
}
