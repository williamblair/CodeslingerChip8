/* How to show graphics */

#include "Display.hpp"

/* Constructor */
Display::Display(const int width, const int height, const char *title)
{
    this->width = width; this->height = height;
    
    // Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "Failed to init SDL: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    
    // create the window
    m_Window = SDL_CreateWindow(
        title, 
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        width, height,
        SDL_WINDOW_SHOWN
    );
    if(!m_Window)
    {
        fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    
    // get the window surface
    m_Surface = SDL_GetWindowSurface(m_Window);
    if(!m_Surface)
    {
        fprintf(stderr, "Failed to get window surface: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    
    
}

/* Deconstructor */
Display::~Display(void)
{
    if(m_Surface)                   SDL_FreeSurface(m_Surface);
    if(m_Window)                    SDL_DestroyWindow(m_Window);
    if(SDL_WasInit(SDL_INIT_VIDEO)) SDL_Quit();
}

/* Update screen and handle keys */
void Display::update(void)
{
    // check keys
    SDL_Event e;
    while(SDL_PollEvent(&e))
    {
        // x out the window
        if(e.type == SDL_QUIT) exit(EXIT_SUCCESS);
        
        // TODO - add escape key exit
    }
    
    // clear the screen
    SDL_FillRect(m_Surface, NULL, SDL_MapRGB(m_Surface->format, 255, 255, 255));
    
    // draw stuff (specifically pixels)
    
    // flip the screen
    SDL_UpdateWindowSurface(m_Window);
    
    // Delay
    // test
    // SDL_Delay(1000.0f/60.0f);
}
