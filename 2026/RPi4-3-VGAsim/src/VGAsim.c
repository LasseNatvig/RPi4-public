// TDT4258 - 2026 - multiversion C code emulating the CPUlator 320 x 240 VGA screen
// assuming SDL3 is installed

#include "VGAsim.h"
#include <SDL3/SDL.h>
#include <stdio.h>
#include <signal.h>  // for signal handling to allow Ctrl+C to exit the program

SDL_Window* window; 
SDL_Surface* screenSurface;

// Signal handler for making Ctrl+C work
volatile int running = 1;  // volatile prevents that the variable running is optimized away by the compiler, 
    // since it is modified in the signal handler which is called asynchronously by the operating system 
    // when SIGINT (Ctrl+C) is received

void signal_handler() {
    running = 0;  // This will cause the main loop to exit and clean up resources
}

void drawRect(int x, int y, int width, int height,  Uint32 color) {
  SDL_Rect rect = {x, y, width, height};
  SDL_FillSurfaceRect(screenSurface, &rect, color);
}

void updateVGA() {
    SDL_UpdateWindowSurface(window);  
}   

int VGAcolor(int colorNo) {
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

int initVGA() {
    window = NULL;   
    screenSurface = NULL;
    // Set up signal handler for Ctrl+C
    signal(SIGINT, signal_handler); 
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    window = SDL_CreateWindow("SDL simulated VGA (320x240 pix)", SCREEN_WIDTH,
                            SCREEN_HEIGHT, 0);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    #if defined(_WIN32) || defined(_WIN64)
        SDL_SetWindowPosition(window, WINDOW_POS_X, WINDOW_POS_Y); // works under windows, not supported on RPi (KMS/DRM/Wayland)
    #endif
    screenSurface = SDL_GetWindowSurface(window);
    return 0;     
}

void runVGA() {
    SDL_Event e;
    while (running) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_EVENT_QUIT) {
                running = 0;  // Exit the loop
            }
        }
        SDL_Delay(16); // Small delay to prevent high CPU usage
    }
    // Cleanup when exiting
    SDL_DestroyWindow(window);
    SDL_Quit();
}

