.include "hdr.asm"

.section ".rodata1" superfree
dbSpr_T_chick64:	.incbin "res/Chick64.pic" ;4.1k
dbSpr_P_chick64:	.incbin "res/Chick64.pal" ;28 bytes = 14 colours

dbSpr_T_Pidg64:		.incbin "res/Pidg64.pic" ;8.2k
dbSpr_P_Pidg64:		.incbin "res/Pidg64.pal" ;22 bytes = 11 colours

dbSpr_T_menubg:		.incbin "res/menu_bg.dat.pic" ;8.2k
dbSpr_P_menubg:		.incbin "res/menu_bg.dat.pal" ;8.2k

dbSpr_T_sprites32:	.incbin "res/sprites32.pic" ;8.2k
dbSpr_P_sprites32:	.incbin "res/sprites32.pal" ;32 bytes = 16 colours

dbTiles_T_font16:		.incbin "res/Font1_16col.pic"; 4.1k
dbTiles_P_font16:		.incbin "res/Font1_16col.pal"; 8bytes = 4 colours

.ends

