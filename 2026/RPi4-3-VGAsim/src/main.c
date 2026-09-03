// TDT4258 - 2026 - test code for RPi4-3-VGAsim
#include <stdio.h>
// #define CPULATOR  // uncomment this on CPULATOR

#ifdef CPULATOR
	void initVGA() {} // dummy function 
	void updateVGA() {} // dummy function
    long long VGAcolor(int colorNo) {
        switch(colorNo) {
            case 0: return 0x0; // Black   
            case 1: return 0x0000738E; // Grey
            case 2: return 0x0000F0F0; // Red
            case 3: return 0x00000F0F; // Green
            case 4: return 0x0000FFE0; // Yellow
            case 5: return 0x0000A144; // Brown
            case 6: return 0x000000FF; // Blue
            case 7: return 0x0000FFFF; // White
            default: return 0x0;      // Default to black
        }
    }
	void setPixel(unsigned int x_coord, unsigned int y_coord, unsigned int color); // to avoid warning
	void blankScreen(); // the same

    asm("setPixel: \n\t" // assumes R0 = x-coord, R1 = y-coord, R2 = colorvalue
        "LDR R3, =#0xc8000000 \n\t" // VGAaddress
        "LSL R1, R1, #10 \n\t"
        "LSL R0, R0, #1 \n\t"
        "ADD R1, R0 \n\t"
        "STRH R2, [R3,R1] \n\t"
        "BX LR");
    asm("blankScreen: \n\t"
        "    PUSH {LR} \n\t"
        "    PUSH {R4, R5} \n\t"
        "    LDR R2, =0x0000FFFF \n\t" 
        "    MOV R4, #0 \n\t"
        "x_loop:\n\t"
        "    CMP R4, #320 \n\t"
        "    BEQ end_x_loop \n\t"
        "    MOV R5, #0 \n\t"
        "y_loop:\n\t"
        "    CMP R5, #240 \n\t"
        "    BEQ end_y_loop \n\t"
        "    MOV R0, R4 \n\t"
        "    MOV R1, R5\n\t"
        "    BL setPixel \n\t"
        "    ADD R5, R5, #1 \n\t"
        "    B y_loop \n\t"
        "end_y_loop:\n\t"
        "    ADD R4, R4, #1 \n\t"
        "    B x_loop \n\t"
        "end_x_loop:\n\t"
        "    POP {R4,R5}\n\t"
        "    POP {LR} \n\t"
        "    BX LR");
#else
	#include "VGAsim.h"

    void setPixel(unsigned int x_coord, unsigned int y_coord, int color){
        drawRect(x_coord, y_coord, 1, 1, color);
    }
    void blankScreen() { // Basic and inefficient way to clear the screen, but it works
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            for (int y = 0; y < SCREEN_HEIGHT; y++) {
                    setPixel(x, y, VGAcolor(WHITE)); 
            }
        }   
    }
#endif

const int bigPixelSize = 20; // Size of the big pixel in terms of small pixels
  // constant not in vGAsim.h since that file is not included in the CPUlator code (cannot include files)
void bigPixel(int x, int y, int colorNo) {
    if ( (x < 0) || (x > 7) || (y < 0) || (y > 7) ) {
        printf("*** Error: Illegal parameters to bigPixel: ");
        printf("%d %d\n", x, y); 
    } else {
        for (int ix = x*bigPixelSize; ix < (x+1)*bigPixelSize; ix++) {
            for (int iy = y*bigPixelSize; iy < (y+1)*bigPixelSize; iy++) {
                setPixel(ix, iy, VGAcolor(colorNo)); 
            }
        }
    }
}

int main() {
    initVGA();
    blankScreen();
    for (int i = 0; i < 8; i++) {
        bigPixel(i, i, (1+i) % 8); 
    }
    updateVGA();
    runVGA();
    return 0;
}