#include <iostream>
#include <fstream>
#include <string>
#include <locale>
#include <codecvt>
#include <map>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cstdarg>

const std::locale UTF16LOCALE(std::locale::classic(), new std::codecvt_utf16<wchar_t, 0x10FFFF, std::consume_header>);
constexpr uint8_t UTF16BOM[] = { 0xFE, 0xFF };
constexpr size_t HANGUL_RANGE_BEGIN = 0xAC00;
constexpr size_t HANGUL_RANGE_END   = 0xD800;

void err(const std::string& msg) {
  std::cout << msg << std::endl;
  exit(-1);
}

std::wifstream genInFile(const std::string& fname) {
  std::wifstream infile(fname, std::ios::binary);
  if(!infile) { err("Could not open input file."); }
  infile.imbue(UTF16LOCALE);
  return infile;
}

std::wofstream genOutFile(const std::string& fname) {
  {
    std::ofstream prepFile(fname, std::ios::binary);
    if(!prepFile) { err("unable to create/open file for writing BOM"); }
    prepFile.write((char*)UTF16BOM, sizeof(UTF16BOM));
  }

  std::wofstream outFile(fname, std::ios::binary|std::ios::app);
  if(!outFile) { err("unable to reopen output file"); }
  outFile.imbue(UTF16LOCALE);

  return outFile;
}

std::map<wchar_t, int> doCount(std::wstringstream& stream) {
  std::map<wchar_t, int> glyphCount;

  wchar_t c;
  while(stream.good()) {
    stream >> c;
    if((c >= HANGUL_RANGE_BEGIN) && (c < HANGUL_RANGE_END)) {
      if(glyphCount.count(c) == 0) { glyphCount[c] = 1; }
      else { glyphCount[c]++; }
    }
  }

  return glyphCount;
}

/*
void writeReport(const std::string& inFileName, const std::string& outFileName) {
  auto counts = doCount(inFileName);

  std::vector<std::pair<wchar_t, int>> vec;
  for(auto kv : counts) { vec.push_back(kv); }

  std::sort(vec.begin(), vec.end(), [](std::pair<wchar_t, int> a, std::pair<wchar_t, int> b) { return a.second < b.second; });

  auto outFile = genOutFile(outFileName);
  outFile << L"Total unique glyphs used: " << vec.size() << L"\n\n";
  for(auto kv : vec) { outFile << kv.first << L" : " << kv.second << L"\n"; }
}
*/

bool sorter(const std::pair<std::wstring, std::vector<uint8_t>>& a, const std::pair<std::wstring, std::vector<uint8_t>>& b) {
  if(a.second[0] == b.second[0]) {
    if((a.second.size() < 2) || (b.second.size() < 2)) { throw std::runtime_error("bool sorter() - Glyph coding mismatch?"); }
    return a.second.back() < b.second.back();
  }
  return a.second[0] < b.second[0];
}

