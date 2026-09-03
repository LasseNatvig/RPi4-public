// VGAsim.h

#include <SDL3/SDL.h> 
#include <stdio.h>
#include <signal.h>
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define WINDOW_POS_X 1200 // Set position of window to where you want it on the screen 
#define WINDOW_POS_Y 50 // Works for windows, not Raspberry Pi

// Declare external variable
extern volatile int running;

#define BLACK 0
#define GREY 1 
#define RED 2
#define GREEN 3
#define YELLOW 4
#define BROWN 5
#define BLUE 6
#define WHITE 7


int VGAcolor(int c);
int initVGA();
void runVGA();
void drawRect(int x, int y, int width, int height,  Uint32 color);
void updateVGA();