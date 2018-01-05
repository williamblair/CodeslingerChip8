/* Opcode functions for Chip8 */

#include "Chip8.hpp"

/* Jump instruction */
void Chip8::m_Op1NNN(Opcode op)
{
    m_PC = op.Num234();
}


/* 00E0: Clear Screen */
void Chip8::m_Op00E0(Opcode op)
{
    for(int i=0; i<320; i++)
    {
        for(int j=0; j<640; j++)
        {
            m_ScreenData[i][j][0] = 255;
            m_ScreenData[i][j][1] = 255;
            m_ScreenData[i][j][2] = 255;
        }
    }
}

/* 00EE: return from subroutine (the previous PC
 * was stored on the stack) */
void Chip8::m_Op00EE(Opcode op)
{
    m_PC = m_Stack.back();
    m_Stack.pop_back();
}

void Chip8::m_Op2NNN(Opcode op)
{
    m_Stack.push_back(m_PC); // save the PC
    m_PC = op.Num234();      // Jump to address NNN
}

/* 3XNN: skips the next instruction if VX = NN
 * (usually next instruction is a jump to skip
 * a code block) */
void Chip8::m_Op3XNN(Opcode op)
{
    int regx = op.Num2();
    regx = regx >> 8;

    // get NN
    int nn = op.Num34();

    int vx = m_Registers[regx];

    // if the values are equal skip the next instruction
    if(vx == nn){
        m_PC += 2;
    }
}

/* 4XNN: skips the next instruction if Vx !=NN
 * same as 3XNN except jump if NOT equal */
void Chip8::m_Op4XNN(Opcode op)
{
    int regx = op.Num2();
    regx = regx >> 8;

    // get NN
    int nn = op.Num34();

    int vx = m_Registers[regx];

    // if the values are equal skip the next instruction
    if(vx != nn){
        m_PC += 2;
    }
}

/* 5XY0: Skip the next instruction if vx == vy */
void Chip8::m_Op5XY0(Opcode op)
{
    // turn 0x5XY0 into 0xX
    int regx = op.Num2(); // 0x0X00
    regx = regx >> 8;     // 0xX

    // turn 0x5XY0 into 0xY
    int regy = op.Num3(); // 0x00Y0
    regy = regy >> 4;     // 0xY

    // if the registers are equal skip the next instruction
    if(m_Registers[regx] == m_Registers[regy]) {
        m_PC += 2;
    }
}

/* 6XNN: sets Vx to NN */
void Chip8::m_Op6XNN(Opcode op)
{
    // get NN
    WORD nn = op.Num34();

    // get the register
    int regx = op.Num2() >> 8;

    // assign the value at the register
    m_Registers[regx] = nn;

    // test
    printf("Registers[0x%X]: 0x%X\n", regx, m_Registers[regx]);
}

/* 7XNN: adds NN to Vx (No flags/overflow check) */
void Chip8::m_Op7XNN(Opcode op)
{
    // get NN
    int NN = op.Num34(); // no shifting needed

    // get the register
    int regx = op.Num2();
    regx = regx >> 8;

    // test
    printf("Vx Before: 0x%X\n", m_Registers[regx]);
    printf("NN: 0x%X\n", NN);

    // add the values
    m_Registers[regx] += NN;

    // test
    printf("Vx After: 0x%X\n", m_Registers[regx]);

}

/* 8XY4: Y is added to register X */
void Chip8::m_Op8XY4(Opcode op)
{
    m_Registers[0xF] = 0; // default flag 0 for no overflow

    int regx = op.Num2();
    regx = regx >> 8;

    int regy = op.Num3();
    regy = regy >> 4;

    int xval = m_Registers[regx];
    int yval = m_Registers[regy];

    // test if there will be overflow
    if(UCHAR_MAX - xval < yval) {
        m_Registers[0xF] = 1;
    }

    m_Registers[regx] = xval + yval;
}

/* 8XY5: Y is subtracted from register X */
void Chip8::m_Op8XY5(Opcode op)
{
    m_Registers[0xF] = 1; // flag register (0 if subtracting and result < 0,
                          // 1 if adding and result > 255)

    int regx = op.Num2();
    regx = regx >> 8;

    int regy = op.Num3();
    regy = regy >> 4;

    int xval = m_Registers[regx];
    int yval = m_Registers[regy];

    // test if there will be underflow
    if(yval > xval) {
        m_Registers[0xF] = 0;
    }

    // write the result
    m_Registers[regx] = xval - yval;
}

/* ANNN: Sets I to the address NNN */
void Chip8::m_OpANNN(Opcode op)
{
    // Get NNN
    int NNN = op.Num234(); // no need to shift here

    // set I to its value
    m_AddressI = NNN;

    // test
    printf("Address I: 0x%X\n", m_AddressI);
}

