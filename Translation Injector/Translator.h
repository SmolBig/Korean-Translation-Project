#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <random>
#include <array>
#include "StringTable.h"
#include "ROM.h"
#include "GlyphTable.h"

class Translator {
public:
  Translator(const GlyphTable& gt, const ROM& rom);

  StringTable translate();

private:
  const GlyphTable& gt;
  const ROM& rom;
  const StringTable romST;

  static constexpr char LOW_BANK_TABLE_PATH[] = "../Data/low banks.txt";

  enum class Item {
    NOTHING,
    L1_SWORD,
    SWORD_AND_SHIELD,
    L2_SWORD,
    MASTER_SWORD,
    L3_SWORD,
    L4_SWORD,
    BLUE_SHIELD,
    RED_SHIELD,
    MIRROR_SHIELD,
    FIRE_ROD,
    ICE_ROD,
    HAMMER,
    HOOKSHOT,
    BOW,
    BOOMERANG,
    MAGIC_POWDER,
    BEE_REQUIRING_BOTTLE,
    BOMBOS_MEDALLION,
    ETHER_MEDALLION,
    QUAKE_MEDALLION,
    LAMP,
    SHOVEL,
    FLUTE_INACTIVE,
    CANE_OF_SOMARIA,
    EMPTY_BOTTLE,
    HEART_PIECE,
    CANE_OF_BYRNA,
    MAGIC_CAPE,
    MAGIC_MIRROR,
    POWER_GLOVE,
    TITANS_MITT,
    BOOK,
    FLIPPERS,
    MOON_PEARL,
    BUG_CATCHING_NET,
    BLUE_MAIL,
    RED_MAIL,
    SMALL_KEY,
    COMPASS,
    HEART_CONTAINER_NO_ANIMATION,
    BOMB,
    BOMBS_3,
    MUSHROOM,
    MAGICAL_BOOMERANG,
    RED_POTION_IN_BOTTLE,
    GREEN_POTION_IN_BOTTLE,
    BLUE_POTION_IN_BOTTLE,
    BOMBS_10,
    BIG_KEY,
    MAP,
    RUPEE,
    RUPEES_5,
    RUPEES_20,
    BOW_AND_ARROWS,
    BOW_AND_SILVER_ARROWS,
    BEE_IN_BOTTLE,
    FAIRY_IN_BOTTLE,
    BOSS_HEART_CONTAINER,
    SANCTUARY_HEART_CONTAINER,
    RUPEES_100,
    RUPEES_50,
    HEART,
    ARROW,
    ARROWS_10,
    SMALL_MAGIC,
    RUPEES_300,
    RUPEES_20_OTHER,
    GOLDEN_BEE_IN_BOTTLE,
    FLUTE_ACTIVE,
    PEGASUS_BOOTS,
    BOMB_UPGRADE_5,
    BOMB_UPGRADE_10,
    BOMB_UPGRADE_50,
    ARROW_UPGRADE_5,
    ARROW_UPGRADE_10,
    ARROW_UPGRADE_70,
    HALF_MAGIC,
    QUARTER_MAGIC,
    SILVER_ARROWS,
    RUPOOR,
    RED_CLOCK,
    BLUE_CLOCK,
    GREEN_CLOCK,
    PROGRESSIVE_SWORD,
    PROGRESSIVE_SHIELD,
    PROGRESSIVE_ARMOR,
    PROGRESSIVE_GLOVE,
    TRIFORCE,
    POWER_STAR,
    TRIFORCE_PIECE,
    MAP_OF_LIGHT_WORLD,
    MAP_OF_DARK_WORLD,
    MAP_OF_GANONS_TOWER,
    MAP_OF_TURTLE_ROCK,
    MAP_OF_THIEVES_TOWN,
    MAP_OF_TOWER_OF_HERA,
    MAP_OF_ICE_PALACE,
    MAP_OF_SKULL_WOODS,
    MAP_OF_MISERY_MIRE,
    MAP_OF_PALACE_OF_DARKNESS,
    MAP_OF_SWAMP_PALACE,
    MAP_OF_AGAHNIMS_TOWER,
    MAP_OF_DESERT_PALACE,
    MAP_OF_EASTERN_PALACE,
    MAP_OF_HYRULE_CASTLE,
    MAP_OF_SEWERS,
    COMPASS_OF_GANONS_TOWER,
    COMPASS_OF_TURTLE_ROCK,
    COMPASS_OF_THIEVES_TOWN,
    COMPASS_OF_TOWER_OF_HERA,
    COMPASS_OF_ICE_PALACE,
    COMPASS_OF_SKULL_WOODS,
    COMPASS_OF_MISERY_MIRE,
    COMPASS_OF_PALACE_OF_DARKNESS,
    COMPASS_OF_SWAMP_PALACE,
    COMPASS_OF_AGAHNIMS_TOWER,
    COMPASS_OF_DESERT_PALACE,
    COMPASS_OF_EASTERN_PALACE,
    COMPASS_OF_HYRULE_CASTLE,
    COMPASS_OF_SEWERS,
    BIG_KEY_OF_GANONS_TOWER,
    BIG_KEY_OF_TURTLE_ROCK,
    BIG_KEY_OF_THIEVES_TOWN,
    BIG_KEY_OF_TOWER_OF_HERA,
    BIG_KEY_OF_ICE_PALACE,
    BIG_KEY_OF_SKULL_WOODS,
    BIG_KEY_OF_MISERY_MIRE,
    BIG_KEY_OF_PALACE_OF_DARKNESS,
    BIG_KEY_OF_SWAMP_PALACE,
    BIG_KEY_OF_AGAHNIMS_TOWER,
    BIG_KEY_OF_DESERT_PALACE,
    BIG_KEY_OF_EASTERN_PALACE,
    BIG_KEY_OF_HYRULE_CASTLE,
    BIG_KEY_OF_SEWERS,
    SMALL_KEY_OF_GANONS_TOWER,
    SMALL_KEY_OF_TURTLE_ROCK,
    SMALL_KEY_OF_THIEVES_TOWN,
    SMALL_KEY_OF_TOWER_OF_HERA,
    SMALL_KEY_OF_ICE_PALACE,
    SMALL_KEY_OF_SKULL_WOODS,
    SMALL_KEY_OF_MISERY_MIRE,
    SMALL_KEY_OF_PALACE_OF_DARKNESS,
    SMALL_KEY_OF_SWAMP_PALACE,
    SMALL_KEY_OF_AGAHNIMS_TOWER,
    SMALL_KEY_OF_DESERT_PALACE,
    SMALL_KEY_OF_EASTERN_PALACE,
    SMALL_KEY_OF_HYRULE_CASTLE,
    SMALL_KEY_OF_SEWERS
  };

