#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <snes.h>
#include "gfxDefines.h"
#include "S2D.h"
#include "Utils.h"
#include "SpriteManager.h"

#define GS_TS_INIT 0
#define GS_TS_RUN 1
#define GS_GM_INIT 2
#define GS_GM_RUN 3

#define GS_HIGHSCORE 250
#define TS_SpriteMax 4
#define TS_CARDMAX 4

#define SUITE_SPADE 0
#define SUITE_HEART 1
#define SUITE_DIAMOND 2
#define SUITE_CLUB 3


#define PAD_COOLDOWN 8

extern u16 GameState;
extern bool IsRunning;
extern u16 G_HighScore;

extern void TS_Init();
extern void TS_Run();
extern tSprite Sprites[TS_SpriteMax];
extern tCard Cards[TS_CARDMAX];

#endif // MAIN_H_INCLUDED
