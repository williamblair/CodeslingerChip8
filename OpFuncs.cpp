/* Opcode functions for Chip8 */

#include "Chip8.hpp"

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

/* 1NNN: goto NNN (move to address NNN) */
void Chip8::m_Op1NNN(Opcode op)
{
    m_PC = op.Num234();
}

/* 2NNN: Jump (set PC value as NNN) */
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
    int regx = op.Num2();

    // turn 0x5XY0 into 0xY
    int regy = op.Num3();

    // if the registers are equal skip the next instruction
    if(m_Registers[regx] == m_Registers[regy]) {
        m_PC += 2;
    }
}

/* 6XNN: sets Vx to NN */
void Chip8::m_Op6XNN(Opcode op)
{
    // get NN
    int nn = op.Num34();

    // get the register
    int regx = op.Num2();

    // assign the value at the register
    m_Registers[regx] = nn;
}

/* 7XNN: adds NN to Vx (No flags/overflow check) */
void Chip8::m_Op7XNN(Opcode op)
{
    // get NN
    int NN = op.Num34(); // no shifting needed

    // get the register
    int regx = op.Num2();

    // add the values
    m_Registers[regx] += NN;

}

/* 8XY0: sets Vx equal to Vy */
void Chip8::m_Op8XY0(Opcode op)
{
    // get regx
    int regx = op.Num2();
    
    // get regy
    int regy = op.Num3();
    
    // set Vx = Vy
    m_Registers[regx] = m_Registers[regy];
}

/* 8XY1: sets Vx to Vx | Vy */
void Chip8::m_Op8XY1(Opcode op)
{
    int regx = op.Num2();
    
    int regy = op.Num3();
    
    m_Registers[regx] = m_Registers[regx] | m_Registers[regy];
}

/* 8XY2: sets Vx to Vx & Vy (bit op) */
void Chip8::m_Op8XY2(Opcode op)
{
    int regx = op.Num2();
    
    int regy = op.Num3();
    
    m_Registers[regx] = m_Registers[regx] & m_Registers[regy];
}

/* 8XY3: sets Vx to Vx XOR Vy */
void Chip8::m_Op8XY3(Opcode op)
{
    int regx = op.Num2();
    
    int regy = op.Num3();
    
    m_Registers[regx] = m_Registers[regx] ^ m_Registers[regy];
}

/* 8XY4: Y is added to register X */
void Chip8::m_Op8XY4(Opcode op)
{
    m_Registers[0xF] = 0; // default flag 0 for no overflow

    int regx = op.Num2();

    int regy = op.Num3();
    
    int value = m_Registers[regx] + m_Registers[regy];
    
    // if overflow set flag to 1
    if(value > 255)
        m_Registers[0xF] = 1;
    
    // Overflow can happen here I believe
    m_Registers[regx] = m_Registers[regx] + m_Registers[regy];
}

/* 8XY5: Y is subtracted from register X */
void Chip8::m_Op8XY5(Opcode op)
{
    m_Registers[0xF] = 1; // flag register (0 if subtracting and result < 0,
                          // 1 if adding and result > 255)

    int regx = op.Num2();

    int regy = op.Num3();

    int xval = m_Registers[regx];
    int yval = m_Registers[regy];

    // test if there will be underflow
    if(xval < yval) {
        m_Registers[0xF] = 0;
    }

    // write the result
    m_Registers[regx] = m_Registers[regx] - m_Registers[regy];
}

/* 8XY6: shifts Vx right by 1
 * Flag is set to the LSB of Vx before the shift */
void Chip8::m_Op8XY6(Opcode op)
{
    int regx = op.Num2();
    
    // ex) 0b0011 & 1 = 1, 0b0010 & 1 = 0
    int LSB = m_Registers[regx] & 1;
    m_Registers[0xF] = LSB;
    
    // shift Vy by 1
    m_Registers[regx] = m_Registers[regx] >> 1;
}

/* 8XY7: sets Vx to Vy - Vx, Vf set to 0 when borrow,
 * 1 when not */
void Chip8::m_Op8XY7(Opcode op)
{
    int regx = op.Num2();
    
    int regy = op.Num3();
    
    // default 1 when no borrow
    m_Registers[0xF] = 1;
    
    // flag 0 if there is borrow
    if(m_Registers[regx] > m_Registers[regy]){
        m_Registers[0xF] = 0;
    }
    
    // subtract
    m_Registers[regx] = m_Registers[regy] - m_Registers[regx];
}

/* 8XYE: shifts Vx left 1
 * flag set to MSB of Vx before shift */
void Chip8::m_Op8XYE(Opcode op)
{
    int regx = op.Num2();
    
    // ex) 0b10110111 >> 7  = 1, 0b00001101 >> 7 = 0
    int MSB = m_Registers[regx] >> 7;
    m_Registers[0xF] = MSB;
    
    // left shift
    m_Registers[regx] = m_Registers[regx] << 1;
}