  enum class Region {
    EASTERN_PALACE,
    DESERT_PALACE,
    TOWER_OF_HERA,
    DEATH_MOUNTAIN,
    HYRULE_CASTLE,
    LIGHT_WORLD,
    PALACE_OF_DARKNESS,
    SWAMP_PALACE,
    SKULL_WOODS,
    THIEVES_TOWN,
    ICE_PALACE,
    MISERY_MIRE,
    TURTLE_ROCK,
    GANONS_TOWER,
    DARK_WORLD,
    AGAHNIMS_TOWER,
    SEWERS
  };

  enum class KeyLocation {
    BOOTS,
    SILVER_ARROWS,
    RED_CRYSTAL_A,
    RED_CRYSTAL_B,
    GREEN_PENDANT
  };

  enum class LockedLocation {
    PEDESTAL,
    ETHER,
    BOMBOS
  };

  static const std::unordered_map<LockedLocation, size_t> LOCKED_LOCATION_ADDRESSES;

  std::unordered_map<LockedLocation, Item> pedTabItems;
  std::unordered_map<KeyLocation, Region> keyLocations;
  std::mt19937 rng;

  static const std::unordered_map<Item, std::wstring> STATIC_ITEM_NAMES;
  const std::unordered_map<Item, std::wstring> ITEM_NAMES;
  static const std::unordered_map<Item, byte> ITEM_IDS;
  const std::unordered_map<byte, Item> REVERSED_ITEM_IDS;
  static const std::unordered_map<Region, std::wstring> REGION_NAMES;
  static const std::unordered_map<Region, std::vector<size_t>> REGIONS;
  static const std::unordered_map<Region, size_t> PRIZES;
  static constexpr byte GREEN_PENDANT_BYTES[2] = { 0x38, 0x62 };
  static constexpr byte RED_CRYSTAL_BYTES[2]   = { 0x32, 0x64 };

  void identifyKeyItemLocations();
  void identifyKeyPrizeLocations();
  void identifyPedTabItems();
  std::unordered_map<byte, Item> reverseItemIDs();
  std::unordered_map<Item, std::wstring> composeItemNames();

  static bool isUncleSpoiler(const std::wstring& line);
  std::wstring generateUncleSpoiler();
  std::wstring lockedItemLine(LockedLocation location);
  void identifyLockedLocation(LockedLocation location);

  std::wstring pedestalLine();
  std::wstring triforceLine();
  std::wstring uncleLine();
  std::wstring ganon1Line();
  std::wstring ganon2Line();
  std::wstring blindLine();
  std::wstring pyramidFairyLine();
  std::wstring sahasralah1Line();
  std::wstring sahasralah2Line();
  std::wstring tavernLine();
  std::wstring bombShop1Line();
  std::wstring bombShop2Line();
  std::wstring etherLine();
  std::wstring bombosLine();
  std::wstring invincible1Line();
  std::wstring invincible2Line();

  void keysanityFixup();

};

