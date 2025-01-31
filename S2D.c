#include "S2D.h"

u16 S2D_TileAtrib;
u16 S2D_TileMapBG1[S2D_MAPSIZE];
u16 S2D_TileMapBG2[S2D_MAPSIZE];
u16 S2D_TileMapBG3[S2D_MAPSIZE];
//these are checked each update, and if changed, then map data is copied to vram
bool zS2D_BG1changed=false;
bool zS2D_BG2changed=false;
bool zS2D_BG3changed=false;

//local vars only for S2D
u8 zS2D_Var0;
u8 zS2D_Var1;
CString zS2D_Str;
static u16 zS2D_Font16base;
static u16 zS2D_NumDHbase;

void S2D_Init(u16 Font16_TileStart,u16 NumDH_TileStart)
{
  S2D_TileAtrib=0;

	zS2D_Font16base=Font16_TileStart;
	zS2D_NumDHbase=NumDH_TileStart;

  bgSetMapPtr(0,S2D_BG_1_VRAM_MAP_ADDR,SC_32x32);
  bgSetMapPtr(1,S2D_BG_2_VRAM_MAP_ADDR,SC_32x32);
  bgSetMapPtr(2,S2D_BG_3_VRAM_MAP_ADDR,SC_32x32);

  zS2D_BG1changed=false;
  zS2D_BG2changed=false;
  zS2D_BG3changed=false;

  bgSetGfxPtr(0,S2D_BG_1_VRAM_TILES_ADDR);
  bgSetGfxPtr(1,S2D_BG_2_VRAM_TILES_ADDR);
  bgSetGfxPtr(2,S2D_BG_3_VRAM_TILES_ADDR);
  //vsync();
  //WaitForVBlank();
  S2D_Cls(S2D_BG1);
  S2D_Cls(S2D_BG2);
  S2D_Cls(S2D_BG3);

  S2D_Update();

}

void S2D_SetAtribTilePriority()
{
  S2D_TileAtrib|=S2D_TAR_BIT_PRIORITY;
}

void S2D_SetAtribPalette(u8 paletteID)
{
	S2D_TileAtrib=0;
  S2D_TileAtrib&=S2D_TAR_NOT_PALETTES_MASK;
  S2D_TileAtrib|=(paletteID<<S2D_TAR_PALETTE_BITSHIFT);
}

void S2D_SetAtribPalAndOnTop(u8 paletteID)
{
	S2D_TileAtrib=S2D_TILE_ATTR(paletteID,1,0,0);
}

void S2D_SetAtribTileMirrorX(bool bIsFlipped)
{
  if(bIsFlipped)
  {
    if(IsBitSet(S2D_TileAtrib,S2D_TAR_BIT_FLIP_H)==false)
    {
      S2D_TileAtrib|=S2D_TAR_BIT_FLIP_H;
      return;
    }
    return;
  }

  if(IsBitSet(S2D_TileAtrib,S2D_TAR_BIT_FLIP_H)==false)
  {
    S2D_TileAtrib^=S2D_TAR_BIT_FLIP_H;
  }
}

void S2D_SetAtribTileMirrorY(bool bIsFlipped)
{
  if(bIsFlipped)
  {
    if(IsBitSet(S2D_TileAtrib,S2D_TAR_BIT_FLIP_V)==false)
    {
      S2D_TileAtrib|=S2D_TAR_BIT_FLIP_V;
      return;
    }
    return;
  }

  if(IsBitSet(S2D_TileAtrib,S2D_TAR_BIT_FLIP_V)==false)
  {
    S2D_TileAtrib^=S2D_TAR_BIT_FLIP_V;
  }
}

///Note No error checking for tile in actual tilemap is done
void S2D_TilePut(u8 layerID,u8 tx,u8 ty,u16 tile)
{

  if(ty>31)
  {
    ty-=32;
    LI=2048u;
    LI+=(ty<<5);
  }
  else
  {
    LI=(ty<<5);
  }

  if(tx&32)
  {
    LI+=1024u;
    tx&=31;
  }

  LI+=tx;
  tile|=S2D_TileAtrib;
  switch(layerID)
  {
  case S2D_BG1:
    S2D_TileMapBG1[LI]=tile;
    zS2D_BG1changed=true;
    break;
  case S2D_BG2:
    S2D_TileMapBG2[LI]=tile;
    zS2D_BG2changed=true;
    break;
  case S2D_BG3:
    S2D_TileMapBG3[LI]=tile;
    zS2D_BG3changed=true;
    break;
  }
}

