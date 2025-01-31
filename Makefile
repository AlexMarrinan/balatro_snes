ifeq ($(strip $(PVSNESLIB_HOME)),)
$(error "Please create an environment variable PVSNESLIB_HOME with path to its folder and restart application. (you can do it on windows with <setx PVSNESLIB_HOME "/c/snesdev">)")
endif

# BEFORE including snes_rules :
# list in AUDIOFILES all your .it files in the right order. It will build to generate soundbank file
#AUDIOFILES :=
# then define the path to generate soundbank data. The name can be different but do not forget to update your include in .c file !
#export SOUNDBANK := res/soundbank

include ${PVSNESLIB_HOME}/devkitsnes/snes_rules

.PHONY: all

# to build musics, define SMCONVFLAGS with parameters you want
#SMCONVFLAGS	:= -s -o $(SOUNDBANK) -v -b 5
#musics: $(SOUNDBANK).obj

#---------------------------------------------------------------------------------
# ROMNAME is used in snes_rules file
export ROMNAME := balatro_demake

#all: musics bitmaps $(ROMNAME).sfc

all: $(ROMNAME).sfc

#clean: cleanBuildRes cleanRom cleanGfx cleanAudio

clean: cleanBuildRes cleanRom cleanGfx
