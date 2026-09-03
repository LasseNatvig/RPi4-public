// TDT4258 - 2026 - multiversion C code emulating the CPUlator 320 x 240 VGA screen
// This file is used in the RPi4 example 3 (VGAsim, SDL2/SDL3 multi-version code, introduced in Lecture 4 )
// assuming SDL3 is installed. SDL3 is NOT part of the reading list for course TDT4258

#include "VGAsim.h"

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
    // Windows console control handler for Ctrl+C
    BOOL WINAPI console_ctrl_handler(DWORD ctrl_type) {
        if (ctrl_type == CTRL_C_EVENT || ctrl_type == CTRL_CLOSE_EVENT) {
            running = 0;
            return TRUE;  // Signal handled
        }
        return FALSE;  // Signal not handled
    }
#endif

SDL_Window* window; // Pointer to the OS window, including title bar, borders etc. 
SDL_Surface* screenSurface; // Pointer to pixel/image data stored in memory, that can be displayed inside the window

// Signal handler for making Ctrl+C work
volatile int running = 1;  // volatile prevents that the variable running is optimized away by the compiler, 
    // since it is modified in the signal handler which is called asynchronously by the operating system 
    // when SIGINT (Ctrl+C) is received

void signal_handler() {  // Is called if user presses Ctrl+C in the terminal window. 
    running = 0;  // Signal handler for Ctrl+C (works on Unix-like systems)
}

void drawRect(int x, int y, int width, int height,  Uint32 color) { 
  // drawas a rectangle at given position with given width and height, filled with given color
  SDL_Rect rect = {x, y, width, height};
  SDL_FillSurfaceRect(screenSurface, &rect, color); // writes in memory, but does not update the screen. 
}

void updateVGA() {
    SDL_UpdateWindowSurface(window);  
}   

int VGAcolor(int colorNo) {  // Maps from color number to RGB value, using SDL_MapSurfaceRGB to convert to the pixel format of the screen surface
    switch(colorNo) {
        case BLACK: return SDL_MapSurfaceRGB(screenSurface, 0, 0, 0);       
        case GREY: return SDL_MapSurfaceRGB(screenSurface, 128, 128, 128);  
        case RED: return SDL_MapSurfaceRGB(screenSurface, 255, 0, 0);      
        case GREEN: return SDL_MapSurfaceRGB(screenSurface, 0, 255, 0);      
        case YELLOW: return SDL_MapSurfaceRGB(screenSurface, 255, 255, 0);    
        case BROWN: return SDL_MapSurfaceRGB(screenSurface, 139, 69, 19);    
        case BLUE: return SDL_MapSurfaceRGB(screenSurface, 0, 0, 255);      
        case WHITE: return SDL_MapSurfaceRGB(screenSurface, 255, 255, 255);  
        default: return SDL_MapSurfaceRGB(screenSurface, 0, 0, 0);      // Default to black
    }
}

int initVGA() {  // Initialize VGA simulation 
    window = NULL;   
    screenSurface = NULL;

    // Set up signal handler for Ctrl+C
    signal(SIGINT, signal_handler); 
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Set up console control handler for Windows
    #if defined(_WIN32) || defined(_WIN64)
        SetConsoleCtrlHandler(console_ctrl_handler, TRUE);
    #endif

    // Create the window
    window = SDL_CreateWindow("SDL simulated VGA (320x240 pix)", SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    #if defined(_WIN32) || defined(_WIN64)
        SDL_SetWindowPosition(window, WINDOW_POS_X, WINDOW_POS_Y); // works under windows, not supported on RPi
    #endif
    screenSurface = SDL_GetWindowSurface(window);
    return 0;     
}

void runVGA() {  // only handles event processing, drawing is done via drawRect and updateVGA
    SDL_Event e;
    while (running) {
        while (SDL_PollEvent(&e) != 0) { 
            if (e.type == SDL_EVENT_QUIT) { // an event was received
                running = 0;  // Exit the loop
            }
        }
        SDL_Delay(16); // Small delay 16 milliseconds to prevent high CPU usage
    }
    // Cleanup when exiting
    SDL_DestroyWindow(window);
    SDL_Quit();
}

