#include "Translator.h"
#include <sstream>

const std::unordered_map<Translator::LockedLocation, size_t> Translator::LOCKED_LOCATION_ADDRESSES = {
  { LockedLocation::PEDESTAL, 0x0289B0 },
  { LockedLocation::ETHER,    0x180016 },
  { LockedLocation::BOMBOS,   0x180017 }
};

const std::unordered_map<Translator::Item, std::wstring> Translator::STATIC_ITEM_NAMES = {
  { Item::NOTHING,                         L"공란" },
  { Item::L1_SWORD,                        L"전사의 검" },
  { Item::SWORD_AND_SHIELD,                L"전사의 검과 방패" },
  { Item::L2_SWORD,                        L"마스터 소드" },
  { Item::MASTER_SWORD,                    L"마스터 소드" },
  { Item::L3_SWORD,                        L"달궈진 검" },
  { Item::L4_SWORD,                        L"골든 소드" },
  { Item::BLUE_SHIELD,                     L"파란 방패" },
  { Item::RED_SHIELD,                      L"불 방패" },
  { Item::MIRROR_SHIELD,                   L"거울 방패" },
  { Item::FIRE_ROD,                        L"불의 지팡이" },
  { Item::ICE_ROD,                         L"얼음의 지팡이" },
  { Item::HAMMER,                          L"망치" },
  { Item::HOOKSHOT,                        L"훅샷" },
  { Item::BOW,                             L"활" },
  { Item::BOOMERANG,                       L"부메랑" },
  { Item::MAGIC_POWDER,                    L"마법의 가루" },
  { Item::BEE_REQUIRING_BOTTLE,            L"벌" },
  { Item::BOMBOS_MEDALLION,                L"봄보스" },
  { Item::ETHER_MEDALLION,                 L"에테르" },
  { Item::QUAKE_MEDALLION,                 L"셰이크" },
  { Item::LAMP,                            L"등불" },
  { Item::SHOVEL,                          L"삽" },
  { Item::FLUTE_INACTIVE,                  L"오카리나 비활성화" },
  { Item::CANE_OF_SOMARIA,                 L"소마리아의 지팡이" },
  { Item::EMPTY_BOTTLE,                    L"병" },
  { Item::HEART_PIECE,                     L"하트 조각" },
  { Item::CANE_OF_BYRNA,                   L"바이라의 지팡이" },
  { Item::MAGIC_CAPE,                      L"망토" },
  { Item::MAGIC_MIRROR,                    L"마법의 거울" },
  { Item::POWER_GLOVE,                     L"파워 글러브" },
  { Item::TITANS_MITT,                     L"거인의 장갑" },
  { Item::BOOK,                            L"무드라의 책" },
  { Item::FLIPPERS,                        L"물갈퀴" },
  { Item::MOON_PEARL,                      L"달의 진주" },
  { Item::BUG_CATCHING_NET,                L"잠자리채" },
  { Item::BLUE_MAIL,                       L"파란 갑옷" },
  { Item::RED_MAIL,                        L"빨간 갑옷" },
  { Item::SMALL_KEY,                       L"작은 열쇠" },
  { Item::COMPASS,                         L"나침반" },
  { Item::HEART_CONTAINER_NO_ANIMATION,    L"하트 컨테이너" },
  { Item::BOMB,                            L"폭탄" },
  { Item::BOMBS_3,                         L"폭탄 세 개" },
  { Item::MUSHROOM,                        L"버섯" },
  { Item::MAGICAL_BOOMERANG,               L"매직 부메랑" },
  { Item::RED_POTION_IN_BOTTLE,            L"빨간 물약이 담긴 병" },
  { Item::GREEN_POTION_IN_BOTTLE,          L"녹색 물약이 담긴 병" },
  { Item::BLUE_POTION_IN_BOTTLE,           L"파란 물약이 담긴 병" },
  { Item::BOMBS_10,                        L"폭탄 열 개" },
  { Item::BIG_KEY,                         L"큰 열쇠" },
  { Item::MAP,                             L"지도" },
  { Item::RUPEE,                           L"루피 한 개" },
  { Item::RUPEES_5,                        L"루피 다섯 개" },
  { Item::RUPEES_20,                       L"루피 스무 개" },
  { Item::BOW_AND_ARROWS,                  L"활과 화살" },
  { Item::BOW_AND_SILVER_ARROWS,           L"활과 은 화살" },
  { Item::BEE_IN_BOTTLE,                   L"벌이 담긴 병" },
  { Item::FAIRY_IN_BOTTLE,                 L"요정이 담긴 병" },
  { Item::BOSS_HEART_CONTAINER,            L"하트 컨테이너" },
  { Item::SANCTUARY_HEART_CONTAINER,       L"하트 컨테이너" },
  { Item::RUPEES_100,                      L"루피 백 개" },
  { Item::RUPEES_50,                       L"루피 오십 개" },
  { Item::HEART,                           L"하트" },
  { Item::ARROW,                           L"화살" },
  { Item::ARROWS_10,                       L"화살 열 개" },
  { Item::SMALL_MAGIC,                     L"작은 마나" },
  { Item::RUPEES_300,                      L"루피 삼백 개" },
  { Item::RUPEES_20_OTHER,                 L"루피 스무 개" },
  { Item::GOLDEN_BEE_IN_BOTTLE,            L"금 벌이 담긴 병" },
  { Item::FLUTE_ACTIVE,                    L"오카리나 활성화" },
  { Item::PEGASUS_BOOTS,                   L"페가수스의 신발" },
  { Item::BOMB_UPGRADE_5,                  L"폭탄 강화 5" },
  { Item::BOMB_UPGRADE_10,                 L"폭탄 강화 10" },
  { Item::BOMB_UPGRADE_50,                 L"폭탄 강화 50" },
  { Item::ARROW_UPGRADE_5,                 L"화살 강화 5" },
  { Item::ARROW_UPGRADE_10,                L"화살 강화 10" },
  { Item::ARROW_UPGRADE_70,                L"화살 강화 70" },
  { Item::HALF_MAGIC,                      L"마나 두 배" },
  { Item::QUARTER_MAGIC,                   L"마나 네 배" },
  { Item::SILVER_ARROWS,                   L"은 화살" },
  { Item::RUPOOR,                          L"검은 루피" },
  { Item::RED_CLOCK,                       L"빨간 망토" },
  { Item::BLUE_CLOCK,                      L"파란 망토" },
  { Item::GREEN_CLOCK,                     L"녹색 망토" },
  { Item::PROGRESSIVE_SWORD,               L"검" },
  { Item::PROGRESSIVE_SHIELD,              L"방패" },
  { Item::PROGRESSIVE_ARMOR,               L"갑옷" },
  { Item::PROGRESSIVE_GLOVE,               L"장갑" },
  { Item::TRIFORCE,                        L"트라이포스" },
  { Item::POWER_STAR,                      L"파워스타" },
  { Item::TRIFORCE_PIECE,                  L"트라이포스 조각" }
};

