#ifndef SPRITEMANAGER_H_INCLUDED
#define SPRITEMANAGER_H_INCLUDED

#include <snes.h>
#include "Utils.h"

/*!  \brief Sprite Table (from no$sns help file)<br>
Contains data for 128 OBJs. OAM Size is 512+32 Bytes. The first part (512<br>
bytes) contains 128 4-byte entries for each OBJ:<br>
  Byte 0 - X-Coordinate (lower 8bit) (upper 1bit at end of OAM)<br>
  Byte 1 - Y-Coordinate (all 8bits)<br>
  Byte 2 - Tile Number  (lower 8bit) (upper 1bit within Attributes)<br>
  Byte 3 - Attributes<br>
Attributes:<br>
  Bit7    Y-Flip (0=Normal, 1=Mirror Vertically)<br>
  Bit6    X-Flip (0=Normal, 1=Mirror Horizontally)<br>
  Bit5-4  Priority relative to BG (0=Low..3=High)<br>
  Bit3-1  Palette Number (0-7) (OBJ Palette 4-7 can use Color Math via CGADSUB)<br>
  Bit0    Tile Number (upper 1bit)<br>
After above 512 bytes, additional 32 bytes follow, containing 2-bits per OBJ:<br>
  Bit7    OBJ 3 OBJ Size     (0=Small, 1=Large)<br>
  Bit6    OBJ 3 X-Coordinate (upper 1bit)<br>
  Bit5    OBJ 2 OBJ Size     (0=Small, 1=Large)<br>
  Bit4    OBJ 2 X-Coordinate (upper 1bit)<br>
  Bit3    OBJ 1 OBJ Size     (0=Small, 1=Large)<br>
  Bit2    OBJ 1 X-Coordinate (upper 1bit)<br>
  Bit1    OBJ 0 OBJ Size     (0=Small, 1=Large)<br>
  Bit0    OBJ 0 X-Coordinate (upper 1bit)<br>
And so on, next 31 bytes with bits for OBJ4..127. Note: The meaning of the OBJ<br>
Size bit (Small/Large) can be defined in OBSEL Register (Port 2101h).<br>
*/

#define SPR_BF_FLIP_X          64
#define SPR_BF_FLIP_Y          128
#define SPR_BF_PRIORITY_HIGH   48
#define SPR_BF_PRIORITY_MED    32
#define SPR_BF_PRIORITY_LOW    16
#define SPR_BF_PAL_7           2+4+8  // 111x = 14
#define SPR_BF_PAL_6           4+8    // 110x = 12
#define SPR_BF_PAL_5           2+8    // 101x = 10
#define SPR_BF_PAL_4           8      // 100x = 8
#define SPR_BF_PAL_3           2+4    // 011x = 6
#define SPR_BF_PAL_2           4      // 100x = 4
#define SPR_BF_PAL_1           2      // 001x = 2
#define SPR_BF_PAL_0           0      // 000x
#define SPR_BF_TILE_HIGH       1      //this bit allows >= Tile 256 to be set

//NOTE 0x6000 (words) or (0xC000 - in bytes) is highest address in VRAM to be able to use all 512 tiles, as 512 tiles == 16k

#define SPR_VRAM_TILES_ADDR 0x4000

#define SPR_HIDE_X 256
#define SPR_HIDE_Y 224

//the below are for use with the SPR_ATR Macros
#define SPR_FLIP_X 1
#define SPR_FLIP_Y 1
#define SPR_PRIORITY_LOW 1
#define SPR_PRIORITY_MED 2
#define SPR_PRIORITY_HIGH 3
#define SPR_PAL0 	0
#define SPR_PAL1 	1
#define SPR_PAL2 	2
#define SPR_PAL3 	3
#define SPR_PAL4 	4
#define SPR_PAL5 	5
#define SPR_PAL6 	6
#define SPR_PAL7	7

///Palette will be wrapped into 0-7 range and return correct bitflag
#define SPR_ATR_PAL(pal) ((pal==0) ? 0 : (((pal&7)-1)<<1)+2)
///Note TileID must be >512 for this to work
#define SPR_ATR(pal, prio, flipV, flipH)     (((flipH & 1) * SPR_BF_FLIP_X) | ((flipV & 1) *SPR_BF_FLIP_Y) | (SPR_ATR_PAL(pal)) | ((prio & 0x03)<<4))
#define SPR_ATR_FULL(pal, prio, flipV, flipH,tileID) (((flipH & 1) * SPR_BF_FLIP_X) | ((flipV & 1) *SPR_BF_FLIP_Y) | (SPR_ATR_PAL(pal)) | ((prio & 0x03)<<4) |(((tileID&0x1ff)&0x100)>>8))

