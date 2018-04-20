#define NOMINMAX

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <limits>
#include "ImageCodec.h"

const std::string UINCODE_GLYPHS_FOLDER = "../Data/Unicode Tables/";
const std::string GLYPH_ORDER_FILE = "../Data/glyph table.txt";
const std::string OUTPUT_FONT_PATH = "../Data/hangul font.png";
const std::map<uint16_t, std::string> UNICODE_TABLE_FILES = {
  { 0xAC00, "unicode 0xAC00-0xAFFF.png" },
  { 0xB000, "unicode 0xB000-0xB3FF.png" },
  { 0xB400, "unicode 0xB400-0xB7FF.png" },
  { 0xB800, "unicode 0xB800-0xBBFF.png" },
  { 0xBC00, "unicode 0xBC00-0xBFFF.png" },
  { 0xC000, "unicode 0xC000-0xC3FF.png" },
  { 0xC400, "unicode 0xC400-0xC7FF.png" },
  { 0xC800, "unicode 0xC800-0xCBFF.png" },
  { 0xCC00, "unicode 0xCC00-0xCFFF.png" },
  { 0xD000, "unicode 0xD000-0xD3FF.png" },
  { 0xD400, "unicode 0xD400-0xD7FF.png" },
  { 0x0000, "unicode 0x0000-0x03FF.png" }
};

std::vector<uint16_t> loadGlyphOrder(const std::string& fname) {
  std::ifstream file(fname, std::ifstream::binary);
  if(!file) { throw std::runtime_error("loadFile() - File could not be opened."); }

  constexpr size_t UNICODE_HEADER_LENGTH = 2;

  file.seekg(0, file.end);
  size_t len = (int)file.tellg() - UNICODE_HEADER_LENGTH;
  file.seekg(UNICODE_HEADER_LENGTH, file.beg);

  std::vector<uint16_t> data;
  data.resize(len / sizeof(uint16_t));
  file.read((char*)&data[0], len);

  file.close();

  std::vector<uint16_t> glyphs{ data[0] };
  auto iter = data.begin();
  auto tail = data.end();
  while(++iter != tail) {
    if(*iter == 0x0A00) {
      if(++iter == tail) { break; }
      if(*iter != 0x5B00) { glyphs.push_back(*iter); }
    }
  }

  std::transform(glyphs.begin(), glyphs.end(), glyphs.begin(), [](uint16_t a) -> uint16_t { return (a >> 8)|((a << 8) & 0xFF00); });

  return glyphs;
}

constexpr size_t GLYPH_WIDTH  = 11;
constexpr size_t GLYPH_HEIGHT = 16;
constexpr size_t GLYPH_TEXEL_CT = GLYPH_WIDTH * GLYPH_HEIGHT;

void copyGlyph(const uint32_t* srcHead, const size_t srcWidth, uint32_t* dstHead, const size_t dstWidth) {
  for(size_t y = 0; y < GLYPH_HEIGHT; y++) {
    memcpy(dstHead, srcHead, GLYPH_WIDTH * sizeof(uint32_t));
    srcHead += srcWidth;
    dstHead += dstWidth;
  }
}

constexpr size_t INDEX_BLOCK_LENGTH = 0x0400;

uint16_t findTableIndex(uint16_t cc) {
  size_t prev = 0;
  for(size_t i = INDEX_BLOCK_LENGTH; i <= std::numeric_limits<uint16_t>::max(); i += INDEX_BLOCK_LENGTH) {
    if(cc < i) { return (uint16_t)prev; }
    prev = i;
  }

  throw std::logic_error("findTableIndex() - Unable to determine source table index. This should never happen.");
}

