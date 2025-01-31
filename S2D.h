#ifndef S2D_H
#define S2D_H

#include <snes.h>
#include <snes/background.h>
#include <stdbool.h>
#include "Utils.h"
#include <string.h>
#include <stdio.h>

///Tile Word BitFlag Bits
  //h = mirror Tile horizontal (x)
  //v = mirror Tile Vertical (y)
  //o = tile priority
  //p = palette ID 0-7 (8 Palettes of 16 colours or 8 Palettes of 4 colours)
  //t = tileID = 0 to 1023 - VRAM dependant
  //hvoppptt tttttttt

#define S2D_MAP_W 32u //Width of tileMap
#define S2D_MAP_H 32u 

#define S2D_MAPSIZE 1024u //number of WORDS for background layer tilemap
#define S2D_MAPSIZEBIG 4096u //64x64 tiles map
//Need to organise VRAM pages for TileMap and TileData

#define S2D_BG_1_VRAM_MAP_ADDR      0x0000
#define S2D_BG_2_VRAM_MAP_ADDR      0x0400 //will use Big TileMap 64x64 == 8k in size!
#define S2D_BG_3_VRAM_MAP_ADDR      0x0800

///Backgrounds 0 and 1 share 16 colour tilesets
#define S2D_BG_1_VRAM_TILES_ADDR    0x1000
#define S2D_BG_2_VRAM_TILES_ADDR    0x1000
#define S2D_BG_3_VRAM_TILES_ADDR    0x2000

///Raw Tile Attribute BitFlag ID 0 - 15

#define S2D_TAR_PALETTE_BITSHIFT    10 //bitflag id == 1<<10
#define S2D_TAR_FLIP_H_BITSHIFT     15 //bitflag id == 1<<15
#define S2D_TAR_FLIP_V_BITSHIFT     14 //bitflag id == 1<<14
#define S2D_TAR_PRIORITY_BITSHIFT   13 //bitflag id == 1<<13

#define S2D_TAR_BIT_FLIP_H (1u<<S2D_TAR_FLIP_H_BITSHIFT)
#define S2D_TAR_BIT_FLIP_V (1u<<S2D_TAR_FLIP_V_BITSHIFT)
#define S2D_TAR_BIT_PRIORITY (1u<<S2D_TAR_PRIORITY_BITSHIFT)

//for selecting bitflags of a certain type
#define S2D_TAR_PALETTE_MASK 0x1C00u
#define S2D_TAR_NOT_PALETTES_MASK 0xE3FFu

#define S2D_TILE_ATTR(pal, prio, flipV, flipH)     (((flipH) << S2D_TAR_FLIP_H_BITSHIFT) + ((flipV) << S2D_TAR_FLIP_V_BITSHIFT) + ((pal) << S2D_TAR_PALETTE_BITSHIFT) + ((prio) << S2D_TAR_PRIORITY_BITSHIFT))

#define S2D_TILE_ATTR_FULL(pal, prio, flipV, flipH,tileID)     (((flipH) << S2D_TAR_FLIP_H_BITSHIFT) + ((flipV) << S2D_TAR_FLIP_V_BITSHIFT) + ((pal) << S2D_TAR_PALETTE_BITSHIFT) + ((prio) << S2D_TAR_PRIORITY_BITSHIFT)+(tileID))

#define S2D_BG1 0
#define S2D_BG2 1
#define S2D_BG3 2

#define S2D_BG_TOP 0
#define S2D_BG_BOT 1
#define S2D_BG_HUD 2

extern u16 S2D_TileMapBG1[S2D_MAPSIZE];
extern u16 S2D_TileMapBG2[S2D_MAPSIZE];
extern u16 S2D_TileMapBG3[S2D_MAPSIZE];

extern void S2D_Init(u16 Font16_TileStart,u16 NumDH_TileStart);
extern u16 S2D_TileAtrib;
extern void S2D_SetAtribPalette(u8 paletteID);
extern void S2D_SetAtribPalAndOnTop(u8 paletteID);
extern void S2D_SetAtribTileMirrorX(bool bIsFlipped);
extern void S2D_SetAtribTileMirrorY(bool bIsFlipped);
extern void S2D_SetAtribTilePriority();

extern void S2D_TilePut(u8 layerID,u8 tx,u8 ty,u16 tile);
extern void S2D_TilePut8x16(u8 layerID,u8 tx,u8 ty,u16 tile);
extern void S2D_TilePut16(u8 layerID,u8 tx,u8 ty,u16 tile);
extern void S2D_TilePut32(u8 layerID,u8 tx,u8 ty,u16 tile);
extern void S2D_TileClr(u8 layerID,u8 tx,u8 ty);
extern void S2D_TileClr16(u8 layerID,u8 tx,u8 ty);
extern void S2D_TileClr8x16(u8 layerID,u8 tx,u8 ty);
extern void S2D_Cls(u8 layerID);

extern void S2D_SetPaletteCol4(u8 palID,u8 palCount,u16* pPalData);
extern void S2D_SetPaletteCol16(u8 palID,u8 colorCount,u16* pPalData);
extern void S2D_SetTileDataCol4(u16 tileID,u16 tileCount,u8* pTileData);
extern void S2D_SetTileDataCol16(u8 BGid,u16 tileID,u16 tileCount,u8* pTileData);

extern void S2D_Print(u8 layerID,u8 tx,u8 ty,const char* msg);
extern void S2D_PrintV(u8 layerID,u8 tx,u8 ty,const char* msg,...);
extern void S2D_PrintCentre(u8 layerID,u8 ty,const char* msg);

extern void S2D_Print16(u8 layerID,u8 tx,u8 ty,const char* msg);
extern void S2D_Print16Centre(u8 layerID,u8 ty,const char* msg);

extern void S2D_PrintDHNum8(u8 layerID,u8 x,u8 y,u8 val8,bool bDrawZeros);
extern void S2D_PrintDHNum16(u8 layerID,u8 x,u8 y,u16 val16,bool bDrawZeros);

extern void S2D_FillStripX(u8 layerID,u8 tx,u8 ty,u8 width,u16 tileID);
extern void S2D_FillStripY(u8 layerID,u8 tx,u8 ty,u8 height,u16 tileID);
//this function will copy the tilemap arrays into their VRAM places
//need to do this after a vsync
extern void S2D_Update();

#endif // S2D_H
