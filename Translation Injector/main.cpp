﻿#include <iostream>
#include "ROM.h"
#include "GlyphTable.h"
#include "StringTable.h"
#include "Translator.h"
#include "SaveMenu.h"

void showErr(const std::exception& e);
bool checkArgs(int argc, char** argv);

int main(int argc, char** argv) {
#ifdef NDEBUG
  try {
#endif
    //if(checkArgs(argc, argv)) { return -1; }

    //ROM rom(argv[1]);
    ROM rom("../Data/fonted rando.sfc");

    GlyphTable gt("../Data/glyph table.txt");

    Translator tl(gt, rom);
    auto st = tl.translate();
    rom.injectStringTable(gt, st);

    SaveMenu menu(gt);
    menu.setString(SaveMenu::BankName::SAVE_MENU_COPY,      L"복 사 하 기");
    menu.setString(SaveMenu::BankName::SAVE_MENU_DELETE,    L"삭 제 하 기");
    menu.setString(SaveMenu::BankName::COPY_MENU_HEADER,    L"< 파 일 복 사 하 기 >");
    menu.setString(SaveMenu::BankName::COPY_MENU_WHICH,     L"어 느 걸 ?");
    menu.setString(SaveMenu::BankName::COPY_MENU_WHERE,     L"어 디 에 ?");
    menu.setString(SaveMenu::BankName::COPY_MENU_EXECUTE,   L"복 사 하 기");
    menu.setString(SaveMenu::BankName::COPY_MENU_CANCEL,    L"어 머 실 수");
    menu.setString(SaveMenu::BankName::DELETE_MENU_HEADER,  L"< 파 일 삭 제 하 기 >");
    menu.setString(SaveMenu::BankName::DELETE_MENU_WHICH,   L"어 느  파 일 을 ?");
    menu.setString(SaveMenu::BankName::DELETE_MENU_EXECUTE, L"삭 제 하 기");
    menu.setString(SaveMenu::BankName::DELETE_MENU_CANCEL,  L"어 머 실 수");
    rom.injectSaveMenu(menu);

    rom.useAuxFontForWishes();

    tl.keysanityFixup(rom, gt);

    //rom.saveAs(argv[2]);
    rom.saveAs("../derp.sfc");

#ifdef NDEBUG
  } catch(const std::exception& e) { showErr(e); }
#endif
}

#include <Windows.h>

void showErr(const std::exception& e) {
  MessageBoxA(0, e.what(), 0, 0);
}

bool checkArgs(int argc, char** argv) {
  if(argc == 3) { return false; }
  std::cout << "Usage:\n\tktrans [input rom] [output rom]\n\n";
  return true;
}

