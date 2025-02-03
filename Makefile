ifeq ($(strip $(PVSNESLIB_HOME)),)
$(error "Please create an environment variable PVSNESLIB_HOME with path to its folder and restart application. (you can do it on windows with <setx PVSNESLIB_HOME "/c/snesdev">)")
endif

# BEFORE including snes_rules :
# list in AUDIOFILES all your .it files in the right order. It will build to generate soundbank file
AUDIOFILES := res/balatro_music.it
# then define the path to generate soundbank data. The name can be different but do not forget to update your include in .c file !
export SOUNDBANK := res/soundbank

include ${PVSNESLIB_HOME}/devkitsnes/snes_rules

.PHONY: all



#---------------------------------------------------------------------------------
# ROMNAME is used in snes_rules file
export ROMNAME := balatro_demake

SMCONVFLAGS	:= -s -o $(SOUNDBANK) -V -b 5
musics: $(SOUNDBANK).obj

all: musics  $(ROMNAME).sfc
all: $(ROMNAME).sfc

clean: cleanBuildRes cleanRom cleanGfx cleanAudio

#clean: cleanBuildRes cleanRom cleanGfx
