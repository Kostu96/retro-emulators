#pragma once
#include "emu_common/graphics/color.hpp"

static constexpr int WINDOW_WIDTH = 1100;
static constexpr int WINDOW_HEIGHT = 550;

static constexpr int ALTAIR_OUTLINE_SIZE = 16;
static constexpr EmuCommon::Color ALTAIR_WHITE_COLOR{ 0xF2, 0xF1, 0xED };
static constexpr EmuCommon::Color ALTAIR_BLUE_COLOR{ 0x66, 0x9D, 0xD2 };
static constexpr EmuCommon::Color ALTAIR_GRAY_COLOR{ 0x6B, 0x69, 0x6A };

static constexpr float LED_SPRITE_SCALE = 1.f / 4.f;
static constexpr float SWITCH_SPRITE_SCALE = 1.f / 3.f;

static constexpr float ALTAIR_GRID_SPACING = 44.f;
static constexpr float ALTAIR_TRIPLET_SPACING = 22.f;
static constexpr float ALTAIR_GRID_COL0 = 98.f;
static constexpr float ALTAIR_GRID_COL1 = ALTAIR_GRID_COL0 + ALTAIR_GRID_SPACING;
static constexpr float ALTAIR_GRID_COL2 = ALTAIR_GRID_COL1 + ALTAIR_GRID_SPACING;
static constexpr float ALTAIR_GRID_COL_LAST = WINDOW_WIDTH - 98.f;
static constexpr float ALTAIR_GRID_ROW0 = 30.f;
static constexpr float ALTAIR_GRID_ROW1 = 125.f;
static constexpr float ALTAIR_GRID_ROW2 = 235.f;
