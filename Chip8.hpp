/* Class definitions and types */

#include <cstdio>
#include <cstdlib>
#include <vector>

#include <climits>

#include "Display.hpp"

#ifndef CHIP8_H_INCLUDED
#define CHIP8_H_INCLUDED

/* data types */
typedef unsigned char BYTE;
typedef unsigned short int WORD;

/* decodes an instruction (1 word/2 bytes) */
class Opcode
{
public:
    // constructor
    Opcode(WORD value){m_Value = value;}

    // getters
    WORD getValue(void){return m_Value;}

    // ex) value is 0x1234
    WORD Num1(void){return (m_Value & 0xF000);} // 0x1234 & 0xF000 = 0x1000
    WORD Num2(void){return (m_Value & 0x0F00);} // 0x1234 & 0x0F00 = 0x0200
    WORD Num3(void){return (m_Value & 0x00F0);} // 0x1234 & 0x00F0 = 0x0030
    WORD Num4(void){return (m_Value & 0x000F);} // 0x1234 & 0x000F = 0x0004

    WORD Num234(void){return (m_Value & 0x0FFF);} // used for op 1NNN

    WORD Num34(void){return (m_Value & 0x00FF);}
private:
    WORD m_Value;
};

class Chip8
{
public:
    // constructor/deconstructor
    Chip8(void);
    ~Chip8(void);

    // reset member variables
    void CPUReset(void);

    // load the ROM
    bool LoadROM(const char *fname);

//private:

    // Convert the 2 bytes at m_PC to a WORD
    // and increment the PC by two (since we read to bytes)
    Opcode GetNextOpcode(void);

    // get the next opcode, decode it, and execute it (call the associated
    // function)
    bool RunNextInstruction(void);
    
    // draw using the DXYN func however we're implementing it
    // coords x,y, height n, width 8 pixels
    // used by m_OpDXYN
    //bool drawSprite(Display &d, int coordx, int coordy, int n);

    //////////////////////////////////////////////////////////////////
    //                 Opcode Instruction Functions                 //
    //////////////////////////////////////////////////////////////////

    void m_Op1NNN(Opcode op);

    void m_Op00E0(Opcode op);
    void m_Op00EE(Opcode op);

    void m_Op2NNN(Opcode op);

    // conditionals (skip next if)
    void m_Op3XNN(Opcode op);
    void m_Op4XNN(Opcode op);
    void m_Op5XY0(Opcode op);

    // constants (vx = NN and vx += NN)
    void m_Op6XNN(Opcode op);
    void m_Op7XNN(Opcode op);

    void m_Op8XY4(Opcode op);
    void m_Op8XY5(Opcode op);

    void m_OpANNN(Opcode op);

    void m_OpDXYN(Opcode op);
    
    void m_OpFX1E(Opcode op);

    void m_OpFX33(Opcode op);

    void m_OpFX55(Opcode op);

    //////////////////////////////////////////////////////////////////

    BYTE m_GameMemory[0xFFF]; // 0xFFF bytes of memory
    BYTE m_Registers[16];     // 16 registers, 1 byte each
    WORD m_AddressI;          // 16 bit address register I
    WORD m_PC;                // 16 bit program counter

    //BYTE m_ScreenData[64][32]; // screen pixels
    BYTE m_ScreenData[320][640][3]; // y,x,3 channels
    //BYTE m_ScreenData[64*32*3]; // screen pixels - width, height, rgb

    std::vector<WORD> m_Stack;      // 16 bit stack
};

#endif // CHIP8_H_INCLUDED
