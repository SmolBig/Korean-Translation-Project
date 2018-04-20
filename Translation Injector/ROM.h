#pragma once
#include <string>
#include "utility.h"

class StringTable;
class GlyphTable;
class SaveMenu;

class ROM {
public:
  ROM(const std::string& fname);
  void saveAs(const std::string& fname);

  StringTable extractStringTable(const GlyphTable& glyphs) const;
  void injectStringTable(const GlyphTable& glyphs, const StringTable& table);
  void injectSaveMenu(const SaveMenu& menu);
  void useAuxFontForWishes();

  enum class Type {
    NORMAL,
    VANILLA,
    TOURNEY
  };

  Type getType() const;
  std::string getSeed() const;
  byte operator[](size_t offset) const;

  static constexpr size_t ROM_EXPECTED_LENGTH = 0x200000;

  static constexpr size_t STAMP_OFFSET = 0x7FC0;
  static constexpr size_t STAMP_LENGTH = 21;
  static constexpr char VANILLA_STAMP[] = "ZELDANODENSETSU      ";
  static constexpr char TOURNEY_STAMP_PREFIX[] = "VT TOURNEY ";
  static constexpr char NORMAL_STAMP_PREFIX[] = "VT";
  static constexpr byte HIROM_BYTE = 0x30;
  static constexpr size_t VERSION_OFFSET = 0x7FCC;
  static constexpr char EXPECTED_VERSION[] = "029";

  static constexpr size_t NORMAL_SEED_OFFSET = STAMP_OFFSET + 3;
  static constexpr size_t NORMAL_SEED_LENGTH = 9;
  static constexpr size_t TOURNEY_SEED_OFFSET = STAMP_OFFSET + 11;
  static constexpr size_t TOURNEY_SEED_LENGTH = 10;

  static constexpr size_t LOW_BANK_OFFSET = 0x0E0000;
  static constexpr size_t LOW_BANK_LENGTH = 0x8000;
  static constexpr size_t LOW_BANK_MAX_LINE_LENGTH = 0x8000;
  static constexpr size_t LOW_BANK_LINE_CT = 401;

  static constexpr size_t HIGH_BANK_OFFSET = 0x180300;
  static constexpr size_t HIGH_BANK_STRIDE = 0x100;
  static constexpr size_t HIGH_BANK_LINE_CT = 16;
  static constexpr size_t HIGH_BANK_LENGTH = HIGH_BANK_STRIDE * HIGH_BANK_LINE_CT;

  static constexpr size_t WISHES_FONT_CALL_OFFSET = 0x01057E;
  static constexpr byte V29_KARKAT_FONT_LOAD_ADDR_BYTES[] = { 0x4A, 0xB3, 0x23 }; //~~@ find the address manually

private:
  ByteArray data;
  std::string seed;
  Type type;

  ByteArray extract(size_t offset, size_t length) const;
  void inject(const ByteArray& table, size_t offset);
  void loadData(const std::string& fname);
  void validateData();
  void captureSeedString();

};

