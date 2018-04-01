#include "SaveMenu.h"

const std::map<SaveMenu::BankName, SaveMenu::BankInfo> SaveMenu::BANK_INFO = {
  { BankName::SAVE_MENU_COPY,      BankInfo{ 0x65E64, 0x65E7E, 11, Color::WHITE,  L"COPY"               }},
  { BankName::SAVE_MENU_DELETE,    BankInfo{ 0x65E98, 0x65EB6, 13, Color::WHITE,  L"DELETE"             }},
  { BankName::COPY_MENU_HEADER,    BankInfo{ 0x66228, 0x6624A, 15, Color::WHITE,  L"< COPY FILE >"      }},
  { BankName::COPY_MENU_WHICH,     BankInfo{ 0x662DB, 0x662ED,  7, Color::YELLOW, L"WHICH?"             }},
  { BankName::COPY_MENU_WHERE,     BankInfo{ 0x6639C, 0x663AE,  7, Color::YELLOW, L"WHERE?"             }},
  { BankName::COPY_MENU_EXECUTE,   BankInfo{ 0x6513A, 0x65150,  9, Color::WHITE,  L"COPY"               }},
  { BankName::COPY_MENU_CANCEL,    BankInfo{ 0x6629A, 0x662BA, 14, Color::WHITE,  L"CANCEL"             }},
  { BankName::DELETE_MENU_HEADER,  BankInfo{ 0x66002, 0x66028, 17, Color::WHITE,  L"< DELETE FILE >"    }},
  { BankName::DELETE_MENU_WHICH,   BankInfo{ 0x6604E, 0x66084, 25, Color::YELLOW, L"DELETE WHICH FILE?" }},
  { BankName::DELETE_MENU_EXECUTE, BankInfo{ 0x65328, 0x65344, 12, Color::WHITE,  L"DELETE"             }},
  { BankName::DELETE_MENU_CANCEL,  BankInfo{ 0x660BA, 0x660DA, 14, Color::WHITE,  L"CANCEL"             }}
};

const std::map<SaveMenu::Color, uint16_t> SaveMenu::COLOR_MASKS = {
  { Color::WHITE,  0x1000 },
  { Color::YELLOW, 0x0000 }
};

SaveMenu::SaveMenu(GlyphTable& glyphTable) : gt(glyphTable) {
  for(auto kv : BANK_INFO) {
    setString(kv.first, kv.second.defaultString);
    setColor(kv.first, kv.second.defaultColor);
  }
}

std::wstring SaveMenu::getString(BankName bank) const {
  return strings.at(bank);
}

size_t SaveMenu::getMaxLength(BankName bank) const {
  return BANK_INFO.at(bank).characters;
}

SaveMenu::Color SaveMenu::getColor(BankName bank) const {
  return colors.at(bank);
}

void SaveMenu::setString(BankName bank, const std::wstring& str) {
  validateString(bank, str);
  strings[bank] = str;
}

void SaveMenu::setColor(BankName bank, Color color) {
  colors[bank] = color;
}

void SaveMenu::validateString(BankName bank, const std::wstring& str) {
  if(str.length() > BANK_INFO.at(bank).characters) {
    throw std::runtime_error("SaveMenu::validateString() - Menu string is too long.");
  }
  for(auto c : str) {
    getCharIndex(c);
  }
}

byte SaveMenu::getCharIndex(wchar_t c) const {
  auto bytes = gt[std::wstring{c}];
  if(bytes.size() != 1) {
    throw std::runtime_error("SaveMenu::validateChar() - Character not valid for menu.");
  }
  return bytes[0];
}

SaveMenu::Tile SaveMenu::getTile(wchar_t c, Color color) const {
  if(c == L' ') { return SPACE_TILE; }

  byte b = getCharIndex(c);
  uint16_t row = b & 0b11110000;
  uint16_t col = b & 0b00001111;
  uint16_t offset = col + (row * 2);

  Tile t;
  t.top = offset + TOP_TILE_BASE_VALUE + COLOR_MASKS.at(color);
  t.bot = t.top + TOP_BOT_DIFF;

  return t;
}

ByteArray SaveMenu::toBytes(const std::vector<uint16_t>& src) {
  ByteArray bytes(src.size() * sizeof(uint16_t));
  memcpy(bytes.data(), src.data(), bytes.size());
  return bytes;
}

std::map<size_t, ByteArray> SaveMenu::dump() const {
  std::map<size_t, ByteArray> data;

  for(auto kv : BANK_INFO) {
    auto name = kv.first;
    auto info = kv.second;
    auto color = colors.at(name);
    auto str = strings.at(name);
    auto len = kv.second.characters;

    std::vector<uint16_t> top(len, SPACE_ID);
    std::vector<uint16_t> bot(len, SPACE_ID);

    for(size_t i = 0; i < str.size(); i++) {
      auto c = str[i];
      Tile t = getTile(c, color);
      top[i] = t.top;
      bot[i] = t.bot;
    }

    data[info.topRowOffset] = toBytes(top);
    data[info.botRowOffset] = toBytes(bot);
  }

  return data;
}