/*bool Chip8::drawSprite(Display &d, int coordx, int coordy, int n)
{
    // loop the amount of vertical lines needed to draw
    for(int yline = 0; yline < n; yline++)
    {
        // sprite data stored at m_AddressI
        BYTE data = m_GameMemory[m_AddressI+yline];
        int xpixelinv = 7;
        int xpixel = 0;
        for(xpixel=0; xpixel < 8; xpixel++,xpixelinv--)
        {
            
            // check for transparency
            // xpixelinv required by the way sprite data is stored
            int mask = 1 << xpixelinv;
            if(data & mask)
            {
                int x = coordx + xpixel;
                int y = coordy + yline;
                
                //int index = (y*64 + x) * 3;
                
                if(m_ScreenData[x][y] == 1){
                //if(m_ScreenData[index] != 0){
                    m_Registers[0xF] = 1; // collision
                }
                // toggle the pixel state
                m_ScreenData[x][y] ^= 1;
                //m_ScreenData[x][y][0] ^= 1;
                //m_ScreenData[x][y][1] ^= 1;
                //m_ScreenData[x][y][2] ^= 1;
                //m_ScreenData[y*64+x + 0] = (m_ScreenData[y*64+x + 0] == 255) ? 0 : 255;
                //m_ScreenData[y*64+x + 1] = (m_ScreenData[y*64+x + 1] == 255) ? 0 : 255;
                //m_ScreenData[y*64+x + 2] = (m_ScreenData[y*64+x + 2] == 255) ? 0 : 255;

                //m_ScreenData[index + 0] = (m_ScreenData[index + 0] == 0xFFFFFF) ? 0 : 0xFFFFFF;
                //m_ScreenData[index + 1] = (m_ScreenData[index + 2] == 0xFFFFFF) ? 0 : 0xFFFFFF;
                //m_ScreenData[index + 2] = (m_ScreenData[index + 3] == 0xFFFFFF) ? 0 : 0xFFFFFF;
            }
        }
    }
    
    // update the screen surface
    d.updateSurface(m_ScreenData);
    
}*/ 

/* DXYN - draw a sprite at coord (x,y) with a width of 8 and height of N */
void Chip8::m_OpDXYN(Opcode op)
{
    const int SCALE = 10; // Chip8 res 64 32, our res 640 320
    int regx = op.Num2();
    regx = regx >> 8;
    int regy = op.Num3();
    regy = regy >> 4;
    
    int coordx = m_Registers[regx] * SCALE;
    int coordy = m_Registers[regy] * SCALE;
    int height = op.Num4(); // no shift needed
    
    // set the flag to zero (no hit detected)
    m_Registers[0xF] = 0;
    
    printf("Before for loop\n");
    
    for(int yline=0; yline < height; yline++)
    {
        // m_AddressI contains sprite data stored as a line of bytes
        BYTE data = m_GameMemory[m_AddressI + yline];
        
        printf("Before xpixel loop\n");
        
        int xpixel = 0;
        int xpixelinv = 7; // xpixel inverted
        for(xpixel=0; xpixel<8; xpixel++, xpixelinv--)
        {
            // is the pixel set to 1? if so then the code needs to toggle its state
            int mask = 1 << xpixelinv;
            if(data & mask)
            {
                int x = (xpixel * SCALE) + coordx;
                int y = (yline * SCALE) + coordy;
                
                int color = 0;
                
                printf("Before if!\n");
                printf("x, y: (%d,%d)\n", x, y);
                
                // if collision detected
                if(m_ScreenData[y][x][0] == 0)
                {
                    color = 255;
                    m_Registers[0xF] = 1; // set collision flag
                }
                
                // debug
                printf("Before color loop!\n");
                
                // color the pixel
                for(int i=0; i < SCALE; i++)
                {
                    for(int j=0; j < SCALE; j++)
                    {
                        m_ScreenData[y+i][x+j][0] = color;
                        m_ScreenData[y+i][x+j][1] = color;
                        m_ScreenData[y+i][x+j][2] = color;
                    }
                }
            }
            
            // debug
            else
                printf("Not data and mask!\n");
        }
    }
    
    printf("After for loop!\n");
}

/* FX1E: adds Vx to Address I */
void Chip8::m_OpFX1E(Opcode op)
{
    // get regx
    int regx = op.Num2();
    regx = regx >> 8;
    
    // test
    printf("Address I before: 0x%X\n", m_AddressI);
    printf("Vx: 0x%X\n", m_Registers[regx]);
    
    // add the value to I
    m_AddressI += m_Registers[regx];
    
    // test
    printf("Address I after: 0x%X\n", m_AddressI);
}

/* FX33: binary coded decimal - store Vx as
 * BCD 3,2,1 at address I */
void Chip8::m_OpFX33(Opcode op)
{
    int regx = op.Num2();
    regx  = regx >> 8;

    int value = m_Registers[regx];

    int hundreds = value / 100;
    int tens     = (value / 10) % 10;
    int units    = value % 10;

    m_GameMemory[m_AddressI+0] = hundreds;
    m_GameMemory[m_AddressI+1] = tens;
    m_GameMemory[m_AddressI+2] = units;
}

/* Fx55: store V0 through Vx in memory starting at address I
 * author unsure if loop is  < or <= */
void Chip8::m_OpFX55(Opcode op)
{
    int regx = op.Num2();
    regx = regx >> 8;

    for(int i=0; i<=regx; i++)
    {
        m_GameMemory[m_AddressI+i] = m_Registers[i];
    }
    m_AddressI = m_AddressI + regx + 1;
}
