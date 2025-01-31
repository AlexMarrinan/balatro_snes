#include "GS_Title.h"
#include "SpriteManager.h"

#define TS_SpriteMax 12

//these are the frame starting positions of 32x32 sprites
#define SP_FS_MARIO 24
#define SP_FS_TBIRD 28

//these are the frame starting positions of 64x64 sprites
#define SP_FS_CHICK 0
#define SP_FS_PIDG 2

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
	u8 ystart=0;
	u8 xstart=0;
	u8 ystart2=0;
	u8 xstart2=160;
	tSprite* pSP=&Sprites[0];
	g=0;
	for(n=0;n<TS_SpriteMax;n++)
	{
		switch(g)
		{
		case 0:
				SPR_SetTSprite32(pSP,xstart,ystart,SP_FS_MARIO,SP_PALID_SPR32);
				ystart+=32;
				pSP->id=eID_OTHER;
			break;
		case 1:
				SPR_SetTSprite32(pSP,xstart,ystart,SP_FS_TBIRD,SP_PALID_SPR32);
				ystart+=32;
				pSP->id=eID_OTHER;
			break;
		case 2:
				SPR_SetTSprite64(pSP,xstart,ystart,SP_FS_CHICK,SP_PALID_CHICK64);
				ystart+=64;
				pSP->id=eID_CHICK;
			break;
		case 3:
				SPR_SetTSprite64(pSP,xstart,ystart,SP_FS_PIDG,SP_PALID_PIDGE64);
				ystart+=64;
				pSP->id=eID_PIDGE;
			break;
		}

		if((ystart+64)>=224)
		{
			ystart=0;
			xstart+=64;
		}

		++tp;


		g++;
		if(g>3)
			g=0;

		++pSP;

	}
}

void MoveSprites()
{
	u8 n,t;
	s16 x;
	tSprite* pSP=&Sprites[0];
	for(n=0;n<TS_SpriteMax;n++)
	{
		x=pSP->x;
		x++;
		if(pSP->id>eID_PIDGE)
		{
			if(x>256)
				x=-32;
		}
		else
		{
			if(x>256)
				x=-64;
		}

		pSP->x=x;
		pSP->updateXY=true;
		if(bAnimate)
		{
			pSP->updateFrame=true;
			x=pSP->frame;
			x++;
			if(pSP->id==eID_CHICK)
			{
				if(x==2)
				{
					x=0;
					if(pSP->mirrorX)
					{
						pSP->mirrorX=false;
					}
					else
					{
						pSP->mirrorX=true;
					}
				}
			}
			else
			{
				if(x==4)
					x=0;
			}

			pSP->frame=x;
		}

		SPR_BatchAdd(pSP);
		++pSP;
	}
}

void TS_Init()
{
	u8 x,y;
	S2D_Cls(S2D_BG1);
	S2D_Cls(S2D_BG2);
	S2D_SetAtribPalette(PAL_ID_GRASS);
	for(y=0;y<28;y+=4)
	{
		for(x=0;x<32;x+=4)
		{
			S2D_SetAtribPalette(PAL_ID_GRASS);
			S2D_TilePut32(S2D_BG_BOT,x,y,GFX_TS_GRASS);
			if(x==0)
			{
				S2D_SetAtribPalette(PAL_ID_WALLS);
				S2D_TilePut32(S2D_BG_TOP,x,y,GFX_TS_WALLS);
			}
			else if(x==28)
			{
				S2D_SetAtribPalette(PAL_ID_WALLS);
				S2D_TilePut32(S2D_BG_TOP,x,y,GFX_TS_WALLS+32);
			}
		}

	S2D_SetAtribPalAndOnTop(PAL_ID_WALLS)						;
	S2D_TilePut32(S2D_BG_BOT,14,y,GFX_TS_WALLS+16);

	}
	S2D_SetAtribPalAndOnTop(PAL_ID_FONT);
	S2D_PrintCentre(S2D_BG_TOP,2,"Sprite 32 & 64 Show");
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