void S2D_TilePut16(u8 layerID,u8 tx,u8 ty,u16 tile)
{
  static u8 tw,th,toffset;
  toffset=S2D_MAP_W-2;
  tile|=S2D_TileAtrib;
  if(ty>31)
  {
    ty-=32;
    LY=2048u;
    LY+=(ty<<5);
  }
  else
  {
    LY=(ty<<5);
  }

  if(tx&32)
  {
    LY+=1024u;
    tx&=31;
  }

  LY+=tx;
  switch(layerID)
  {
  case S2D_BG1:
    pSrc16=&S2D_TileMapBG1[LY];
    zS2D_BG1changed=true;
    break;
  case S2D_BG2:
    pSrc16=&S2D_TileMapBG2[LY];
    zS2D_BG2changed=true;
    break;
  case S2D_BG3:
    pSrc16=&S2D_TileMapBG3[LY];
    zS2D_BG3changed=true;
    break;
  }

  for(th=0;th<2;++th)
  {
    for(tw=0;tw<2;++tw)
    {
      *pSrc16=tile;
      ++tile;
      ++pSrc16;
    }
      pSrc16+=toffset;
  }
}

void S2D_TilePut8x16(u8 layerID,u8 tx,u8 ty,u16 tile)
{

  tile|=S2D_TileAtrib;
  if(ty>31)
  {
    ty-=32;
    LY=2048u;
    LY+=(ty<<5);
  }
  else
  {
    LY=(ty<<5);
  }

  if(tx&32)
  {
    LY+=1024u;
    tx&=31;
  }

  LY+=tx;
  switch(layerID)
  {
  case S2D_BG1:
    pSrc16=&S2D_TileMapBG1[LY];
    zS2D_BG1changed=true;
    break;
  case S2D_BG2:
    pSrc16=&S2D_TileMapBG2[LY];
    zS2D_BG2changed=true;
    break;
  case S2D_BG3:
    pSrc16=&S2D_TileMapBG3[LY];
    zS2D_BG3changed=true;
    break;
  }

   *pSrc16=tile;
   ++tile;
    pSrc16+=32;
   *pSrc16=tile;
}

void S2D_TileClr16(u8 layerID,u8 tx,u8 ty)
{
	static u8 tw,th,toffset;
  toffset=S2D_MAP_W-2;
  if(ty>31)
  {
    ty-=32;
    LY=2048u;
    LY+=(ty<<5);
  }
  else
  {
    LY=(ty<<5);
  }

  if(tx&32)
  {
    LY+=1024u;
    tx&=31;
  }

  LY+=tx;
  switch(layerID)
  {
  case S2D_BG1:
    pSrc16=&S2D_TileMapBG1[LY];
    zS2D_BG1changed=true;
    break;
  case S2D_BG2:
    pSrc16=&S2D_TileMapBG2[LY];
    zS2D_BG2changed=true;
    break;
  case S2D_BG3:
    pSrc16=&S2D_TileMapBG3[LY];
    zS2D_BG3changed=true;
    break;
  }

  for(th=0;th<2;++th)
  {
    for(tw=0;tw<2;++tw)
    {
      *pSrc16=0;
      ++pSrc16;
    }
    pSrc16+=toffset;
  }
}

void S2D_TilePut32(u8 layerID,u8 tx,u8 ty,u16 tile)
{
  static u8 tw,th,toffset;
  toffset=S2D_MAP_W-4;

  if(ty>31)
  {
    ty-=32;
    LY=2048u;
    LY+=(ty<<5);
  }
  else
  {
    LY=(ty<<5);
  }

  if(tx&32)
  {
    LY+=1024u;
    tx&=31;
  }

  tile|=S2D_TileAtrib;
  LY+=tx;
  switch(layerID)
  {
  case S2D_BG1:
    pSrc16=&S2D_TileMapBG1[LY];
    zS2D_BG1changed=true;
    break;
  case S2D_BG2:
    pSrc16=&S2D_TileMapBG2[LY];
    zS2D_BG2changed=true;
    break;
  case S2D_BG3:
    pSrc16=&S2D_TileMapBG3[LY];
    zS2D_BG3changed=true;
    break;
  }

  for(th=0;th<4;++th)
  {
    for(tw=0;tw<4;++tw)
    {
      *pSrc16=tile;
      ++tile;
      ++pSrc16;
    }
      pSrc16+=toffset;
  }
}

