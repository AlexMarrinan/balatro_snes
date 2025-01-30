.include "hdr.asm"

.section ".rodata1" superfree

tilfont:
.incbin "pvsneslibfont.pic"

palfont:
.incbin "pvsneslibfont.pal"

gfxpsrite:
.incbin "card.pic"
gfxpsrite_end:

palsprite:
.incbin "card.pal"
palsprite_end:

.ends
