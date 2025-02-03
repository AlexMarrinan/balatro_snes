;************************************************
; snesmod soundbank data                        *
; total size:      39946 bytes                  *
;************************************************

.include "hdr.asm"

.BANK 5
.SECTION "SOUNDBANK0" ; need dedicated bank(s)

SOUNDBANK__0:
.incbin "res/soundbank.bnk" read $8000
.ENDS

.BANK 6
.SECTION "SOUNDBANK1" ; need dedicated bank(s)

SOUNDBANK__1:
.incbin "res/soundbank.bnk" skip $8000
.ENDS