#define SPR_LUT16_MAX 128
#define SPR_LUT32_MAX 32
#define SPR_LUT64_MAX 8

//this project will use 32 and 64 sized sprites only
//to change sizes amend the below line to OBJ_SIZEXX_LXX as per pvsneslib
#define SPR_SIZES OBJ_SIZE32_L64

//As we are backing up sprite variables to an array only use the amount needed
//Change this for your own projects
#define SPR_MAX 16

//Sprite State Changes
enum
{
	eSPR_F_NO_ACTION=0,
	eSPR_F_UPDATE_POS=1,
	eSPR_F_UPDATE_TILE=2,
	eSPR_F_SIZE_SMALL=4,
	eSPR_F_MAX

};


typedef struct
{
	u16 id;
  u16 x;
  u16 y;
  u8 palID;
  u16 frame;
  u16 baseFrame;
  bool bIsSmall;
	bool updateXY;
	bool updateFrame;
	bool mirrorX;
	bool isVisible;
} tSprite;

typedef struct
{
  u16 id;
  u8 suit;
  u8 rank;
  tSprite *sprite;
} tCard;

extern void SPR_Init();
extern void SPR_ClearAll();

extern void SPR_SetTileData(u16 tileStart,u16 tileCount,const u8* pGfx);
extern void SPR_SetPalette(u16 palID,u16 colCount,const u16* pPal);
extern void SPR_SetTileData16x16(u16 tileStart,u16 tileCount,const u8* pGfx);
extern u8 SPR_AddT16(u16 tileBase,s16 xp,s16 yp,u8 Atribs);
extern u8 SPR_AddT32(u16 tileBase,s16 xp,s16 yp,u8 Atribs);
extern void SPR_Hide(u8 spriteID);
extern void SPR_SetPos(u8 spriteID,s16 x,s16 y);
extern void SPR_SetPosAndTile(u8 spriteID,s16 x,s16 y,u16 tile,u8 attribs);
extern void SPR_SetPosAndTile16(u8 spriteID,s16 x,s16 y,u16 base,u8 frame,u8 attribs);

extern void SPR_SetTileData16(u8 frameID,u8 frameCount,const u8* pGfx);
extern u8 SPR_SpriteCount;
extern u8 SPR_BatchCount;
//sprite batch syste
extern tSprite SPR_Batch[SPR_MAX];
//Note this is currently configured for 32 & 64 sized sprites, you will need
//to change which LUT's are used for small and Large sprite types if you want a different
//sizes of sprites in your own project
extern void SPR_DrawBatch();
extern void SPR_SetTSprite16(tSprite* pSpr,u16 x,u16 y,u8 baseFrameID,u8 paletteID);
extern void SPR_SetTSprite16Ex(tSprite* pSpr,u16 x,u16 y,u8 baseFrameID,u8 frameOffsetID,u8 paletteID,bool bMirrorX,bool bIsVisible);
extern void SPR_SetTSprite32(tSprite* pSpr,u16 x,u16 y,u8 baseFrameID,u8 paletteID);
extern void SPR_SetTSprite32Ex(tSprite* pSpr,u16 x,u16 y,u8 baseFrameID,u8 frameOffsetID,u8 paletteID,bool bMirrorX,bool bIsVisible);
extern void SPR_SetTSprite64(tSprite* pSpr,u16 x,u16 y,u8 baseFrameID,u8 paletteID);
extern void SPR_SetTSprite64Ex(tSprite* pSpr,u16 x,u16 y,u8 baseFrameID,u8 frameOffsetID,u8 paletteID,bool bMirrorX,bool bIsVisible);
extern void SPR_BatchAdd(tSprite* pSpr);

extern u16 SPR_ImgLUT16[SPR_LUT16_MAX];
extern u16 SPR_ImgLUT32[SPR_LUT32_MAX];
extern u16 SPR_ImgLUT64[SPR_LUT64_MAX];

#endif // SPRITEMANAGER_H_INCLUDED
