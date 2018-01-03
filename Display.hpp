/* How to show graphics */

#include <SDL2/SDL.h>

#include <stdio.h>

#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED

class Display
{
public:
    // constructor/deconstructor
    Display(const int width, const int height, const char *title);
    ~Display(void);
    
    // clear screen and get keys
    void update(void);
private:
    SDL_Window  *m_Window;
    SDL_Surface *m_Surface;
    
    int width, height;
};

#endif
