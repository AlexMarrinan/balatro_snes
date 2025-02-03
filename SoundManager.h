#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <snes.h>
#include "soundbank.h"
#include "Utils.h"

//can only get a small amount of sfx working from .it files
//note that they use channel 8 internally

enum
{
	eSFX_Go=0,
	eSFX_Yeah,
	eSFX_BoulderDrop,
	eSFX_Gem,
	eSFX_Bell,
	eSGX_Chirp,
	eSFX_MAX
};

// soundbank that are declared in soundbank.asm
extern char SOUNDBANK__1, SOUNDBANK__0;

extern void Snd_Init();
extern void Snd_PlaySfx(u8 id,u8 pitch);
extern void Snd_PlaySong(u8 id);
extern void Snd_Update();

#endif // SOUNDMANAGER_H