const std::unordered_map<Translator::Item, byte> Translator::ITEM_IDS = {
  { Item::NOTHING,                         0x5A },
  { Item::L1_SWORD,                        0x49 },
  { Item::SWORD_AND_SHIELD,                0x00 },
  { Item::L2_SWORD,                        0x01 },
  { Item::MASTER_SWORD,                    0x50 },
  { Item::L3_SWORD,                        0x02 },
  { Item::L4_SWORD,                        0x03 },
  { Item::BLUE_SHIELD,                     0x04 },
  { Item::RED_SHIELD,                      0x05 },
  { Item::MIRROR_SHIELD,                   0x06 },
  { Item::FIRE_ROD,                        0x07 },
  { Item::ICE_ROD,                         0x08 },
  { Item::HAMMER,                          0x09 },
  { Item::HOOKSHOT,                        0x0A },
  { Item::BOW,                             0x0B },
  { Item::BOOMERANG,                       0x0C },
  { Item::MAGIC_POWDER,                    0x0D },
  { Item::BEE_REQUIRING_BOTTLE,            0x0E },
  { Item::BOMBOS_MEDALLION,                0x0F }, //
  { Item::ETHER_MEDALLION,                 0X10 }, // These have multiple bytes in the php source.
  { Item::QUAKE_MEDALLION,                 0x11 }, //
  { Item::LAMP,                            0x12 },
  { Item::SHOVEL,                          0x13 },
  { Item::FLUTE_INACTIVE,                  0x14 },
  { Item::CANE_OF_SOMARIA,                 0x15 },
  { Item::EMPTY_BOTTLE,                    0x16 },
  { Item::HEART_PIECE,                     0x17 },
  { Item::CANE_OF_BYRNA,                   0x18 },
  { Item::MAGIC_CAPE,                      0x19 },
  { Item::MAGIC_MIRROR,                    0x1A },
  { Item::POWER_GLOVE,                     0x1B },
  { Item::TITANS_MITT,                     0x1C },
  { Item::BOOK,                            0x1D },
  { Item::FLIPPERS,                        0x1E },
  { Item::MOON_PEARL,                      0x1F },
  { Item::BUG_CATCHING_NET,                0x21 },
  { Item::BLUE_MAIL,                       0x22 },
  { Item::RED_MAIL,                        0x23 },
  { Item::SMALL_KEY,                       0x24 },
  { Item::COMPASS,                         0x25 },
  { Item::HEART_CONTAINER_NO_ANIMATION,    0x26 },
  { Item::BOMB,                            0x27 },
  { Item::BOMBS_3,                         0x28 },
  { Item::MUSHROOM,                        0x29 },
  { Item::MAGICAL_BOOMERANG,               0x2A },
  { Item::RED_POTION_IN_BOTTLE,            0x2B },
  { Item::GREEN_POTION_IN_BOTTLE,          0x2C },
  { Item::BLUE_POTION_IN_BOTTLE,           0x2D },
  { Item::BOMBS_10,                        0x31 },
  { Item::BIG_KEY,                         0x32 },
  { Item::MAP,                             0x33 },
  { Item::RUPEE,                           0x34 },
  { Item::RUPEES_5,                        0x35 },
  { Item::RUPEES_20,                       0x36 },
  { Item::BOW_AND_ARROWS,                  0x3A },
  { Item::BOW_AND_SILVER_ARROWS,           0x3B },
  { Item::BEE_IN_BOTTLE,                   0x3C },
  { Item::FAIRY_IN_BOTTLE,                 0x3D },
  { Item::BOSS_HEART_CONTAINER,            0x3E },
  { Item::SANCTUARY_HEART_CONTAINER,       0x3F },
  { Item::RUPEES_100,                      0x40 },
  { Item::RUPEES_50,                       0x41 },
  { Item::HEART,                           0x42 },
  { Item::ARROW,                           0x43 },
  { Item::ARROWS_10,                       0x44 },
  { Item::SMALL_MAGIC,                     0x45 },
  { Item::RUPEES_300,                      0x46 },
  { Item::RUPEES_20_OTHER,                 0x47 },
  { Item::GOLDEN_BEE_IN_BOTTLE,            0x48 },
  { Item::FLUTE_ACTIVE,                    0x4A },
  { Item::PEGASUS_BOOTS,                   0x4B },
  { Item::BOMB_UPGRADE_5,                  0x51 },
  { Item::BOMB_UPGRADE_10,                 0x52 },
  { Item::BOMB_UPGRADE_50,                 0x4C },
  { Item::ARROW_UPGRADE_5,                 0x53 },
  { Item::ARROW_UPGRADE_10,                0x54 },
  { Item::ARROW_UPGRADE_70,                0x4D },
  { Item::HALF_MAGIC,                      0x4E },
  { Item::QUARTER_MAGIC,                   0x4F },
  { Item::SILVER_ARROWS,                   0x58 },
  { Item::RUPOOR,                          0x59 },
  { Item::RED_CLOCK,                       0x5B },
  { Item::BLUE_CLOCK,                      0x5C },
  { Item::GREEN_CLOCK,                     0x5D },
  { Item::PROGRESSIVE_SWORD,               0x5E },
  { Item::PROGRESSIVE_SHIELD,              0x5F },
  { Item::PROGRESSIVE_ARMOR,               0x60 },
  { Item::PROGRESSIVE_GLOVE,               0x61 },
  { Item::TRIFORCE,                        0x6A },
  { Item::POWER_STAR,                      0x6B },
  { Item::TRIFORCE_PIECE,                  0x6C },
  { Item::MAP_OF_LIGHT_WORLD,              0x70 },
  { Item::MAP_OF_DARK_WORLD,               0x71 },
  { Item::MAP_OF_GANONS_TOWER,             0x72 },
  { Item::MAP_OF_TURTLE_ROCK,              0x73 },
  { Item::MAP_OF_THIEVES_TOWN,             0x74 },
  { Item::MAP_OF_TOWER_OF_HERA,            0x75 },
  { Item::MAP_OF_ICE_PALACE,               0x76 },
  { Item::MAP_OF_SKULL_WOODS,              0x77 },
  { Item::MAP_OF_MISERY_MIRE,              0x78 },
  { Item::MAP_OF_PALACE_OF_DARKNESS,       0x79 },
  { Item::MAP_OF_SWAMP_PALACE,             0x7A },
  { Item::MAP_OF_AGAHNIMS_TOWER,           0x7B },
  { Item::MAP_OF_DESERT_PALACE,            0x7C },
  { Item::MAP_OF_EASTERN_PALACE,           0x7D },
  { Item::MAP_OF_HYRULE_CASTLE,            0x7E },
  { Item::MAP_OF_SEWERS,                   0x7F },
  { Item::COMPASS_OF_GANONS_TOWER,         0x82 },
  { Item::COMPASS_OF_TURTLE_ROCK,          0x83 },
  { Item::COMPASS_OF_THIEVES_TOWN,         0x84 },
  { Item::COMPASS_OF_TOWER_OF_HERA,        0x85 },
  { Item::COMPASS_OF_ICE_PALACE,           0x86 },
  { Item::COMPASS_OF_SKULL_WOODS,          0x87 },
  { Item::COMPASS_OF_MISERY_MIRE,          0x88 },
  { Item::COMPASS_OF_PALACE_OF_DARKNESS,   0x89 },
  { Item::COMPASS_OF_SWAMP_PALACE,         0x8A },
  { Item::COMPASS_OF_AGAHNIMS_TOWER,       0x8B },
  { Item::COMPASS_OF_DESERT_PALACE,        0x8C },
  { Item::COMPASS_OF_EASTERN_PALACE,       0x8D },
  { Item::COMPASS_OF_HYRULE_CASTLE,        0x8E },
  { Item::COMPASS_OF_SEWERS,               0x8F },
  { Item::BIG_KEY_OF_GANONS_TOWER,         0x92 },
  { Item::BIG_KEY_OF_TURTLE_ROCK,          0x93 },
  { Item::BIG_KEY_OF_THIEVES_TOWN,         0x94 },
  { Item::BIG_KEY_OF_TOWER_OF_HERA,        0x95 },
  { Item::BIG_KEY_OF_ICE_PALACE,           0x96 },
  { Item::BIG_KEY_OF_SKULL_WOODS,          0x97 },
  { Item::BIG_KEY_OF_MISERY_MIRE,          0x98 },
  { Item::BIG_KEY_OF_PALACE_OF_DARKNESS,   0x99 },
  { Item::BIG_KEY_OF_SWAMP_PALACE,         0x9A },
  { Item::BIG_KEY_OF_AGAHNIMS_TOWER,       0x9B },
  { Item::BIG_KEY_OF_DESERT_PALACE,        0x9C },
  { Item::BIG_KEY_OF_EASTERN_PALACE,       0x9D },
  { Item::BIG_KEY_OF_HYRULE_CASTLE,        0x9E },
  { Item::BIG_KEY_OF_SEWERS,               0x9F },
  { Item::SMALL_KEY_OF_GANONS_TOWER,       0xAD },
  { Item::SMALL_KEY_OF_TURTLE_ROCK,        0xAC },
  { Item::SMALL_KEY_OF_THIEVES_TOWN,       0xAB },
  { Item::SMALL_KEY_OF_TOWER_OF_HERA,      0xAA },
  { Item::SMALL_KEY_OF_ICE_PALACE,         0xA9 },
  { Item::SMALL_KEY_OF_SKULL_WOODS,        0xA8 },
  { Item::SMALL_KEY_OF_MISERY_MIRE,        0xA7 },
  { Item::SMALL_KEY_OF_PALACE_OF_DARKNESS, 0xA6 },
  { Item::SMALL_KEY_OF_SWAMP_PALACE,       0xA5 },
  { Item::SMALL_KEY_OF_AGAHNIMS_TOWER,     0xA4 },
  { Item::SMALL_KEY_OF_DESERT_PALACE,      0xA3 },
  { Item::SMALL_KEY_OF_EASTERN_PALACE,     0xA2 },
  { Item::SMALL_KEY_OF_HYRULE_CASTLE,      0xA1 },
  { Item::SMALL_KEY_OF_SEWERS,             0xA0 }
};

