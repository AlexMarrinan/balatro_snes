#include "SpriteManager.h"

u16 SPR_ImgLUT16[SPR_LUT16_MAX];
u16 SPR_ImgLUT32[SPR_LUT32_MAX];
u16 SPR_ImgLUT64[SPR_LUT64_MAX];
u8 SPR_Count;
u8 SPR_BatchCount;
static u8 zSPR_sizes;
static u8 zSPR_nextID;
static u16 zSLI,zSLJ;
tSprite SPR_Batch[SPR_MAX];
static tSprite* zSPR_pSprite;

//LUT should work to set and get frame address from VRAM
void zSPR_Init_LUT32()
{
	u16 n=0; 
	u16 tileBlock=0;
	u16 tileOffset=0;
	u32 tot;
	pDst16=&SPR_ImgLUT32[0];
	for(n=0;n<SPR_LUT32_MAX;n++)
	{
		tileBlock=(n>>2)<<6; //divide n/4 then * by 64
		tileOffset=(n&3)<<2; //mod 4 then * by 4
		tot=(tileBlock+tileOffset);
		if(tot>=0xFFFF)
			ErrorTrap("Overflow");
		*pDst16=(tileBlock+tileOffset); //Byte Offset must be divided by 2 for words!
		++pDst16;
	}
}

void zSPR_Init_LUT64()
{
	u16 n=0;
	u16 tileBlock=0;
	u16 tileOffset=0;
	pDst16=&SPR_ImgLUT64[0];
	for(n=0;n<SPR_LUT64_MAX;n++)
	{
		tileBlock=(n>>1)<<7; //divide n/4 then * by 64
		tileOffset=0;
		if(n&1)
			tileOffset=8;

		*pDst16=(tileBlock+tileOffset); //Byte Offset must be divided by 2 for words!
		++pDst16;
	}

}

void zSPR_Init_LUT16()
{
	u16 n=0;
	u16 tileBlock=0;
	u16 tileOffset=0;
	pDst16=&SPR_ImgLUT16[0];
	for(n=0;n<SPR_LUT16_MAX;n++)
	{
		tileBlock=(n>>3)<<5; //divide n/8 then * by 32
		tileOffset=(n&7)<<1; //mod 8 then * by 2
		*pDst16=(tileBlock+tileOffset); //Byte Offset must be divided by 2 for words!
		++pDst16;
	}
}

void SPR_Init()
{
	oamInitGfxAttr(SPR_VRAM_TILES_ADDR,SPR_SIZES);
	SPR_ClearAll();
	zSPR_nextID=0;
	zSPR_Init_LUT32();
	zSPR_Init_LUT64();
}

void SPR_ClearAll()
{
	u8 X=0;
	SPR_Count=0;
	oamClear(0,0);
	zSPR_nextID=0;
	zSPR_pSprite=&SPR_Batch[0];
	while(X<SPR_Count)
	{
		zSPR_pSprite->id=0;
		zSPR_pSprite->isVisible=false;
		++zSPR_pSprite;
		++X;
	};
	SPR_BatchCount=0;
}

void SPR_SetTileData(u16 tileStart,u16 tileCount,const u8* pGfx)
{
	zSLJ=tileCount<<5;
  zSLI=SPR_VRAM_TILES_ADDR;
  zSLI+=(tileStart<<4);
  WaitForVBlank();
  dmaCopyVram((u8*)pGfx,zSLI,zSLJ);
}

void SPR_SetPalette(u16 palID,u16 colCount,const u16* pPal)
{
	zSLI=128; //Note CGRAM upper part is for sprite palettes - actual start is 256 bytes - this is 128 * 2
  zSLI+=(palID*16); //*16 -
  zSLJ=(colCount<<1); //this is size in bytes
  vsync();
  dmaCopyCGram((u8*)pPal,zSLI,zSLJ);
}

u8 SPR_AddT16(u16 tileBase,s16 xp,s16 yp,u8 Atribs)
{
	if(zSPR_nextID==128)
	{
		ErrorTrap("SPR_AddT16 Too Many");
	}

	zSLI=zSPR_nextID<<2;
	oamSetEx(zSLI,OBJ_SMALL,OBJ_SHOW);
	oamSetAttr(zSLI,xp,yp,tileBase,Atribs);
	++zSPR_nextID;
	return zSPR_nextID-1;
}