void S2D_TileClr(u8 layerID,u8 tx,u8 ty)
{
  LI=(ty<<5);
  LI+=tx;

  switch(layerID)
  {
  case S2D_BG1:
    S2D_TileMapBG1[LI]=0;
    zS2D_BG1changed=true;
    break;
  case S2D_BG2:
    S2D_TileMapBG2[LI]=0;
    zS2D_BG2changed=true;
    break;
  case S2D_BG3:
    S2D_TileMapBG3[LI]=0;
    zS2D_BG3changed=true;
    break;
  }
}

void S2D_TileClr8x16(u8 layerID,u8 tx,u8 ty)
{
  LI=(ty<<5);
  LI+=tx;

  switch(layerID)
  {
  case S2D_BG1:
    S2D_TileMapBG1[LI]=0;
    S2D_TileMapBG1[LI+32]=0;
    zS2D_BG1changed=true;
    break;
  case S2D_BG2:
    S2D_TileMapBG2[LI]=0;
    S2D_TileMapBG2[LI+32]=0;
    zS2D_BG2changed=true;
    break;
  case S2D_BG3:
    S2D_TileMapBG3[LI]=0;
    S2D_TileMapBG3[LI+32]=0;
    zS2D_BG3changed=true;
    break;
  }
}

void S2D_FillStripX(u8 layerID,u8 tx,u8 ty,u8 width,u16 tileID)
{
  LY=(ty<<5);
  LY+=tx;
  tileID|=S2D_TileAtrib;
  switch(layerID)
  {
  case S2D_BG1:
    pSrc16=&S2D_TileMapBG1[LY];
    zS2D_BG1changed=true;
    break;
  case S2D_BG2:
    pSrc16=&S2D_TileMapBG2[LY];
    zS2D_BG2changed=true;
    break;
  case S2D_BG3:
    pSrc16=&S2D_TileMapBG3[LY];
    zS2D_BG3changed=true;
    break;
  }

  LI=0;
  do
  {
    *pSrc16=tileID;
    ++pSrc16;
    ++LI;
  }while(LI<width);

}

void S2D_FillStripY(u8 layerID,u8 tx,u8 ty,u8 height,u16 tileID)
{
  LX=S2D_MAP_W;
  LY=(ty<<5);
  LY+=tx;
  tileID|=S2D_TileAtrib;

  switch(layerID)
  {
  case S2D_BG1:
    pSrc16=&S2D_TileMapBG1[LY];
    zS2D_BG1changed=true;
    break;
  case S2D_BG2:
    pSrc16=&S2D_TileMapBG2[LY];
    zS2D_BG2changed=true;
    break;
  case S2D_BG3:
    pSrc16=&S2D_TileMapBG3[LY];
    zS2D_BG3changed=true;
    break;
  }

  LI=0;

  do
  {
    *pSrc16=tileID;
    pSrc16+=LX;
    ++LI;
  }while(LI<height);

}

void S2D_Cls(u8 layerID)
{
  pDst16=NULL;
  switch(layerID)
  {
    case S2D_BG1:
      pDst16=&S2D_TileMapBG1[0];
      zS2D_BG1changed=true;
    break;
    case S2D_BG2:
      pDst16=&S2D_TileMapBG2[0];
      zS2D_BG2changed=true;
    break;
    case S2D_BG3:
      pDst16=&S2D_TileMapBG3[0];
      zS2D_BG3changed=true;
    break;
  };

  LI=0;
  LJ=S2D_MAPSIZE;

  do
  {
    *pDst16=S2D_TileAtrib;
    ++pDst16;
    ++LI;
  }while(LI<LJ);
}

void S2D_Print(u8 layerID,u8 tx,u8 ty,const char* msg)
{
  static u8 tile;

  LY=(ty<<5);
  LY+=tx;
  switch(layerID)
  {
  case S2D_BG1:
    pSrc16=&S2D_TileMapBG1[LY];
    zS2D_BG1changed=true;
    break;
  case S2D_BG2:
    pSrc16=&S2D_TileMapBG2[LY];
    zS2D_BG2changed=true;
    break;
  case S2D_BG3:
    pSrc16=&S2D_TileMapBG3[LY];
    zS2D_BG3changed=true;
    break;
  }

  do
  {
    tile=*msg;
    ++msg;
    *pSrc16=(tile|S2D_TileAtrib);
    ++pSrc16;
  }while(tile>0);
}

