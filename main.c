#include "main.h"

u16 GameState;
bool IsRunning;
u16 G_HighScore;

#define MOD_POLLEN8                     	0

tSprite Sprites[TS_SpriteMax];
tCard Cards[TS_CARDMAX]; 
s16 hoveredCardIndex;
extern char SOUNDBANK__1, SOUNDBANK__0;

void zInitGFX()
{
	u16 off,tc;
	vsync();
	S2D_SetTileDataCol16(S2D_BG1,GFX_TS_FONT,GFX_TC_FONT,&dbTiles_T_font16);
	S2D_SetPaletteCol16(PAL_ID_FONT,PAL_CC_FONT,&dbTiles_P_font16);

	S2D_SetTileDataCol16(S2D_BG1,GFX_TS_GRASS,GFX_TC_GRASS,gfxMenuBg);
	S2D_SetPaletteCol16(PAL_ID_GRASS,PAL_CC_GRASS,palMenuBg);

	S2D_SetTileDataCol16(S2D_BG1,GFX_TS_WALLS,GFX_TC_WALLS,gfxWalls);
	S2D_SetPaletteCol16(PAL_ID_WALLS,PAL_CC_WALLS,palWalls);

	vsync();
	SPR_SetPalette(SP_PALID_CHICK64,SP_PALCC_CHICK64,&dbSpr_P_chick64);
	SPR_SetPalette(SP_PALID_CARD64,SP_PALCC_PIDGE64,&dbSpr_P_Pidg64);
	SPR_SetPalette(SP_PALID_SPR32,SP_PALCC_SPR32,&dbSpr_P_sprites32);
	//split graphics into chucks of 128x32

	pSrc8=(u8*)&dbSpr_T_chick64;
	SPR_SetTileData(SP_TS_CHICK64,SP_TC_CHICK64,pSrc8);
	vsync();
	SPR_SetTileData(SP_TS_PIDGE64,SP_TC_PIDGE64,&dbSpr_T_Pidg64);
	vsync();
	SPR_SetTileData(SP_TS_SPRITES32,SP_TC_SPRITES32,&dbSpr_T_sprites32);

}

int main()
{
    s16 spriteX = 0;
    s16 spriteY = 0;

    consoleInit();

    //clears input
    IN_Clear();

    //initalize 2d 
    S2D_Init(0,0);
    
    //initialize sprites
    SPR_Init(); 
    zInitGFX();
    setMode(BG_MODE1,BG3_MODE1_PRIORITY_HIGH);

    setScreenOn();

	  Snd_Init();
		Snd_PlaySong(0);



    IsRunning=true;
    GameState=GS_TS_INIT;
    PAD pad0;
    u16 padCooldown = 0;
    
    while(1)
    {
      switch(GameState)
      {
      case GS_TS_INIT:
        TS_Init();
        ++GameState;
        hoveredCardIndex = 0;
        tCard* pC = &Cards[hoveredCardIndex];
        pC->sprite->updateXY=true;
        pC->sprite->y-=32;
        break;
      case GS_TS_RUN:
        TS_Run();

        break;
      }


      pad0 = padsCurrent(0);
      tCard* pCard = &Cards[hoveredCardIndex];

      if (padCooldown > 0){
        padCooldown--;
      }
      if (pad0 && padCooldown <= 0){
        // Update sprite with current pad
        // if(pad0 & J_UP) {
        //   pSP->y-=speed;         
        // }
        if(pad0 & J_LEFT) {
          tSprite* pSP=pCard->sprite;
          
          pSP->updateXY=true;
          pSP->y+=32;

          hoveredCardIndex -= 1;

          if (hoveredCardIndex < 0){
            hoveredCardIndex = TS_CARDMAX-1;
          }
          pSP=&Sprites[hoveredCardIndex];
          pSP->updateXY=true;
          pSP->y-=32;
        }
        if(pad0 & J_RIGHT) {
          tSprite* pSP=pCard->sprite;
          
          pSP->updateXY=true;
          pSP->y+=32;

          hoveredCardIndex += 1;

          if (hoveredCardIndex >= TS_CARDMAX){
            hoveredCardIndex = 0;
          }
          pSP=&Sprites[hoveredCardIndex];
          pSP->updateXY=true;
          pSP->y-=32;
        }
        // if(pad0 & J_DOWN) {
        //   pSP->y+=speed;
        // } 
        padCooldown = PAD_COOLDOWN;
      }
      // S2D_PrintDHNum8(S2D_BG_TOP,3,3, hoveredCardIndex, true);
      char str[10];
      sprintf(str, "%d", (u16)hoveredCardIndex);
      S2D_PrintCentre(S2D_BG_TOP,3,str);
      vsync();
      Snd_Update();
			S2D_Update();
    }
return 0;
}