u8 SPR_AddT32(u16 tileBase,s16 xp,s16 yp,u8 Atribs)
{
	if(zSPR_nextID==128)
	{
		ErrorTrap("SPR_AddT32 Too Many");
	}
	zSLI=zSPR_nextID<<2;
	oamSetEx(zSLI,OBJ_LARGE,OBJ_SHOW);
	oamSetAttr(zSLI,xp,yp,tileBase,Atribs);
	++zSPR_nextID;
	return zSPR_nextID-1;
}

void SPR_Hide(u8 spriteID)
{
	if(spriteID<zSPR_nextID)
	{
		zSLI=spriteID<<2;
		oamSetXY(zSLI,SPR_HIDE_X,SPR_HIDE_Y);
	}
	else
	{
		ErrorTrapEX("SPR_Hide illegal %d",(u16)spriteID);
	}
}

void SPR_SetPos(u8 spriteID,s16 x,s16 y)
{
	
	if(spriteID<zSPR_nextID)
	{
		zSLI=spriteID<<2;
		oamSetXY(zSLI,x,y);
	}
	else
	{
		ErrorTrapEX("SPR_SetPos illegal %d",(u16)spriteID);
	}
}

void SPR_SetPosAndTile(u8 spriteID,s16 x,s16 y,u16 tile,u8 attribs)
{
	if(spriteID<zSPR_nextID)
	{
		zSLI=spriteID<<2;
		if(tile>=256)
		{
			attribs|=1;
			tile &=0xff;
		}
		oamSetAttr(zSLI,x,y,tile,attribs);

	}
	else
	{
		ErrorTrapEX("SPR_SetPosAndTile %d",(u16)spriteID);
	}
}

void SPR_SetPosAndTile16(u8 spriteID,s16 x,s16 y,u16 base,u8 frame,u8 attribs)
{
	static u8 T;
	if(spriteID>=zSPR_nextID)
		ErrorTrapEX("SPR_SetPosAndTile16 %d",(u16)spriteID);

	T=frame & 0x07;
	frame-=T;
	zSLJ=frame<<5;
	zSLJ+=(T<<1);
	zSLJ+=base;
	zSLI=spriteID<<2;
	if(zSLJ>=256)
	{
		attribs|=1;
		zSLJ &=0xff;
	}
	oamSetAttr(zSLI,x,y,zSLJ,attribs);
}

void SPR_SetTSprite16(tSprite* pSpr,u16 x,u16 y,u8 baseFrameID,u8 paletteID)
{
	pSpr->bIsSmall=true; //You will need to change this is you change to 8 & 16 size sprites
	pSpr->baseFrame=baseFrameID;
	pSpr->frame=0;
	pSpr->isVisible=true;
	pSpr->mirrorX=false;
	pSpr->palID=paletteID;
	pSpr->x=x;
	pSpr->y=y;
	pSpr->updateFrame=true; //have to make true to draw it
}

void SPR_SetTSprite16Ex(tSprite* pSpr,u16 x,u16 y,u8 baseFrameID,u8 frameOffsetID,u8 paletteID,bool bMirrorX,bool bIsVisible)
{
	pSpr->bIsSmall=true; //You will need to change this is you change to 8 & 16 size sprites
	pSpr->baseFrame=baseFrameID;
	pSpr->frame=frameOffsetID;
	pSpr->isVisible=bIsVisible;
	pSpr->mirrorX=bMirrorX;
	pSpr->palID=paletteID;
	pSpr->x=x;
	pSpr->y=y;
	pSpr->updateFrame=true; //have to make true to draw it
}

void SPR_SetTSprite32(tSprite* pSpr,u16 x,u16 y,u8 baseFrameID,u8 paletteID)
{
	pSpr->bIsSmall=true; //You will need to change this is you change to <32 & 32 size sprites
	pSpr->baseFrame=baseFrameID;
	pSpr->frame=0;
	pSpr->isVisible=true;
	pSpr->mirrorX=false;
	pSpr->palID=paletteID;
	pSpr->x=x;
	pSpr->y=y;
	pSpr->updateFrame=true; //have to make true to draw it
}

void SPR_SetTSprite32ex(tSprite* pSpr,u16 x,u16 y,u8 baseFrameID,u8 frameOffsetID,u8 paletteID,bool bMirrorX,bool bIsVisible)
{
	pSpr->bIsSmall=true; //You will need to change this is you change to <32 & 32 size sprites
	pSpr->baseFrame=baseFrameID;
	pSpr->frame=frameOffsetID;
	pSpr->isVisible=bIsVisible;
	pSpr->mirrorX=bMirrorX;
	pSpr->palID=paletteID;
	pSpr->x=x;
	pSpr->y=y;
	pSpr->updateFrame=true; //have to make true to draw it
}

