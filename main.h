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

#define GS_HIGHSCORE		250
#define TS_SpriteMax 12


extern u16 GameState;
extern bool IsRunning;
extern u16 G_HighScore;

extern void TS_Init();
extern void TS_Run();
extern tSprite Sprites[TS_SpriteMax];

#endif // MAIN_H_INCLUDED
