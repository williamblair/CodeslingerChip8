/* How to show graphics */

// uses SDL 1.2
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include <stdio.h>

#include "Chip8.hpp"

#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED

class Display
{
public:
    // constructor/deconstructor
    Display(const int width, const int height, const char *title);
    ~Display(void);
    
    // clear screen
    void update(unsigned char data[320][640][3]);
    
    // check keys
    void pollEvents(Chip8 &chip);
    
    // recreate the surface from the given array
    //bool updateSurface(unsigned char data[320][640][3]);
private:
    SDL_Surface *m_WinSurface;
    
    int width, height;
};

#endif