const std::unordered_map<Translator::Region, std::wstring> Translator::REGION_NAMES = {
  { Region::EASTERN_PALACE,     L"동쪽 신전" }, 
  { Region::DESERT_PALACE,      L"사막의 신전" },
  { Region::TOWER_OF_HERA,      L"헤라의 탑" },
  { Region::DEATH_MOUNTAIN,     L"죽음의 산" },
  { Region::HYRULE_CASTLE,      L"하이랄 성" },
  { Region::LIGHT_WORLD,        L"빛의 세계" },
  { Region::PALACE_OF_DARKNESS, L"어둠의 신전" },
  { Region::SWAMP_PALACE,       L"물의 신전" },
  { Region::SKULL_WOODS,        L"해골 숲" },
  { Region::THIEVES_TOWN,       L"버려진 자들의 아지트" },
  { Region::ICE_PALACE,         L"얼음의 신전" },
  { Region::MISERY_MIRE,        L"절망의 수렁" },
  { Region::TURTLE_ROCK,        L"거북바위" },
  { Region::GANONS_TOWER,       L"가논의 탑" },
  { Region::DARK_WORLD,         L"어둠의 세계" },
  { Region::AGAHNIMS_TOWER,     L"하이랄 성 탑" },
  { Region::SEWERS,             L"비밀 통로" }
};

