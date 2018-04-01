#pragma once
#include "utility.h"
#include <unordered_map>

class GlyphTable;

class StringTable {
public:
  StringTable(const std::string& fname);

  enum class HiBankIndex {
    PEDESTAL,
    TRIFORCE,
    UNCLE,
    GANON1,
    GANON2,
    BLIND,
    PYRAMID_FAIRY,
    SAHASRALAH1,
    SAHASRALAH2,
    TAVERN_MAN,
    BOMB_SHOP1,
    BOMB_SHOP2,
    ETHER,
    BOMBOS,
    GANON_INVINCIBLE1,
    GANON_INVINCIBLE2
  };

  std::wstring& operator[](size_t i);
  const std::wstring& operator[](size_t i) const;
  std::wstring& operator[](HiBankIndex i);
  const std::wstring& operator[](HiBankIndex i) const;

  void saveAs(const std::string& fname) const;

private:
  std::vector<std::wstring> lines;
  std::vector<std::wstring> hiLines;
  static const std::unordered_map<HiBankIndex, size_t> hiBankIndices;

  friend class ROM;
  explicit StringTable(const GlyphTable& gt, const ByteArray& loData, const ByteArray& hiData);

  void loadLoData(const GlyphTable& gt, const ByteArray& data);
  void loadHiData(const GlyphTable& gt, const ByteArray& data);

  std::wstring unpackLoLine(const GlyphTable& gt, ByteArray::const_iterator& iter, ByteArray::const_iterator tail);
  ByteArray exportLoBank(const GlyphTable& gt) const;

  std::wstring unpackHiLine(const GlyphTable& gt, ByteArray::const_iterator iter);
  ByteArray exportHiBank(const GlyphTable& gt) const;

};

