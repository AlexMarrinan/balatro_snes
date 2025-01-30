/*---------------------------------------------------------------------------------


    Simple console 'hello world' demo
    -- alekmaul


---------------------------------------------------------------------------------*/
#include <snes.h>
#define PAD short

#define TRUE 1
#define FALSE 0
#define SPRITE_SIZE 4

extern char tilfont, palfont;
extern char gfxpsrite, gfxpsrite_end;
extern char palsprite, palsprite_end;

//---------------------------------------------------------------------------------
int main(void)
{
    int spriteX = 0;
    int spriteY = 0;
    // Initialize SNES 
    consoleInit();

    // Initialize text console with our font
    consoleSetTextVramBGAdr(0x6800);
    consoleSetTextVramAdr(0x3000);
    consoleSetTextOffset(0x0100);
    consoleInitText(0, 16 * 2, &tilfont, &palfont);

    // Init Sprites gfx and palette with default size of 32x32
    oamInitGfxSet(&gfxpsrite, (&gfxpsrite_end - &gfxpsrite), &palsprite, (&palsprite_end - &palsprite), 0, 0x0000, OBJ_SIZE32_L64);

    // Define sprites parameters
    //every new sprite must be incremented by 4 (*SPRITE_SIZE)
    oamSet(0*SPRITE_SIZE, spriteX, spriteY, 3, 0, 0, 0, 0); // Put sprite in 100,100, with maximum priority 3 from tile entry 0, palette 0
    oamSetEx(0, OBJ_SMALL, OBJ_SHOW);
    
    oamSet(1*SPRITE_SIZE, 100, 175, 3, 0, 0, 0, 0);
    oamSetEx(1*SPRITE_SIZE, OBJ_SMALL, OBJ_SHOW);
    
    oamSet(2*SPRITE_SIZE, 125, 100, 3, 0, 0, 0, 0);
    oamSetEx(2*SPRITE_SIZE, OBJ_SMALL, OBJ_SHOW);

    // Init background
    bgSetGfxPtr(0, 0x2000);
    bgSetMapPtr(0, 0x6800, SC_32x32);

    // Now Put in 16 color mode and disable Bgs except current
    setMode(BG_MODE1, 0);
    bgSetDisable(1);    
    bgSetDisable(2);

    // Draw a wonderful text :P
    consoleDrawText(10, 10, "Hello World !");
    consoleDrawText(6, 14, "DEEZ NUTSSSSSSSSS");

    // Wait for nothing :P
    setScreenOn();
    
    PAD pad0;

    while (1)
    {
        //get player 0 controller
        pad0 = padsCurrent(0);

        if (pad0 & KEY_A) {
            consoleDrawText(6, 20, "KEY_A");
        }else{
            consoleDrawText(6, 20, "     ");
        }
        if (pad0 & KEY_B) {
            consoleDrawText(12, 20, "KEY_B");
        }else{
            consoleDrawText(12, 20, "     ");
        }
        if (pad0){
            // Update sprite with current pad
			if(pad0 & KEY_UP) {
				spriteY--;
			}
			if(pad0 & KEY_LEFT) {
				spriteX--;
            }
			if(pad0 & KEY_RIGHT) {
				spriteX++;
			}
			if(pad0 & KEY_DOWN) {
			    spriteY++;
			}
        }

        //set sprite at index 0 to spriteX, spriteY coordinates
        oamSetXY(0*SPRITE_SIZE, spriteX, spriteY); 

        WaitForVBlank();
    }
    return 0;
}