std::vector<std::pair<std::wstring, std::vector<uint8_t>>> createTable(std::map<wchar_t, int> counts) {
  constexpr int GLYPHS_PER_TABLE = 512;
  constexpr size_t RESERVED_RANGE_BEGIN = 0xF0;
  constexpr size_t RESERVED_RANGE_END   = 0x100;
  constexpr int RESERVED_SPACE = RESERVED_RANGE_END - RESERVED_RANGE_BEGIN;
  constexpr int MAX_CHARACTERS = GLYPHS_PER_TABLE - RESERVED_SPACE;
  if(counts.size() > MAX_CHARACTERS) { err("Too many characters for table."); }

  std::vector<std::pair<std::wstring, std::vector<uint8_t>>> table = {
    { L"0",                       { 0xA0 } },
    { L"1",                       { 0xA1 } },
    { L"2",                       { 0xA2 } },
    { L"3",                       { 0xA3 } },
    { L"4",                       { 0xA4 } },
    { L"5",                       { 0xA5 } },
    { L"6",                       { 0xA6 } },
    { L"7",                       { 0xA7 } },
    { L"8",                       { 0xA8 } },
    { L"9",                       { 0xA9 } },
    { L"A",                       { 0xAA } },
    { L"B",                       { 0xAB } },
    { L"C",                       { 0xAC } },
    { L"D",                       { 0xAD } },
    { L"E",                       { 0xAE } },
    { L"F",                       { 0xAF } },
    { L"G",                       { 0xB0 } },
    { L"H",                       { 0xB1 } },
    { L"I",                       { 0xB2 } },
    { L"J",                       { 0xB3 } },
    { L"K",                       { 0xB4 } },
    { L"L",                       { 0xB5 } },
    { L"M",                       { 0xB6 } },
    { L"N",                       { 0xB7 } },
    { L"O",                       { 0xB8 } },
    { L"P",                       { 0xB9 } },
    { L"Q",                       { 0xBA } },
    { L"R",                       { 0xBB } },
    { L"S",                       { 0xBC } },
    { L"T",                       { 0xBD } },
    { L"U",                       { 0xBE } },
    { L"V",                       { 0xBF } },
    { L"W",                       { 0xC0 } },
    { L"X",                       { 0xC1 } },
    { L"Y",                       { 0xC2 } },
    { L"Z",                       { 0xC3 } },
    { L"(",                       { 0xC4 } },
    { L")",                       { 0xC5 } },
    { L"?",                       { 0xC6 } },
    { L"!",                       { 0xC7 } },
    { L",",                       { 0xC8 } },
    { L"-",                       { 0xC9 } },
    { L"<",                       { 0xCA } },
    { L">",                       { 0xCB } },
    { L"_",                       { 0xCC } },
    { L".",                       { 0xCD } },
    { L"~",                       { 0xCE } },
    { L"@",                       { 0xD2 } },
    { L"#",                       { 0xD3 } },
    { L"'",                       { 0xD8 } },
    { L"$",                       { 0xDD } },
    { L"%",                       { 0xDE } },
    { L"^",                       { 0xDF } },
    { L"u",                       { 0xE0 } },
    { L"d",                       { 0xE1 } },
    { L"r",                       { 0xE2 } },
    { L"l",                       { 0xE3 } },
    { L"c",                       { 0xE4 } },
    { L"[PAGE]",                  { 0xF6 } },
    { L"[L1]",                    { 0xF7 } },
    { L"[L2]",                    { 0xF8 } },
    { L"[L3]",                    { 0xF9 } },
    { L"[KEYWAIT]",               { 0xFA } },
    { L"[END]",                   { 0xFB } },
    { L"[SPEED3]",                { 0xFC, 0x03 } },
    { L"[MARQUEE]",               { 0xFE, 0x06 } },
    { L"[SLIDE]",                 { 0xFE, 0x67 } },
    { L"[CHOICE]",                { 0xFE, 0x68 } },
    { L"[NAME]",                  { 0xFE, 0x6A } },
    { L"[NO BORDER]",             { 0xFE, 0x6B, 0x02 } },
    { L"[VAR0]",                  { 0xFE, 0x6C, 0x00 } },
    { L"[VAR1]",                  { 0xFE, 0x6C, 0x01 } },
    { L"[VAR2]",                  { 0xFE, 0x6C, 0x02 } },
    { L"[VAR3]",                  { 0xFE, 0x6C, 0x03 } },
    { L"[MOVE DIALOG TO BOTTOM]", { 0xFE, 0x6D, 0x01 } },
    { L"[START OF INTRO]",        { 0xFE, 0x6E, 0x00 } },
    { L"[INTRO TEXT PALETTE]",    { 0xFE, 0x77, 0x07 } },
    { L"[DELAY1]",                { 0xFE, 0x78, 0x01 } },
    { L"[DELAY3]",                { 0xFE, 0x78, 0x03 } },
    { L"[DELAY5]",                { 0xFE, 0x78, 0x05 } },
    { L"[DELAY7]",                { 0xFE, 0x78, 0x07 } },
    { L"[DELAY9]",                { 0xFE, 0x78, 0x09 } },
    { L"[SFX HARP]",              { 0xFE, 0x79, 0x2D } },
    { L" ",                       { 0xFF } },
  };

  std::vector<int> reservedIndices;
  for(auto kv : table) {
    reservedIndices.push_back(kv.second[0]);
  }
  std::unique(reservedIndices.begin(), reservedIndices.end());

  std::vector<std::pair<wchar_t, int>> sortingVec;
  for(auto kv : counts) { sortingVec.push_back(kv); }
  std::sort(sortingVec.begin(), sortingVec.end(), [](std::pair<wchar_t, int> a, std::pair<wchar_t, int> b) { return a.second > b.second; });

  std::wstring buffer = L" ";
  auto iter = sortingVec.begin();
  auto tail = sortingVec.end();

  uint8_t index = 0;
  while((index < RESERVED_RANGE_BEGIN) && (iter != tail)) {
    bool reserved = false;
    for(int res : reservedIndices) {
      if(res == index) {
        reserved = true;
        break;
      }
    }

    if(reserved) {
      index++;
      continue;
    }

    buffer[0] = iter++->first;
    table.emplace_back(std::make_pair(buffer, std::vector<uint8_t>{ index++ }));
  }

  index = 0;
  while(iter != tail) {
    buffer[0] = iter++->first;
    table.emplace_back(std::make_pair(buffer, std::vector<uint8_t>{ 0xFD, index++ }));
  }

  std::sort(table.begin(), table.end(), sorter);

  return table;
}

std::wstring stringf(const wchar_t* format, ...) {
  va_list args;
  va_start(args, format);
  std::vector<wchar_t> buffer(_vscwprintf(format, args) + 1);
  _vsnwprintf_s(buffer.data(), buffer.size(), buffer.size(), format, args);
  va_end(args);

  return buffer.data();
}

std::wstringstream collectToStream(const std::string& lowBankFname, const std::string& hiBankFname, const std::string& saveMenuFname) {
  auto low = genInFile(lowBankFname);
  auto high = genInFile(hiBankFname);
  auto menu = genInFile(saveMenuFname);

  std::wstringstream ss;
  ss << low.rdbuf();
  ss << high.rdbuf();

  //save menu glyphs MUST be in the lower range of the table,
  //so for now I'm just inflating their counts
  for(int i = 0; i < 6; i++) {
    ss << menu.rdbuf();
    if(ss.fail()) { err("derp"); }
    menu.seekg(0, SEEK_SET);
  }

  return ss;
}

void generateTableFile(const std::string& lowBankFname, const std::string& hiBankFname, const std::string& saveMenuFname, const std::string& outFileName) {
  auto ss = collectToStream(lowBankFname, hiBankFname, saveMenuFname);
  auto glyphCount = doCount(ss);
  
  auto data = createTable(glyphCount);
  auto outfile = genOutFile(outFileName);
  for(auto token : data) {
    outfile << token.first;
    for(auto b : token.second) { outfile << stringf(L",%02X", b); }
    outfile << L"\n";
  }
}

int main(int argc, char* argv[]) {
  generateTableFile("../Data/low banks.txt", "../Data/high bank texts.txt", "../Data/save menu texts.txt", "../Data/glyph table.txt");

}