/* 9XY0: skips next instruction if Vx != Vy */
void Chip8::m_Op9XY0(Opcode op)
{
    int regx = op.Num2();
    
    int regy = op.Num3();
    
    if(m_Registers[regx] != m_Registers[regy]){
        m_PC += 2;
    }
}

/* ANNN: Sets I to the address NNN */
void Chip8::m_OpANNN(Opcode op)
{
    // Get NNN
    int NNN = op.Num234(); // no need to shift here

    // set I to its value
    m_AddressI = NNN;
}

/* BNNN: jumps to address NNN + V0 */
void Chip8::m_OpBNNN(Opcode op)
{
    m_PC = m_Registers[0x0] + op.Num234();
}

/* CXNN: sets Vx to rand() (usually 0-255) & NN */
void Chip8::m_OpCXNN(Opcode op)
{
    int regx = op.Num2();
    
    m_Registers[regx] = (rand()%255) & op.Num34();
}

/* DXYN - draw a sprite at coord (x,y) with a width of 8 and height of N */
void Chip8::m_OpDXYN(Opcode op)
{
    const int SCALE = 10; // Chip8 res 64 32, our res 640 320
    int regx = op.Num2();
    int regy = op.Num3();
    
    int coordx = m_Registers[regx] * SCALE;
    int coordy = m_Registers[regy] * SCALE;
    int height = op.Num4(); // no shift needed
    
    // set the flag to zero (no hit detected)
    m_Registers[0xF] = 0;
    
    for(int yline=0; yline < height; yline++)
    {
        // m_AddressI contains sprite data stored as a line of bytes
        BYTE data = m_GameMemory[m_AddressI + yline];
        
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
                
                // if collision detected
                if(m_ScreenData[y][x][0] == 0)
                {
                    color = 255;
                    m_Registers[0xF] = 1; // set collision flag
                }
                
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
        }
    }
}

/* EX9E: Skips the next instruction if the key stored in 
 * Vx is pressed */
void Chip8::m_OpEX9E(Opcode op)
{
    int regx = op.Num2();
    
    int key = m_Registers[regx];
    
    // if the key IS pressed, skip the next instruction
    if(m_Keys[key] == 1){
        m_PC += 2;
    }
}

/* EXA1: skips next instruction if key() != Vx */
void Chip8::m_OpEXA1(Opcode op)
{
    int regx = op.Num2();
    
    int key = m_Registers[regx];
    
    // if the key is NOT pressed, skip the next instruction
    if(m_Keys[key] == 0) {
        m_PC += 2;
    }
}

/* FX07: sets Vx to the value of the delay timer */
void Chip8::m_OpFX07(Opcode op)
{
    int regx = op.Num2();
    
    m_Registers[regx] = m_DelayTimer;
}

/* FX0A: a key press is waited, and then stored in Vx
 * (Blocking operation - all instructions halted
 * until next key event) */
void Chip8::m_OpFX0A(Opcode op)
{
    int i;
    bool keypressed = false;
    
    int regx = op.Num2();
    
    // look for a keypress
    for(i=0; i<16; i++)
    {
        // if a key was pressed, store it in Vx
        // and tell our if below we're done
        if(m_Keys[i] == 1){
            keypressed = true;
            m_Registers[regx] = i;
        }
    }
    // if no keys were pressed, move the PC
    // back to this instruction again
    if(!keypressed){
        m_PC -= 2;
    }
}

/* FX15: Sets delay timer to Vx */
void Chip8::m_OpFX15(Opcode op)
{
    int regx = op.Num2();
    
    m_DelayTimer = m_Registers[regx];
}

/* FX18: sets sound timer to Vx */
void Chip8::m_OpFX18(Opcode op)
{
    int regx = op.Num2();
    
    m_SoundTimer = m_Registers[regx];
}

/* FX1E: adds Vx to Address I */
void Chip8::m_OpFX1E(Opcode op)
{
    // get regx
    int regx = op.Num2();
    
    // add the value to I
    m_AddressI += m_Registers[regx];
}

/* FX29: sets I to the location of the sprite for the
 * character in Vx. Characters 0-F (hex) represented by 4x5 font */
void Chip8::m_OpFX29(Opcode op)
{
    int regx = op.Num2();
    
    m_AddressI = m_Registers[regx]*5; // 4x5
}

/* FX33: binary coded decimal - store Vx as
 * BCD 3,2,1 at address I */
void Chip8::m_OpFX33(Opcode op)
{
    int regx = op.Num2();

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

    for(int i=0; i<=regx; i++)
    {
        m_GameMemory[m_AddressI+i] = m_Registers[i];
    }
    m_AddressI = m_AddressI + regx + 1;
}

/* FX65: fills V0 to Vx (including Vx) with values from
 * memory starting at address I */
void Chip8::m_OpFX65(Opcode op)
{
    int regx = op.Num2();

    for(int i=0; i<=regx; i++)
    {
        m_Registers[i] = m_GameMemory[m_AddressI+i];
    }
    m_AddressI = m_AddressI + regx + 1;
}