const std::unordered_map<Translator::Region, std::vector<size_t>> Translator::REGIONS = {
  { Region::EASTERN_PALACE,     { 0x00E98F, 0x00E9B6, 0x180160, 0x00E9C2, 0x00E9CB, 0x180151 } },
  { Region::DESERT_PALACE,      { 0x00E977, 0x00E97D, 0x00E9B3, 0x00E9B9, 0x00E9F5, 0x180150 } },
  { Region::TOWER_OF_HERA,      { 0x00E9E6, 0x180162, 0x00E9AD, 0x00E9FB, 0x00E9F8, 0x180152 } },
  { Region::DEATH_MOUNTAIN,     { 0x0F69FA, 0x180002, 0x180016, 0x180140, 0x00E9BF, 0x00E9C5, 0x00EB2A, 0x00EB2D, 0x00EB30, 0x00EB33, 0x00EB36, 0x00EB39, 0x00EB3C, 0x180141 } },
  { Region::HYRULE_CASTLE,      { 0x00EAB5, 0x00EAB2, 0x00EA79, 0x00E974, 0x00EB0C, 0x00EB09, 0x00EB5D, 0x00EB60, 0x00EB63, 0x00E96E } },
  { Region::LIGHT_WORLD,        { 0x0289B0, 0x02DF45, 0x00E971, 0x00E97A, 0x00E98C, 0x00E9BC, 0x00E9CE, 0x00E9E9, 0x00E9F2, 0x00EA82, 0x00EA85, 0x00EA88, 0x00EA8E, 0x00EA91, 0x00EA94, 0x00EA97, 0x00EA9A, 0x00EB0F, 0x00EB12, 0x00EB15, 0x00EB18, 0x00EB1B, 0x00EB3F, 0x00EB42, 0x00EB45, 0x00EB48, 0x00EB4B, 0x00EB4E, 0x02EB18, 0x02F1FC, 0x180015, 0x0339CF, 0x033E7D, 0x180017, 0x0EE1C3, 0x180000, 0x180001, 0x180003, 0x180004, 0x180005, 0x180010, 0x180012, 0x180013, 0x180014, 0x180142, 0x180143, 0x180144, 0x180145, 0x180149, 0x18014A, 0x00E9B0, 0x00E9D1 } },
  { Region::PALACE_OF_DARKNESS, { 0x00EA5B, 0x00EA37, 0x00EA3A, 0x00EA3D, 0x00EA49, 0x00EA52, 0x00EA40, 0x00EA43, 0x00EA46, 0x00EA4C, 0x00EA4F, 0x00EA55, 0x00EA58, 0x180153 } },
  { Region::SWAMP_PALACE,       { 0x00EA9D, 0x00E989, 0x00EAA6, 0x00E986, 0x00EAA3, 0x00EAA0, 0x00EAA9, 0x00EAAC, 0x00EAAF, 0x180154 } },
  { Region::SKULL_WOODS,        { 0x00E998, 0x00E99E, 0x00E992, 0x00E99B, 0x00E9FE, 0x00E9A1, 0x00E9C8, 0x180155 } },
  { Region::THIEVES_TOWN,       { 0x00EA0D, 0x00EA04, 0x00EA01, 0x00EA07, 0x00EA0A, 0x00EA10, 0x00EA13, 0x180156 } },
  { Region::ICE_PALACE,         { 0x00E9A4, 0x00E9D4, 0x00E9DD, 0x00E9E0, 0x00E995, 0x00E9E3, 0x00E9AA, 0x180157 } },
  { Region::MISERY_MIRE,        { 0x00EA67, 0x00EA5E, 0x00EA6D, 0x00EA64, 0x00EA61, 0x00EA6A, 0x00E9DA, 0x180158 } },
  { Region::TURTLE_ROCK,        { 0x00EA16, 0x00EA19, 0x00EA1C, 0x00EA1F, 0x00EA22, 0x00EA25, 0x00EA28, 0x00EA2B, 0x00EA2E, 0x00EA31, 0x00EA34, 0x180159 } },
  { Region::GANONS_TOWER,       { 0x180161, 0x00EAB8, 0x00EABB, 0x00EABE, 0x00EAC1, 0x00EAC4, 0x00EAC7, 0x00EACA, 0x00EACD, 0x00EAD0, 0x00EAD3, 0x00EAD6, 0x00EAD9, 0x00EADC, 0x00EADF, 0x00EAE2, 0x00EAE5, 0x00EAE8, 0x00EAEB, 0x00EAEE, 0x00EAF1, 0x00EAF4, 0x00EAF7, 0x00EAFD, 0x00EB00, 0x00EB03, 0x00EB06 } },
  { Region::DARK_WORLD,         { 0x00EB1E, 0x00EB21, 0x00EB24, 0x00EB27, 0x0330C7, 0x180011, 0x180148, 0x00E9EC, 0x00E9EF, 0x00EDA8, 0x180006, 0x180146, 0x18002A, 0x03355C, 0x033D68, 0x0EE185, 0x180147, 0x180028, 0x034914, 0x00EA73, 0x00EA76, 0x00EA8B, 0x00EA7C, 0x00EA7F, 0x00EB51, 0x00EB54, 0x00EB57, 0x00EB5A } }
};

const std::unordered_map<Translator::Region, size_t> Translator::PRIZES = {
  { Region::EASTERN_PALACE,     0x053EF8 },
  { Region::DESERT_PALACE,      0x053F1C },
  { Region::TOWER_OF_HERA,      0x053F0A },
  { Region::PALACE_OF_DARKNESS, 0x053F00 },
  { Region::SWAMP_PALACE,       0x053F6C },
  { Region::SKULL_WOODS,        0x053F12 },
  { Region::THIEVES_TOWN,       0x053F36 },
  { Region::ICE_PALACE,         0x053F5A },
  { Region::MISERY_MIRE,        0x053F48 },
  { Region::TURTLE_ROCK,        0x053F24 }
};

Translator::Translator(const GlyphTable& gt, const ROM& rom) : gt(gt), rom(rom), romST(rom.extractStringTable(gt)), rng(std::hash<std::string>()(rom.getSeed())), REVERSED_ITEM_IDS(reverseItemIDs()), ITEM_NAMES(composeItemNames()) {
  identifyKeyItemLocations();
  identifyKeyPrizeLocations();
  identifyPedTabItems();
}

StringTable Translator::translate() {
  StringTable st(LOW_BANK_TABLE_PATH);

  st[StringTable::HiBankIndex::PEDESTAL] = pedestalLine();
  st[StringTable::HiBankIndex::TRIFORCE] = triforceLine();
  st[StringTable::HiBankIndex::UNCLE] = uncleLine();
  st[StringTable::HiBankIndex::GANON1] = ganon1Line();
  st[StringTable::HiBankIndex::GANON2] = ganon2Line();
  st[StringTable::HiBankIndex::BLIND] = blindLine();
  st[StringTable::HiBankIndex::PYRAMID_FAIRY] = pyramidFairyLine();
  st[StringTable::HiBankIndex::SAHASRALAH1] = sahasralah1Line();
  st[StringTable::HiBankIndex::SAHASRALAH2] = sahasralah2Line();
  st[StringTable::HiBankIndex::TAVERN_MAN] = tavernLine();
  st[StringTable::HiBankIndex::BOMB_SHOP1] = bombShop1Line();
  st[StringTable::HiBankIndex::BOMB_SHOP2] = bombShop2Line();
  st[StringTable::HiBankIndex::ETHER] = etherLine();
  st[StringTable::HiBankIndex::BOMBOS] = bombosLine();
  st[StringTable::HiBankIndex::GANON_INVINCIBLE1] = invincible1Line();
  st[StringTable::HiBankIndex::GANON_INVINCIBLE2] = invincible2Line();

  return st;
}

