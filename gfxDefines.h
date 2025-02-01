#ifndef GFXDEFINES_H_INCLUDED
#define GFXDEFINES_H_INCLUDED
#include <snes.h>
#include "Utils.h"

#define GFX_TS_FONT 0
#define GFX_TC_FONT 128

#define GFX_TS_GRASS (GFX_TS_FONT+GFX_TC_FONT)
#define GFX_TC_GRASS 64

#define GFX_TS_WALLS (GFX_TS_GRASS+GFX_TC_GRASS)
#define GFX_TC_WALLS 48

#define PAL_ID_FONT 0
#define PAL_CC_FONT 4

#define PAL_ID_GRASS 1
#define PAL_CC_GRASS 11

#define PAL_ID_WALLS 2
#define PAL_CC_WALLS 16

//sprites

#define SP_TS_CHICK64 0
#define SP_TC_CHICK64 128

#define SP_TS_PIDGE64 (SP_TS_CHICK64+SP_TC_CHICK64)
#define SP_TC_PIDGE64 128

#define SP_TS_SPRITES32 (SP_TS_PIDGE64+SP_TC_PIDGE64)
#define SP_TC_SPRITES32 256

#define SP_PALID_CHICK64 0
#define SP_PALCC_CHICK64 14

#define SP_PALID_PIDGE64 1
#define SP_PALCC_PIDGE64 11

#define SP_PALID_SPR32 2
#define SP_PALCC_SPR32 16

extern const u16 palGrass[];
extern const u8 gfxGrass[];

extern const u16 palWalls[];
extern const u8 gfxWalls[];

extern const u16 palMenuBg[];
extern const u8 gfxMenuBg[];

//binary file byte externs
extern const u8 dbSpr_T_chick64;
extern const u8 dbSpr_P_chick64;

extern const u8 dbSpr_T_Pidg64;
extern const u8 dbSpr_P_Pidg64;

extern const u8 dbSpr_T_sprites32;
extern const u8 dbSpr_P_sprites32;

extern const u8 dbTiles_T_font16;
extern const u8 dbTiles_P_font16;

#endif // GFXDEFINES_H_INCLUDED
