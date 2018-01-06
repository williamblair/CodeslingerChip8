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

void Display::pollEvents(Chip8 &chip)
{
    // check keys
    SDL_Event e;
    while(SDL_PollEvent(&e))
    {
        // x out the window
        if(e.type == SDL_QUIT) exit(EXIT_SUCCESS);
        
        // press keys
        if(e.type == SDL_KEYDOWN)
        {
            switch(e.key.keysym.sym)
            {
                // exit the emulator
                case SDLK_ESCAPE: exit(EXIT_SUCCESS);
                
                case SDLK_1: chip.SetKey(0x1, 1); break;
                case SDLK_2: chip.SetKey(0x2, 1); break;
                case SDLK_3: chip.SetKey(0x3, 1); break;
                case SDLK_4: chip.SetKey(0xC, 1); break;
                case SDLK_q: chip.SetKey(0x4, 1); break;
                case SDLK_w: chip.SetKey(0x5, 1); break;
                case SDLK_e: chip.SetKey(0x6, 1); break;
                case SDLK_r: chip.SetKey(0xD, 1); break;
                case SDLK_a: chip.SetKey(0x7, 1); break;
                case SDLK_s: chip.SetKey(0x8, 1); break;
                case SDLK_d: chip.SetKey(0x9, 1); break;
                case SDLK_f: chip.SetKey(0xE, 1); break;
                case SDLK_z: chip.SetKey(0xA, 1); break;
                case SDLK_x: chip.SetKey(0x0, 1); break;
                case SDLK_c: chip.SetKey(0xB, 1); break;
                case SDLK_v: chip.SetKey(0xF, 1); break;
                default:
                    break;
            }
        }
        // release keys
        else if(e.type == SDL_KEYUP)
        {
            switch(e.key.keysym.sym)
            {
                case SDLK_1: chip.SetKey(0x1, 0); break;
                case SDLK_2: chip.SetKey(0x2, 0); break;
                case SDLK_3: chip.SetKey(0x3, 0); break;
                case SDLK_4: chip.SetKey(0xC, 0); break;
                case SDLK_q: chip.SetKey(0x4, 0); break;
                case SDLK_w: chip.SetKey(0x5, 0); break;
                case SDLK_e: chip.SetKey(0x6, 0); break;
                case SDLK_r: chip.SetKey(0xD, 0); break;
                case SDLK_a: chip.SetKey(0x7, 0); break;
                case SDLK_s: chip.SetKey(0x8, 0); break;
                case SDLK_d: chip.SetKey(0x9, 0); break;
                case SDLK_f: chip.SetKey(0xE, 0); break;
                case SDLK_z: chip.SetKey(0xA, 0); break;
                case SDLK_x: chip.SetKey(0x0, 0); break;
                case SDLK_c: chip.SetKey(0xB, 0); break;
                case SDLK_v: chip.SetKey(0xF, 0); break;
                default:
                    break;
            }
        }
    }
}