void Translator::identifyKeyItemLocations() {
  for(auto kv : REGIONS) {
    for(auto location : kv.second) {
      byte id = rom[location];
      switch(REVERSED_ITEM_IDS.at(id)) {
      case Item::PEGASUS_BOOTS:         keyLocations[KeyLocation::BOOTS]         = kv.first; break;
      case Item::BOW_AND_SILVER_ARROWS: keyLocations[KeyLocation::SILVER_ARROWS] = kv.first; break;
      case Item::SILVER_ARROWS:         keyLocations[KeyLocation::SILVER_ARROWS] = kv.first; break;
      }
    }
  }
}

void Translator::identifyKeyPrizeLocations() {
  bool foundFirstCrystal = false;
  for(auto kv : PRIZES) {
    ByteArray regionBytes{ rom[kv.second], rom[kv.second + 1] };
    if(Utility::ezEqual(GREEN_PENDANT_BYTES, regionBytes)) {
      keyLocations[KeyLocation::GREEN_PENDANT] = kv.first;
    }
    else if(Utility::ezEqual(RED_CRYSTAL_BYTES, regionBytes)) {
      KeyLocation index = foundFirstCrystal ? KeyLocation::RED_CRYSTAL_B : KeyLocation::RED_CRYSTAL_A;
      keyLocations[index] = kv.first;
      foundFirstCrystal = true;
    }
  }
}

void Translator::identifyPedTabItems() {
  identifyLockedLocation(LockedLocation::PEDESTAL);
  identifyLockedLocation(LockedLocation::ETHER);
  identifyLockedLocation(LockedLocation::BOMBOS);
}

std::unordered_map<byte, Translator::Item> Translator::reverseItemIDs() {
  std::unordered_map<byte, Translator::Item> rev;
  for(auto kv : ITEM_IDS) {
    rev[kv.second] = kv.first;
  }
  return rev;
}

