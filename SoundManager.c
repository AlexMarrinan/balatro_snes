#include "SoundManager.h"

void Snd_Init()
{
	u8 I;
  spcBoot();
	spcAllocateSoundRegion(39);
  spcSetBank(&SOUNDBANK__);

  //load sfx
  spcLoad(MOD_TESTSONG); 
  vsync();
  // allocate around 10K of sound ram (39 256-byte blocks)

  // Load all effects
  // spcLoad(MOD_YTSFX1);
  // vsync();
  // spcStop();spcLoad(0);
	// for (I=0;I<eSFX_MAX;++I)
  // {
  //     spcLoadEffect(I);
  // }
}

void Snd_PlaySong(u8 id)
{
	static u8 I;
	spcStop();
	if(id==0)
	{
		spcLoad(MOD_TESTSONG);
		spcPlay(0);
	}
	// for (I=0;I<eSFX_MAX;++I)
  // {
  //     spcLoadEffect(I);
  // }
}

// void Snd_PlaySfx(u8 id,u8 pitch)
// {
//   spcEffect(pitch,id,248);
// }

void Snd_Update()
{
  spcProcess();
}