void SPR_SetTSprite64(tSprite* pSpr,u16 x,u16 y,u8 baseFrameID,u8 paletteID)
{
	pSpr->bIsSmall=false;
	pSpr->baseFrame=baseFrameID;
	pSpr->frame=0;
	pSpr->isVisible=true;
	pSpr->mirrorX=false;
	pSpr->palID=paletteID;
	pSpr->x=x;
	pSpr->y=y;
	pSpr->updateFrame=true; //have to make true to draw it
}

void SPR_SetTSprite64ex(tSprite* pSpr,u16 x,u16 y,u8 baseFrameID,u8 frameOffsetID,u8 paletteID,bool bMirrorX,bool bIsVisible)
{
	pSpr->bIsSmall=false;
	pSpr->baseFrame=baseFrameID;
	pSpr->frame=frameOffsetID;
	pSpr->isVisible=bIsVisible;
	pSpr->mirrorX=bMirrorX;
	pSpr->palID=paletteID;
	pSpr->x=x;
	pSpr->y=y;
	pSpr->updateFrame=true; //have to make true to draw it
}

void SPR_BatchAdd(tSprite* pSpr)
{
	if(SPR_BatchCount<SPR_MAX)
	{
		zSPR_pSprite=&SPR_Batch[SPR_BatchCount];
		++SPR_BatchCount;
		zSPR_pSprite->bIsSmall=pSpr->bIsSmall;
		zSPR_pSprite->baseFrame=pSpr->baseFrame;
		zSPR_pSprite->frame=pSpr->frame;
		zSPR_pSprite->isVisible=pSpr->isVisible;
		zSPR_pSprite->mirrorX=pSpr->mirrorX;
		zSPR_pSprite->palID=pSpr->palID;
		zSPR_pSprite->updateFrame=pSpr->updateFrame;
		zSPR_pSprite->updateXY=pSpr->updateXY;
		zSPR_pSprite->x=pSpr->x;
		zSPR_pSprite->y=pSpr->y;

	}
}

void SPR_DrawBatch()
{
	static u8 T,A,F;
	static u16 S;
	static u16 sprID;
	zSPR_pSprite=&SPR_Batch[0];
	T=SPR_BatchCount;
	SPR_Count=0;
	//hide any sprites above count but smaller than max
	sprID=SPR_BatchCount<<2;
	while(T<SPR_MAX)
	{
		oamSetAttr(sprID,SPR_HIDE_X,SPR_HIDE_Y,0,0);
		sprID+=4;
		++T;
	};
	T=0;
	sprID=0;
	while(T<SPR_BatchCount)
	{
		F=0;
		if(zSPR_pSprite->isVisible)
		{
			if(zSPR_pSprite->updateFrame)
			{
				if(zSPR_pSprite->updateXY)
					zSPR_pSprite->updateXY=false;

				if(zSPR_pSprite->mirrorX)
					F=1;

				if(zSPR_pSprite->bIsSmall)
				{
					S=SPR_ImgLUT32[zSPR_pSprite->baseFrame+zSPR_pSprite->frame];
					A=SPR_ATR_FULL(zSPR_pSprite->palID,SPR_PRIORITY_HIGH,0,F,S);
					oamSetEx(sprID,OBJ_SMALL,OBJ_SHOW);
				}
				else
				{
					S=SPR_ImgLUT64[zSPR_pSprite->baseFrame+zSPR_pSprite->frame];
					A=SPR_ATR_FULL(zSPR_pSprite->palID,SPR_PRIORITY_HIGH,0,F,S);
					oamSetEx(sprID,OBJ_LARGE,OBJ_SHOW);
				}


				oamSetAttr(sprID,zSPR_pSprite->x,zSPR_pSprite->y,S,A);
			}

			if(zSPR_pSprite->updateXY)
			{
				zSPR_pSprite->updateXY=false;
				oamSetXY(sprID,zSPR_pSprite->x,zSPR_pSprite->y);
			}
			++SPR_Count;
		}
		else
		{
			oamSetAttr(T,SPR_HIDE_X,SPR_HIDE_Y,0,0);
		}
		sprID+=4;
		++T;
		++zSPR_pSprite;
	};
	SPR_BatchCount=0;
}
