/* Follow along of Codeslinger.co.uk Chip8 emulator */

#include <cstdio>
#include <cstdlib>
#include <vector>

#include "Chip8.hpp"
#include "Display.hpp"

// Stupid SDL issue
#ifdef __WIN32
#undef main
#endif

int main(int argc, char **argv)
{
    Chip8 chip;

    // make sure ROM filename was given
    if(argc != 2) {
        printf("Usage: %s [ROM file]\n", argv[0]);
        return 0;
    }
    
    Display display(640, 320, "Chip8 Emulator");

    // reset the CPU
    chip.CPUReset();

    // load the ROM
    if(!chip.LoadROM(argv[1])) {
        return -1;
    }

    // fps of the game to run at
    int fps = 60;
    
    // found in chip8 src ini
    int opsPerSec = 400;
    
    // number of opcodes to execute per frame
    int numframe = opsPerSec / fps;
    
    // how long to delay
    float interval = 1000.0f / fps;
    
    unsigned int time2 = SDL_GetTicks();

    // inf loop
    for(;;)
    {
        //chip.RunNextInstruction(display);
        
        //display.update(chip.m_ScreenData);
        //getchar();
        
        unsigned int current = SDL_GetTicks();
        
        if( (time2 + interval) < current )
        {
            // TODO - add cpu timers
            //chip.decreaseTimers();
            
            // execute our calculated number of ops
            for(int i=0; i<numframe; i++)
                chip.RunNextInstruction();
                
            // get the new time
            time2 = current;
            
            // get keys and refresh the screen
            display.update(chip.m_ScreenData);
        }
    }

    return 0;
}
