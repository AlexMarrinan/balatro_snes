#include "Utils.h"
#include "S2D.h"

u16 IN_JoyNow,IN_JoyOld;
const u8* pSrc;
const u8* pDst;
u8* pSrc8;
u8* pDst8;

u16* pSrc16;
u16* pDst16;
u16 LI,LX,LY,LJ;

CString zTextBuffer;

void ErrorTrap(const char* errMsg)
{
	setBrightness(15);
	S2D_FillStripX(S2D_BG_HUD,0,0,32,0);
	S2D_Print(S2D_BG_HUD,0,0,errMsg);

	do
	{
		vsync();
		S2D_Update();
	}while(TRUE);
}

void ErrorTrapEX(const char* errMsg, ...)
{
	va_list args;
	CString_Clear(&zTextBuffer);
	va_start(args, errMsg);
	vsnprintf(zTextBuffer.Data,CSTRING_MAX,errMsg,args);
	ErrorTrap(zTextBuffer.Data);
}

u16 Rnd(u16 value)
{
	return rand()%value;
}

bool IsBitSet(u16 var,u16 flag)
{
	if((var & flag)==flag)
		return true;

	return false;
}

void vsync()
{
	WaitForVBlank();
}

void Vec2_Set(Vec2* pVec,u8 nx,u8 ny)
{
	pVec->x=nx;
	pVec->y=ny;
}

void Rect_Set(Rect* r,u8 x,u8 y,u8 w,u8 h)
{
	r->x=x;
	r->y=y;
	r->w=w;
	r->h=h;
}

bool Rect_HitVec2(Rect* pRec,Vec2 *pVec)
{
	if((pVec->x>=pRec->x) && (pVec->x<(pRec->x+pRec->w)) && (pVec->y>=pRec->y) && (pVec->y<(pRec->y+pRec->h)))
	{
		return true;
	}

	return false;
}

void IN_Clear()
{
	IN_JoyOld=IN_JoyNow=0;
}

void IN_Update()
{
	IN_JoyOld=IN_JoyNow;
	IN_JoyNow=padsCurrent(0);
}

bool IN_WasKeyPressed(u16 keyID)
{
	if(IsBitSet(IN_JoyOld,keyID))
	{
		if(!IsBitSet(IN_JoyNow,keyID))
		{
			IN_JoyOld=0;
			return true;
		}
	}
	return false;
}

bool IN_IsKeyPressed(u16 keyID)
{
	if(IsBitSet(IN_JoyNow,keyID))
	{
		return true;
	}

	return false;
}

void CString_Clear(CString* pStr)
{
	pStr->Len=0;
	pStr->Data[0]=0;
}

void TickTimer_Init(tTickTimer *pTT,u16 myDelay)
{
	pTT->bHasTimeElapsed=false;
	pTT->nCountDown=myDelay;
	pTT->nTimeVal=myDelay;
}

void TickTimer_Reset(tTickTimer *pTT)
{
	pTT->nCountDown=pTT->nTimeVal;
	pTT->bHasTimeElapsed=false;
}

bool TickTimer_Update(tTickTimer *pTT)
{
	if(pTT->bHasTimeElapsed==false)
	{
		--pTT->nCountDown;
		if(pTT->nCountDown==0)
			pTT->bHasTimeElapsed=true;
	}

	return pTT->bHasTimeElapsed;
}
