#pragma once
#include <map>
#include "GlyphTable.h"

class SaveMenu {
public:
  SaveMenu(GlyphTable& glyphTable);

  enum class BankName {
    SAVE_MENU_COPY,
    SAVE_MENU_DELETE,
    COPY_MENU_HEADER,
    COPY_MENU_WHICH,
    COPY_MENU_WHERE,
    COPY_MENU_EXECUTE,
    COPY_MENU_CANCEL,
    DELETE_MENU_HEADER,
    DELETE_MENU_WHICH,
    DELETE_MENU_EXECUTE,
    DELETE_MENU_CANCEL
  };

  enum class Color {
    WHITE,
    YELLOW
  };

  std::wstring getString(BankName bank) const;
  size_t getMaxLength(BankName bank) const;
  Color getColor(BankName bank) const;
  void setString(BankName bank, const std::wstring& str);
  void setColor(BankName bank, Color color);

private:
  friend class ROM;

  struct Tile {
    uint16_t top;
    uint16_t bot;
  };

  void validateString(BankName bank, const std::wstring& str);
  byte getCharIndex(wchar_t c) const;
  Tile getTile(wchar_t c, Color color) const;
  static ByteArray toBytes(const std::vector<uint16_t>& src);
  std::map<size_t, ByteArray> dump() const;

  static constexpr uint16_t TOP_TILE_BASE_VALUE = 0x0C00;
  static constexpr uint16_t SPACE_ID = 0x0188;
  static constexpr Tile SPACE_TILE{ SPACE_ID, SPACE_ID };
  static constexpr uint16_t TOP_BOT_DIFF = 0x0010;

  struct BankInfo {
    size_t topRowOffset;
    size_t botRowOffset;
    size_t characters;
    Color defaultColor;
    std::wstring defaultString;
  };

  static const std::map<BankName, BankInfo> BANK_INFO;
  static const std::map<Color, uint16_t> COLOR_MASKS;

  std::map<BankName, std::wstring> strings;
  std::map<BankName, Color> colors;
  GlyphTable& gt;

};