std::unordered_map<Translator::Item, std::wstring> Translator::composeItemNames() {
  auto names = STATIC_ITEM_NAMES;

  static const std::wstring MAP_SUFFIX = L"의 지도";
  static const std::wstring COMPASS_SUFFIX = L"의 나침반";
  static const std::wstring BIG_KEY_SUFFIX = L"의 큰 열쇠";
  static const std::wstring SMALL_KEY_SUFFIX = L"의 작은 열쇠";

  names[Item::MAP_OF_LIGHT_WORLD]        = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::LIGHT_WORLD).c_str(), MAP_SUFFIX.c_str());
  names[Item::MAP_OF_DARK_WORLD]         = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::DARK_WORLD).c_str(), MAP_SUFFIX.c_str());
  names[Item::MAP_OF_GANONS_TOWER]       = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::GANONS_TOWER).c_str(), MAP_SUFFIX.c_str());
  names[Item::MAP_OF_TURTLE_ROCK]        = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::TURTLE_ROCK).c_str(), MAP_SUFFIX.c_str());
  names[Item::MAP_OF_THIEVES_TOWN]       = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::THIEVES_TOWN).c_str(), MAP_SUFFIX.c_str());
  names[Item::MAP_OF_TOWER_OF_HERA]      = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::TOWER_OF_HERA).c_str(), MAP_SUFFIX.c_str());
  names[Item::MAP_OF_ICE_PALACE]         = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::ICE_PALACE).c_str(), MAP_SUFFIX.c_str());
  names[Item::MAP_OF_SKULL_WOODS]        = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::SKULL_WOODS).c_str(), MAP_SUFFIX.c_str());
  names[Item::MAP_OF_MISERY_MIRE]        = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::MISERY_MIRE).c_str(), MAP_SUFFIX.c_str());
  names[Item::MAP_OF_PALACE_OF_DARKNESS] = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::PALACE_OF_DARKNESS).c_str(), MAP_SUFFIX.c_str());
  names[Item::MAP_OF_SWAMP_PALACE]       = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::SWAMP_PALACE).c_str(), MAP_SUFFIX.c_str());
  names[Item::MAP_OF_AGAHNIMS_TOWER]     = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::AGAHNIMS_TOWER).c_str(), MAP_SUFFIX.c_str());
  names[Item::MAP_OF_DESERT_PALACE]      = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::DESERT_PALACE).c_str(), MAP_SUFFIX.c_str());
  names[Item::MAP_OF_EASTERN_PALACE]     = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::EASTERN_PALACE).c_str(), MAP_SUFFIX.c_str());
  names[Item::MAP_OF_HYRULE_CASTLE]      = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::HYRULE_CASTLE).c_str(), MAP_SUFFIX.c_str());
  names[Item::MAP_OF_SEWERS]             = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::SEWERS).c_str(), MAP_SUFFIX.c_str());
  names[Item::COMPASS_OF_GANONS_TOWER]       = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::GANONS_TOWER).c_str(), COMPASS_SUFFIX.c_str());
  names[Item::COMPASS_OF_TURTLE_ROCK]        = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::TURTLE_ROCK).c_str(), COMPASS_SUFFIX.c_str());
  names[Item::COMPASS_OF_THIEVES_TOWN]       = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::THIEVES_TOWN).c_str(), COMPASS_SUFFIX.c_str());
  names[Item::COMPASS_OF_TOWER_OF_HERA]      = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::TOWER_OF_HERA).c_str(), COMPASS_SUFFIX.c_str());
  names[Item::COMPASS_OF_ICE_PALACE]         = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::ICE_PALACE).c_str(), COMPASS_SUFFIX.c_str());
  names[Item::COMPASS_OF_SKULL_WOODS]        = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::SKULL_WOODS).c_str(), COMPASS_SUFFIX.c_str());
  names[Item::COMPASS_OF_MISERY_MIRE]        = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::MISERY_MIRE).c_str(), COMPASS_SUFFIX.c_str());
  names[Item::COMPASS_OF_PALACE_OF_DARKNESS] = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::PALACE_OF_DARKNESS).c_str(), COMPASS_SUFFIX.c_str());
  names[Item::COMPASS_OF_SWAMP_PALACE]       = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::SWAMP_PALACE).c_str(), COMPASS_SUFFIX.c_str());
  names[Item::COMPASS_OF_AGAHNIMS_TOWER]     = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::AGAHNIMS_TOWER).c_str(), COMPASS_SUFFIX.c_str());
  names[Item::COMPASS_OF_DESERT_PALACE]      = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::DESERT_PALACE).c_str(), COMPASS_SUFFIX.c_str());
  names[Item::COMPASS_OF_EASTERN_PALACE]     = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::EASTERN_PALACE).c_str(), COMPASS_SUFFIX.c_str());
  names[Item::COMPASS_OF_HYRULE_CASTLE]      = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::HYRULE_CASTLE).c_str(), COMPASS_SUFFIX.c_str());
  names[Item::COMPASS_OF_SEWERS]             = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::SEWERS).c_str(), COMPASS_SUFFIX.c_str());
  names[Item::BIG_KEY_OF_GANONS_TOWER]       = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::GANONS_TOWER).c_str(), BIG_KEY_SUFFIX.c_str());
  names[Item::BIG_KEY_OF_TURTLE_ROCK]        = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::TURTLE_ROCK).c_str(), BIG_KEY_SUFFIX.c_str());
  names[Item::BIG_KEY_OF_THIEVES_TOWN]       = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::THIEVES_TOWN).c_str(), BIG_KEY_SUFFIX.c_str());
  names[Item::BIG_KEY_OF_TOWER_OF_HERA]      = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::TOWER_OF_HERA).c_str(), BIG_KEY_SUFFIX.c_str());
  names[Item::BIG_KEY_OF_ICE_PALACE]         = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::ICE_PALACE).c_str(), BIG_KEY_SUFFIX.c_str());
  names[Item::BIG_KEY_OF_SKULL_WOODS]        = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::SKULL_WOODS).c_str(), BIG_KEY_SUFFIX.c_str());
  names[Item::BIG_KEY_OF_MISERY_MIRE]        = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::MISERY_MIRE).c_str(), BIG_KEY_SUFFIX.c_str());
  names[Item::BIG_KEY_OF_PALACE_OF_DARKNESS] = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::PALACE_OF_DARKNESS).c_str(), BIG_KEY_SUFFIX.c_str());
  names[Item::BIG_KEY_OF_SWAMP_PALACE]       = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::SWAMP_PALACE).c_str(), BIG_KEY_SUFFIX.c_str());
  names[Item::BIG_KEY_OF_AGAHNIMS_TOWER]     = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::AGAHNIMS_TOWER).c_str(), BIG_KEY_SUFFIX.c_str());
  names[Item::BIG_KEY_OF_DESERT_PALACE]      = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::DESERT_PALACE).c_str(), BIG_KEY_SUFFIX.c_str());
  names[Item::BIG_KEY_OF_EASTERN_PALACE]     = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::EASTERN_PALACE).c_str(), BIG_KEY_SUFFIX.c_str());
  names[Item::BIG_KEY_OF_HYRULE_CASTLE]      = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::HYRULE_CASTLE).c_str(), BIG_KEY_SUFFIX.c_str());
  names[Item::BIG_KEY_OF_SEWERS]             = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::SEWERS).c_str(), BIG_KEY_SUFFIX.c_str());
  names[Item::SMALL_KEY_OF_GANONS_TOWER]       = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::GANONS_TOWER).c_str(), SMALL_KEY_SUFFIX.c_str());
  names[Item::SMALL_KEY_OF_TURTLE_ROCK]        = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::TURTLE_ROCK).c_str(), SMALL_KEY_SUFFIX.c_str());
  names[Item::SMALL_KEY_OF_THIEVES_TOWN]       = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::THIEVES_TOWN).c_str(), SMALL_KEY_SUFFIX.c_str());
  names[Item::SMALL_KEY_OF_TOWER_OF_HERA]      = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::TOWER_OF_HERA).c_str(), SMALL_KEY_SUFFIX.c_str());
  names[Item::SMALL_KEY_OF_ICE_PALACE]         = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::ICE_PALACE).c_str(), SMALL_KEY_SUFFIX.c_str());
  names[Item::SMALL_KEY_OF_SKULL_WOODS]        = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::SKULL_WOODS).c_str(), SMALL_KEY_SUFFIX.c_str());
  names[Item::SMALL_KEY_OF_MISERY_MIRE]        = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::MISERY_MIRE).c_str(), SMALL_KEY_SUFFIX.c_str());
  names[Item::SMALL_KEY_OF_PALACE_OF_DARKNESS] = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::PALACE_OF_DARKNESS).c_str(), SMALL_KEY_SUFFIX.c_str());
  names[Item::SMALL_KEY_OF_SWAMP_PALACE]       = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::SWAMP_PALACE).c_str(), SMALL_KEY_SUFFIX.c_str());
  names[Item::SMALL_KEY_OF_AGAHNIMS_TOWER]     = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::AGAHNIMS_TOWER).c_str(), SMALL_KEY_SUFFIX.c_str());
  names[Item::SMALL_KEY_OF_DESERT_PALACE]      = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::DESERT_PALACE).c_str(), SMALL_KEY_SUFFIX.c_str());
  names[Item::SMALL_KEY_OF_EASTERN_PALACE]     = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::EASTERN_PALACE).c_str(), SMALL_KEY_SUFFIX.c_str());
  names[Item::SMALL_KEY_OF_HYRULE_CASTLE]      = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::HYRULE_CASTLE).c_str(), SMALL_KEY_SUFFIX.c_str());
  names[Item::SMALL_KEY_OF_SEWERS]             = Utility::stringf(L"%s%s", REGION_NAMES.at(Region::SEWERS).c_str(), SMALL_KEY_SUFFIX.c_str());

  return names;                                
}

bool Translator::isUncleSpoiler(const std::wstring& uncleLine) {
  static const std::vector<std::wstring> SPOILER_LINES = {
    L"[L1]LONK![L2]",
    L"[L1]BOOTS AT ",
    L"[L1]LONK! BOOTS"
  };

  for(auto& line : SPOILER_LINES) {
    if(Utility::ezEqual(line, uncleLine)) { return true; }
  }

  return false;
}