void S2D_PrintV(u8 layerID,u8 tx,u8 ty,const char* msg,...)
{
  va_list args;
  CString_Clear(&zS2D_Str);
  zS2D_Var0=0;
  va_start(args,msg);
  vsnprintf(zS2D_Str.Data,CSTRING_MAX,msg,args);
  va_end(args);

  S2D_Print(layerID,tx,ty,zS2D_Str.Data);
  //for(zS2D_Var0=0;zS2D_Var0<10;++zS2D_Var0)
  //{
    //zS2D_Str.Data[zS2D_Var0]='A';
  //}
  //S2D_Print(layerID,tx,ty,"MY MSG");
//  S2D_Print(S2D_BG3,0,0,"zS2D_Str.Data");
}

void S2D_PrintCentre(u8 layerID,u8 ty,const char* msg)
{
	static u8 Len,tile;
	Len=0;
	pDst=&msg[0];
	do
	{
		tile=*pDst++;
		++Len;
	}while(tile>0);
	if(Len>32)
		Len=32;
	Len>>=1;
	tile=16-Len;
	S2D_Print(layerID,tile,ty,msg);
}

void S2D_Print16(u8 layerID,u8 tx,u8 ty,const char* msg)
{
	static u8 tile;
	static u16 block;

  do
  {
    tile=*msg;
    ++msg;
		if((tile>'@') && (tile<'['))
		{
			tile-=65;
			tile<<=2;
			block=zS2D_Font16base;
			block+=tile;
			S2D_TilePut16(layerID,tx,ty,block);
			tile=1;
		}
		else
		{
			S2D_TileClr16(layerID,tx,ty);
		}
		++tx;
		++tx;
  }while(tile>0);
}

void S2D_Print16Centre(u8 layerID,u8 ty,const char* msg)
{
	static u8 Len,tile;
	Len=0;
	pDst=&msg[0];
	do
	{
		tile=*pDst++;
		++Len;
	}while(tile>0);
	if(Len>16)
		Len=16;
	tile=16-Len;
	S2D_Print16(layerID,tile,ty,msg);
}

void S2D_PrintDHNum8(u8 layerID,u8 x,u8 y,u8 val8,bool bDrawZeros)
{
	static u8 h,t;///Hundreds Tens Units
	static u16 tile;
	h=t=0;
	while(val8>=100)
	{
		++h;
		val8-=100;
	}

	while(val8>=10)
	{
		++t;
		val8-=10;
	}

	//val8 now hold only units 0-9
	if(h>0)
	{
		tile=h;
		tile<<=1;
		tile+=zS2D_NumDHbase;
		h=1;
		//h=1 we use this to now indicate that a digit is in '100 counter' so ten value must be drawn if value of 0
		S2D_TilePut8x16(layerID,x,y,tile);
		++x;
	}
	else
	{
		if(bDrawZeros)
		{
			S2D_TilePut8x16(layerID,x,y,zS2D_NumDHbase);
			++x;
		}
	}

	if((t>0) || (h>0))
	{
		tile=t;
		tile<<=1;
		tile+=zS2D_NumDHbase;
		S2D_TilePut8x16(layerID,x,y,tile);
		++x;

	}
	else
	{
		if(bDrawZeros)
		{
			S2D_TilePut8x16(layerID,x,y,zS2D_NumDHbase);
			++x;
		}
	}

	tile=val8;
	tile<<=1;
	tile+=zS2D_NumDHbase;
	S2D_TilePut8x16(layerID,x,y,tile);
}

