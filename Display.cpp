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
    // 8 is BPP - 8 bits per pixel
    m_WinSurface = SDL_SetVideoMode(width, height, 8, SDL_OPENGL);
    if(!m_WinSurface)
    {
        fprintf(stderr, "Failed to create SDL window: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    
    // set the window title
    SDL_WM_SetCaption(title, NULL);
    
    // set opengl settings
    glViewport(0, 0, width, height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1.0f, -1.0f);
    glClearColor(0,0,0,1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glShadeModel(GL_FLAT);
    
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST); // 2D - no need for depth testing
    glDisable(GL_CULL_FACE);
    glDisable(GL_DITHER);
    glDisable(GL_BLEND);
    
}

/* Deconstructor */
Display::~Display(void)
{
    if(m_WinSurface)                SDL_FreeSurface(m_WinSurface);
    if(SDL_WasInit(SDL_INIT_VIDEO)) SDL_Quit();
}

/* Recreate the display surface */
/*bool Display::updateSurface(unsigned char data[64][32])
{
    
    
    return true;
}*/

/* Update screen and handle keys */
void Display::update(unsigned char data[320][640][3])
{
    // check keys
    SDL_Event e;
    while(SDL_PollEvent(&e))
    {
        // x out the window
        if(e.type == SDL_QUIT) exit(EXIT_SUCCESS);
        
        // TODO - add escape key exit
    }
    
    // opengl stuff
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glRasterPos2i(-1, 1);
    glPixelZoom(1, -1);
    
    glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
    
    SDL_GL_SwapBuffers();
    
    glFlush();
    
    // Delay
    // test
    // SDL_Delay(1000.0f/60.0f);
}