std::wstring Translator::lockedItemLine(LockedLocation location) {
  auto itemName = ITEM_NAMES.at(pedTabItems.at(location));
  return Utility::stringf(L"[L1]%s[END]", itemName.c_str());
}

void Translator::identifyLockedLocation(LockedLocation location) {
  size_t addr = LOCKED_LOCATION_ADDRESSES.at(location);
  byte itemByte = rom[addr];
  Item item = REVERSED_ITEM_IDS.at(itemByte);
  pedTabItems[location] = item;
}

std::wstring Translator::pedestalLine() {
  return lockedItemLine(LockedLocation::PEDESTAL);
}

std::wstring Translator::triforceLine() {
  static const std::vector<std::wstring> LINES = {
    L"[L1]오직 모니카만.",
    L"[L1]GG",
    L"[L1]웅아, 도착했어! 포켓몬[L2]리그장이야![END]",
    L"[L1]네 기지 모두 다 내[L2]거다요.[END]",
    L"[L1]고마워요 마리오![L2]공주는 다른 성에 있어요[END]",
    L"[L1]Dr. 와일리를[L2]저지했습니다.[END]",
    L"[L1]삭제해라 애송이.[END]",
    L"[L1]예측대로 하니까[L2]이기던데요.[END]",
    L"[L1]다들 채팅 쳐 EE![END]",
    L"[L1]한 판 더 할까?[END]",
    L"[L1]이번 시드 재미있었죠?[END]",
    L"[L1]지금이야! 채팅쳐![END]",
    L"[L1]와 삼위일체가 이제[L2]나오나요.[END]",
    L"[L1]야아아아아아! 야아아![L2]야아아![END]",
    L"[L1]야옹 야옹 야옹 야옹[L2]야옹 야옹 야옹.[END]"
  };

  return Utility::sample(LINES, rng);
}

std::wstring Translator::generateUncleSpoiler() {
  auto region = keyLocations.at(KeyLocation::BOOTS);
  auto location = REGION_NAMES.at(region);
  return Utility::stringf(L"[L1]신발은[L2]%s에[L3]숨겨져 있습니다.[END]", location.c_str());
}

std::wstring Translator::uncleLine() {
  std::wstring uncleLine = romST[StringTable::HiBankIndex::UNCLE];
  if(isUncleSpoiler(uncleLine)) { return generateUncleSpoiler(); }

  static const std::vector<std::wstring> UNCLE_LINES = {
    L"[L1]파 껍질은 바나나 껍질보다[L2]더 미끄럽다.[END]",
    L"[L1]저 농구가 하고 싶어요.[END]",
    L"[L1]아! 한 시 강의 들을걸.[END]",
    L"[L1]내 발은 눈보다 빠르다.[END]",
    L"[L1]양말 한 짝 잃어버렸어.[END]",
    L"[L1]마술 하나 보여줄까?[END]",
    L"[L1]고양이 꼬리는 말랑말랑.[END]",
    L"[L1]고구마 한 봉지[L2]39900원.[END]",
    L"[L1]카드 두 장 합쳐서 21.[END]",
    L"[L1]고기 연골 버섯 조림[L2]제조법은 비밀.[END]",
    L"[L1]나는 이제 백색의[L2]간달프다.[END]",
    L"[L1]해산물 시장에서 소[L2]꼬리 사 오세요.[END]",
    L"[L1]죠니가 왔다![END]",
    L"[L1]가즈아아아아아아아아.[END]",
    L"[L1]안심하세요. 병원이에요.[END]",
    L"[L1]화장실 가고 싶어.[END]",
    L"[L1]잠자기 전에 네 검 닦는[L2]거 잊지 마.[END]",
    L"[L1]미역이랑 치즈랑 먹으면[L2]맛있을까?[END]",
    L"[L1]물약이 비싸다면 도토리를[L2]먹으렴.[END]",
    L"[L1]내 저금통엔 모노폴리가[L2]들어 있단다.[END]",
    L"[L1]젤다 공주는 내 것이야![END]",
    L"[L1]포켓몬 데려올게.[END]",
    L"[L1]늦기 전에 들어오렴.[END]",
    L"[L1]올 때 메로나.[END]",
    L"[L1]싱크대에 도토리묵 있단다.[END]",
    L"[L1]모험이 우릴 기다린다![END]",
    L"[L1]도토리![END]",
    L"[L1]곰 젤리가 다 떨어졌어.[END]",
    L"[L1]내가 죽으면 오카리나를[L2]불어줘.[END]",
    L"[L1]나는 지금 거짓말을 하고[L2]있어.[END]",
    L"[L1]오늘은 여기까지.[END]",
    L"[L1]직박구리 폴더는[L2]열지 마렴.[END]",
    L"[L1]성까지 시합하자![END]",
    L"[L1]담배 한 대 피우고 오마.[END]",
    L"[L1]난 사실 네 아빠가[L2]아니야.[END]"
  };

  return Utility::sample(UNCLE_LINES, rng);
}

std::wstring Translator::ganon1Line() {
  static const std::vector<std::wstring> GANON_LINES = {
    L"[L1]영양 많고 맛있는 우리들의[L2]아침, 언제나 시리얼로[L3]아침을 깨우세요![END]",
    L"[L1]넌 내 또 다른 모습인[L2]아가님을 두 번이나[L3]물리쳤지만, 이번엔 내가[LF][CR][L3]너를 무찌르겠다![END]",
    L"[L1]임파 말에 의하면 네 손의[L2]문장이 네가 젤다 공주를[L3]깨울 영웅이라는 걸[LF][CR]의미한다더라. 내가[CR][L3]환생하려면 네 피가[CR][L3]필요해.[END]",
    L"[L1]내 옆에 있지마 내 옆에[L2]가까이 있지마 내 옆에[L3]제발 내 옆에[LF][CR][L3]가까이 있지마[END]",
    L"[L1]하루 사이에 모든 사람이[L2]내 너절한 농담에 웃음을[L3]터뜨리기 시작했다.[END]",
    L"[L1]달팽이는 다른 연체동물처럼[L2]머리에는 늘었다 줄었다[L3]하는 뿔처럼 생긴 두 쌍의[LF][CR]촉각이 있다.[END]",
    L"[L1]오빠, 나 오늘 머리[L2]3밀리 커트했는데 그것도[L3]몰라? 헤어져![END]",
    L"[L1]난 네 아빠의 형제의[L2]조카에 사촌 예전집에 살던[L3]사람이다. 그게 무슨[LF][CR]상관이냐고?[END]",
    L"[L1]게임 알지도 못하는[L2]놈들아. 너네들이 와서 함[L3]해볼래?![END]",
    L"[L1]야식 때문이 아니라면[L2]냉장고에 불은 왜[L3]있는 거야?[END]",
    L"[L1]왠지 이 일 여러 번[L2]있었던 것 같아. 안[L3]그래? 완전 데자뷔라니까.[END]",
    L"[L1]세상에서 가장 크고 무거운[L2]치즈는 57518파운드에[L3]32피트나 길었대.[END]",
    L"[L1]만약에 말야 우리 조금[L2]어렸었다면 지금[L3]어땠었을까. 만약에 말야[LF][CR][L3]우리 지금 이럴 거라면[CR][L3]후회하진 않을까.[END]",
    L"[L1]포도당을 최우선적인[L2]에너지원으로 쓰기 때문에[L3]장기간의 단식은 악영향을[LF][CR}끼칠 수 있다.[END]"
  };

  return Utility::sample(GANON_LINES, rng);
}