void S2D_PrintDHNum16(u8 layerID,u8 x,u8 y,u16 val16,bool bDrawZeros)
{
	static u8 a,b,h,t,u;///Hundreds Tens Units
	static u16 tile;
	a=b=h=t=0;
	while(val16>=10000)
	{
		++a;
		val16-=10000;
	}

	while(val16>=1000)
	{
		++b;
		val16-=1000;
	}

	while(val16>=100)
	{
		++h;
		val16-=100;
	}
	u=(u8)val16;

	while(u>=10)
	{
		++t;
		u-=10;
	}

	if(a>0)
	{
		tile=a;
		tile<<=1;
		tile+=zS2D_NumDHbase;
		S2D_TilePut8x16(layerID,x,y,tile);
		++x;
		a=1;
		//a=1 we use this to now indicate that a digit is in '100 counter' so ten value must be drawn if value of 0
	}
	else if(bDrawZeros)
	{
		if(bDrawZeros)
		{
			S2D_TilePut8x16(layerID,x,y,zS2D_NumDHbase);
			++x;
		}
	}

	if((a>0) || (b>0))
	{
		tile=b;
		tile<<=1;
		tile+=zS2D_NumDHbase;
		S2D_TilePut8x16(layerID,x,y,tile);
		++x;
		a=1;
	}
	else if(bDrawZeros)
	{
		if(bDrawZeros)
		{
			S2D_TilePut8x16(layerID,x,y,zS2D_NumDHbase);
			++x;
		}
	}

	//val8 now hold only units 0-9
	if((h>0) || (a>0))
	{
		tile=h;
		tile<<=1;
		tile+=zS2D_NumDHbase;
		S2D_TilePut8x16(layerID,x,y,tile);
		++x;
		a=1;
	}
	else if(bDrawZeros)
	{
		if(bDrawZeros)
		{
			S2D_TilePut8x16(layerID,x,y,zS2D_NumDHbase);
			++x;
		}
	}

	if((t>0) || (a>0))
	{
		tile=t;
		tile<<=1;
		tile+=zS2D_NumDHbase;
		S2D_TilePut8x16(layerID,x,y,tile);
		++x;
		a=1;
	}
	else if(bDrawZeros)
	{
		if(bDrawZeros)
		{
			S2D_TilePut8x16(layerID,x,y,zS2D_NumDHbase);
			++x;
		}
	}

	tile=u;
	tile<<=1;
	tile+=zS2D_NumDHbase;
	S2D_TilePut8x16(layerID,x,y,tile);
}

void S2D_SetPaletteCol4(u8 palID,u8 palCount,u16* pPalData)
{
  LI=(u16)(palID*4);
  LJ=(u16)(palCount*8);
  pSrc8=(u8*)&pPalData[0];
  dmaCopyCGram(pSrc8,LI,LJ);
}

void S2D_SetPaletteCol16(u8 palID,u8 colorCount,u16* pPalData)
{
  LI=(u16)(palID<<4);
  LJ=(u16)(colorCount<<1);
  pSrc8=(u8*)&pPalData[0];
  dmaCopyCGram(pSrc8,LI,LJ);
}

//expects S2D_BG1 to be 4bpp tiles
void S2D_SetTileDataCol4(u16 tileID,u16 tileCount,u8* pTileData)
{
  LI=tileID<<3u; //tileID * 8 - memory is word aligned in vram - would be 16 bytes per tile at 2bpp - but is 8 due to doubling up as words
  LJ=tileCount<<4u;
  LI+=S2D_BG_3_VRAM_TILES_ADDR;
  dmaCopyVram(pTileData,LI,LJ);
}

//expects S2D_BG0 to be 16 colour tiles
void S2D_SetTileDataCol16(u8 BGid,u16 tileID,u16 tileCount,u8* pTileData)
{

  LJ=tileCount<<5u;
  if(BGid==S2D_BG1)
  {
    LI=S2D_BG_1_VRAM_TILES_ADDR;
  }
  else
    LI=S2D_BG_2_VRAM_TILES_ADDR;

  LI|=(tileID<<4u); //tileID * 8 - memory is word aligned in vram - would be 16 bytes per tile at 2bpp - but is 8 due to doubling up as words
  dmaCopyVram(pTileData,LI,LJ);
}

void S2D_Update()
{

    if(zS2D_BG1changed)
    {
      pSrc8=(u8*)&S2D_TileMapBG1[0];
      dmaCopyVram(pSrc8,S2D_BG_1_VRAM_MAP_ADDR,2048); //size is 1024 * 2 as map is in words not bytes
      zS2D_BG1changed=false;
    }

    if(zS2D_BG2changed)
    {
      pSrc8=(u8*)&S2D_TileMapBG2[0];
      dmaCopyVram(pSrc8,S2D_BG_2_VRAM_MAP_ADDR,2048); //size is 4096 * 2 as map is in words not bytes
      zS2D_BG2changed=false;
    }

    if(zS2D_BG3changed)
    {
      pSrc8=(u8*)&S2D_TileMapBG3[0];
      dmaCopyVram(pSrc8,S2D_BG_3_VRAM_MAP_ADDR,2048); //size is 1024 * 2 as map is in words not bytes
      zS2D_BG3changed=false;
    }
}
