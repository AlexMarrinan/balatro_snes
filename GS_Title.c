#include "GS_Title.h"
#include "SpriteManager.h"
#include <stdio.h> 
#include <stdlib.h>

//these are the frame starting positions of 32x32 sprites
#define SP_FS_MARIO 24
#define SP_FS_TBIRD 28 

//these are the frame starting positions of 64x64 sprites
#define SP_FS_CHICK 0
#define SP_FS_CARD 2

tTickTimer zTS_AnimTime;
bool bAnimate; 
u8 tp=8;
//this is used to track sprite types, as they have different amounts of
//animation frames
enum
{
	eID_CHICK=0,
	eID_PIDGE,
	eID_OTHER
};

tSprite Sprites[TS_SpriteMax];  
tCard Cards[TS_CARDMAX];

void DrawTileOffset32() 
{
	u8 n;
	for(n=0;n<SPR_LUT32_MAX;n++)
	{
		if(n<28)
		{ 
			S2D_PrintV(0,0,n,"LUT id%d = %d",(u16)n,SPR_ImgLUT32[n]);
		}
		else 
		{
			S2D_PrintV(16,0,(n-28),"LUT id%d = %d",(u16)n,SPR_ImgLUT32[n]);
		}
	}
}

void InitSprites()
{
	u8 n,g;
	u8 ystart=160;
	u8 xstart=0;
	u8 ystart2=0;
	u8 xstart2=160;
	tSprite* pSP=&Sprites[0];
	tCard* pCard=&Cards[0];

	g=0;
	for(n=0;n<TS_SpriteMax;n++)
	{
		SPR_SetTSprite64(pSP,xstart,ystart,SP_FS_CARD,SP_PALID_CARD64);
		pSP->id=eID_OTHER;

		//set card value
		pCard->suit=SUITE_SPADE;
		pCard->rank=0;
		pCard->sprite=pSP;
		xstart+=32;
		++tp;
		++pSP;
		++pCard;
	}
}

void MoveSprites()
{
	u8 n,t;
	s16 x;
	tSprite* pSP=&Sprites[0];
	for(n=0;n<TS_SpriteMax;n++)
	{
		pSP->updateXY=true;
		SPR_BatchAdd(pSP);
		++pSP; 
	}
}

void TS_Init()
{
	// u8 x,y; 
	// S2D_Cls(S2D_BG1);
	// S2D_Cls(S2D_BG2);
	// S2D_SetAtribPalette(PAL_ID_GRASS);
	// for(y=0;y<28;y+=4)  
	// {
	// 	for(x=0;x<32;x+=4)
	// 	{
	// 		S2D_SetAtribPalette(PAL_ID_GRASS);
	// 		S2D_TilePut32(S2D_BG_BOT,x,y,GFX_TS_GRASS);
	// 	}
	// }
	
	u16 num = 100;

	S2D_PrintCentre(S2D_BG_TOP,2,"BALATRO");

	//void S2D_PrintDHNum8(u8 layerID,u8 x,u8 y,u8 val8,bool bDrawZeros)

	S2D_Print(S2D_BG_TOP,3, 12,"100 x 50");

	TickTimer_Init(&zTS_AnimTime,8);
	bAnimate=false;
	InitSprites();
}

void TS_Run()
{
	if(TickTimer_Update(&zTS_AnimTime)==true)
	{
		TickTimer_Reset(&zTS_AnimTime);
		bAnimate=true;
	}
	MoveSprites();
	bAnimate=false;

	SPR_DrawBatch();
}