std::wstring Translator::ganon2Line() {
  Region region = keyLocations.at(KeyLocation::SILVER_ARROWS);
  std::wstring location = REGION_NAMES.at(region);
  return Utility::stringf(L"[L1]%s에[L2]있는 은 화살은[L3]얻으셨나요?[END]", location.c_str());
}

std::wstring Translator::blindLine() {
  static const std::vector<std::wstring> BLIND_LINES = {
    L"[L1]음식에 다시다를 넣으면[L2]다 시다.[END]",
    L"[L1]너 죽을 준비해! 난[L2]밥을 준비할게.[END]",
    L"[L1]물고기의 반대말은 불고기[END]",
    L"[L1]우리 사이다 마신 사이다[END]",
    L"[L1]형돈이 형 돈 좀 줘[END]",
    L"[L1]바나나를 먹으면[L2]나한테 바나나?[END]",
    L"[L1]고로케가 고로케 맛있나?[END]",
    L"[L1]알파카로 만든 파카는[L2]알파카 파카.[END]",
    L"[L1]알파카 파카를 팔면[L2]알파카 파카 파카[END]",
    L"[L1]드립을 드립니다.[END]",
    L"[L1]망고가 얼망고[END]",
    L"[L1]마나가 마나도 너무나 마나[END]",
    L"[L1]내가 그린 기린 구름 기린[L2]그림 참 잘 그린 기린[L3]구름 그림[END]",
    L"[L1]사자를 사자[END]",
    L"[L1]판다를 판다[END]",
    L"[L1]차이나 가면 차이나[END]",
    L"[L1]아주머니 집이 아주 머니?[END]",
    L"[L1]칠판 지우개로 칠판 지우개[END]",
    L"[L1]나 지금 가야대 가야대[END]",
    L"[L1]시립대 가면 손이 시립대[END]",
    L"[L1]자라야 발 닦고 자라[END]"
  };

  return Utility::sample(BLIND_LINES, rng);
}

std::wstring Translator::pyramidFairyLine() {
  return L"[L1]HEY![L2]LISTEN![END]";
}

std::wstring Translator::sahasralah1Line() {
  Region region = keyLocations.at(KeyLocation::GREEN_PENDANT);
  std::wstring location = REGION_NAMES.at(region);
  return Utility::stringf(L"[L1]%s에서[L2]용기의 문장을[L3]가져오거라.[END]", location.c_str());
}

std::wstring Translator::sahasralah2Line() {
  return L"[L1]그걸로 세상을 구하거라.[END]";
}

std::wstring Translator::tavernLine() {
  static const std::vector<std::wstring> TAVERN_LINES = {
    L"[L1]반성문이 두 개[L2]있으면 성문[END]",
    L"[L1]대학생들이 힘이 좋은[L2]이유는? 개강해서.[END]",
    L"[L1]다리미가 좋아하는[L2]음식은 피자.[END]",
    L"[L1]우주인들의 술집은[L2]스페이스바.[END]",
    L"[L1]왕이 주차하면[L2]파킹이다.[END]",
    L"[L1]성 세 개에 불이[L2]나면 삼성화재.[END]",
    L"[L1]재수 없는게 좋은[L2]사람은 고삼수험생.[END]",
    L"[L1]뜨거운 복숭아는[L2]천도복숭아.[END]",
    L"[L1]길가다 나무를[L2]주으면 우드득.[END]",
    L"[L1]할아버지가 좋아하는[L2]돈은 할머니.[END]"
  };

  return Utility::sample(TAVERN_LINES, rng);
}

std::wstring Translator::bombShop1Line() {
  Region regions[2] = { keyLocations.at(KeyLocation::RED_CRYSTAL_A), keyLocations.at(KeyLocation::RED_CRYSTAL_B) };
  std::wstring locations[2] = { REGION_NAMES.at(regions[0]), REGION_NAMES.at(regions[1]) };
  const wchar_t format[] = L"[L1]큰 폭탄을 만들어 드리려면[L2]%s의[L3]수정과 %s의[LF][CR][L3]수정이 필요해요.[END]";
  return Utility::stringf(format, locations[0].c_str(), locations[1].c_str());
}

std::wstring Translator::bombShop2Line() {
  return L"[L1]폭탄 30개 100루피[L2]큰 폭탄 하나 100루피[END]";
}

std::wstring Translator::etherLine() {
  return lockedItemLine(LockedLocation::ETHER);
}

std::wstring Translator::bombosLine() {
  return lockedItemLine(LockedLocation::BOMBOS);
}

std::wstring Translator::invincible1Line() {
  static const std::wstring PREFIX = L"[L1]난 죽지 않아.";
  static const std::wstring PEDESTAL_GOAL = L"[L2]마스터 소드나 찾으러 가.";
  static const std::wstring TRIFORCE_GOAL = L"[L2]트라이포스나 찾으러 가.";
  static const std::wstring POSTFIX = L"[END]";

  auto line = romST[StringTable::HiBankIndex::GANON2];

  std::wstringstream ss;
  ss << PREFIX;
  if(line.find(L"PEDESTAL") != line.npos)      { ss << PEDESTAL_GOAL; }
  else if(line.find(L"TRIFORCE") != line.npos) { ss << TRIFORCE_GOAL; }
  ss << POSTFIX;

  return ss.str();
}

std::wstring Translator::invincible2Line() {
  return L"[L1]귀먹었냐? 난 안[L2]죽는다니까.[END]";
}

