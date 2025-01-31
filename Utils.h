#ifndef UTILS_H
#define UTILS_H

#include <snes.h>
#include <snes/snestypes.h>
#include <stdbool.h>
#include <string.h>

#define CSTRING_MAX	65

#define J_UP      KEY_UP
#define J_DOWN    KEY_DOWN
#define J_LEFT    KEY_LEFT
#define J_RIGHT   KEY_RIGHT
#define J_A       KEY_A
#define J_B       KEY_B
#define J_X       KEY_X
#define J_Y       KEY_Y
#define J_L       KEY_L
#define J_R       KEY_R
#define J_START   KEY_START
#define J_SELECT  KEY_SELECT
#define PAD short

#define TRUE 1
#define FALSE 0
#define SPRITE_SIZE 4

enum
{
	eDIR_NOTHING=0,
	eDIR_UP,
	eDIR_DOWN,
	eDIR_RIGHT,
	eDIR_LEFT,
	eDIR_MAX
};

typedef struct
{
  u8 Len;
  u8 Data[CSTRING_MAX];
} CString;

typedef struct
{
	u16 nTimeVal;
	u16 nCountDown;
	bool bHasTimeElapsed;
} tTickTimer;

typedef struct
{
  u8 x;
  u8 y;
} Vec2;

typedef struct
{
	u8 x;
	u8 y;
	u8 w;
	u8 h;
} Rect;

extern u16 IN_JoyNow,IN_JoyOld;
extern const u8* pSrc;
extern const u8* pDst;
extern u8* pSrc8;
extern u8* pDst8;

extern u16* pSrc16;
extern u16* pDst16;

extern u16 LI,LX,LY,LJ;

extern void vsync();

extern void ErrorTrap(const char* errMsg);
extern void ErrorTrapEX(const char* errMsg, ...);

extern u16 Rnd(u16 value);
extern bool IsBitSet(u16 value,u16 flag);
extern void Vec2_Set(Vec2* pVec,u8 nx,u8 ny);

extern void Rect_Set(Rect* r,u8 x,u8 y,u8 w,u8 h);
extern bool Rect_HitVec2(Rect* pRec,Vec2 *pVec);

extern void IN_Clear();
extern void IN_Update();
extern bool IN_WasKeyPressed(u16 keyID);
extern bool IN_IsKeyPressed(u16 keyID);

extern void CString_Clear(CString* pStr);

extern void TickTimer_Init(tTickTimer *pTT,u16 myDelay);
extern bool TickTimer_Update(tTickTimer *pTT);
extern void TickTimer_Reset(tTickTimer *pTT);
#endif // UTILS_H