size_t findUnicodeTableGlyphTexelOffset(uint16_t glyphIndex) {
  constexpr size_t CELL_WIDTH = GLYPH_WIDTH + 1;
  constexpr size_t CELL_HEIGHT = GLYPH_HEIGHT + 1;
  constexpr size_t GLYPHS_PER_ROW = 16;
  constexpr size_t GLYPHS_PER_BLOCK = 256;
  constexpr size_t BLOCK_PX_HEIGHT = 17 * CELL_HEIGHT;
  constexpr size_t TABLE_PX_WIDTH = 17 * CELL_WIDTH;
  constexpr size_t TEXELS_PER_BLOCK = BLOCK_PX_HEIGHT * TABLE_PX_WIDTH;
  constexpr size_t TEXELS_PER_ROW = CELL_HEIGHT * TABLE_PX_WIDTH;

  size_t texel = 0;

  //skip unused blocks
  int block = glyphIndex >> 8;
  glyphIndex &= 0xFF;
  int row = 1 + (glyphIndex >> 4);
  int col = 1 + (glyphIndex & 0x0F);

  texel += (block * TEXELS_PER_BLOCK);
  texel += (row * TEXELS_PER_ROW);
  texel += (col * CELL_WIDTH);

  return texel;
}

Bitmap getGlyph(uint16_t cc, const std::map<uint16_t, Bitmap>& tables) {
  const uint16_t tableIndex = findTableIndex(cc);
  const uint16_t glyphIndex = cc - tableIndex;

  const Bitmap& src = tables.at(tableIndex);

  Bitmap bmp{ GLYPH_WIDTH, GLYPH_HEIGHT, std::vector<uint32_t>(GLYPH_TEXEL_CT) };

  auto offset = findUnicodeTableGlyphTexelOffset(glyphIndex);
  copyGlyph(src.data.data() + offset, src.width, bmp.data.data(), bmp.width);

  return bmp;
}

size_t getFontTexelOffset(size_t glyphIndex) {
  //~~@ magic numbers
  int row = glyphIndex / 32;
  int col = glyphIndex % 32;

  int x = col * 12;
  int y = row * 17;

  return x + (y * 384);
}

int main() {
  std::map<uint16_t, Bitmap> unicodeTables;
  for(auto kv : UNICODE_TABLE_FILES) {
    unicodeTables[kv.first] = ImageCodec::loadFromFile(UINCODE_GLYPHS_FOLDER + kv.second);
  }

  auto glyphOrder = loadGlyphOrder(GLYPH_ORDER_FILE);
  std::map<uint16_t, Bitmap> glyphs;
  for(auto g : glyphOrder) {
    glyphs[g] = getGlyph(g, unicodeTables);
  }

  constexpr size_t GLYPHS_PER_ROW = 32;
  constexpr size_t TOTAL_ROWS = 16;
  constexpr size_t TOTAL_CELLS = TOTAL_ROWS * GLYPHS_PER_ROW;
  constexpr size_t CELL_WIDTH = GLYPH_WIDTH + 1;
  constexpr size_t CELL_HEIGHT = GLYPH_HEIGHT + 1;
  constexpr size_t FONT_BMP_WIDTH = GLYPHS_PER_ROW * CELL_WIDTH;
  constexpr size_t FONT_BMP_HEIGHT = TOTAL_ROWS * CELL_HEIGHT;
  constexpr size_t FONT_TEXELS = FONT_BMP_WIDTH * FONT_BMP_HEIGHT;
  constexpr uint32_t GREEN = 0xFF00FF00;

  Bitmap font {
    FONT_BMP_WIDTH,
    FONT_BMP_HEIGHT,
    std::vector<uint32_t>(FONT_TEXELS, GREEN)
  };

  const auto& emptyGlyph = glyphs[L' '];
  for(size_t i = 0; i < TOTAL_CELLS; i++) {
    size_t offset = getFontTexelOffset(i);
    copyGlyph(emptyGlyph.data.data(), emptyGlyph.width, font.data.data() + offset, font.width);
  }

  for(size_t i = 0; i < glyphOrder.size(); i++) {
    const auto& glyph = glyphs[glyphOrder[i]];
    auto index = i;
    if(i >= 0xF0) { index += 16; }
    if(index >= 0x200) { throw std::runtime_error("Too many glyphs."); }
    size_t offset = getFontTexelOffset(index);
    copyGlyph(glyph.data.data(), glyph.width, font.data.data() + offset, font.width);
  }

  ImageCodec::saveToFile(OUTPUT_FONT_PATH, font);

}





