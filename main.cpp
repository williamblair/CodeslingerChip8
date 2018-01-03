/* Follow along of Codeslinger.co.uk Chip8 emulator */

#include <cstdio>
#include <cstdlib>
#include <vector>

#include "Chip8.hpp"
#include "Display.hpp"

int main(int argc, char **argv)
{
    Chip8 chip;

    // make sure ROM filename was given
    if(argc != 2) {
        printf("Usage: %s [ROM file]\n", argv[0]);
        return 0;
    }
    
    Display display(640, 480, "Chip8 Emulator");

    // reset the CPU
    chip.CPUReset();

    // load the ROM
    if(!chip.LoadROM(argv[1])) {
        return -1;
    }

    // inf loop
    for(;;)
    {
        chip.RunNextInstruction();
        
        display.update();
        getchar();
    }

    return 0;
}
