/* Opcode functions for Chip8 */

#include "Chip8.hpp"

/* Jump instruction */
void Chip8::m_Op1NNN(Opcode op)
{
    m_PC = op.Num234();
}


// unhandled for now...
void Chip8::m_Op00E0(Opcode op)
{
    printf("Chip8::m_Op00E0: Warning: Not Implemented!\n");
}
void Chip8::m_Op00EE(Opcode op)
{
    printf("Chip8::m_Op00EE: Warning: Not Implemented!\n");
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
    WORD nn = 0x0000;
    int n1 = op.Num3(); // 0x12
    nn = n1 << 8;      // 0x1200
    int n2 = op.Num4(); // 0x34
    nn |= n2;          // 0x1200 | 0x0034 = 0x1234

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
    WORD nn = 0x0000;
    int n1 = op.Num3(); // 0x12
    nn = n1 << 8;      // 0x1200
    int n2 = op.Num4(); // 0x34
    nn |= n2;          // 0x1200 | 0x0034 = 0x1234

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

/* DXYN - draw a sprite at coord (x,y) with a width of 8 and height of N */
void Chip8::m_OpDXYN(Opcode op)
{
    int regx = op.Num2();
    regx = regx >> 8;

    int regy = op.Num3();
    regy = regy >> 4;

    int height = op.Num4();

    int coordx = m_Registers[regx];
    int coordy = m_Registers[regy];

    m_Registers[0xF] = 0;

    // loop the amount of vertical lines needed to draw
    for(int yline = 0; yline < height; yline++)
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
                if(m_ScreenData[x][y] == 1){
                    m_Registers[0xF] = 1; // collision
                }
                // toggle the pixel state
                m_ScreenData[x][y] ^= 1;
            }
        }
    }
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
