/* Follow along of Codeslinger.co.uk Chip8 emulator */

#include <iostream>
#include <vector>

/* data types */
typedef unsigned char BYTE;
typedef unsigned short int WORD;

class Chip8
{
public:
private:
    BYTE m_GameMemory[0xFFF]; // 0xFFF bytes of memory
    BYTE m_Registers[16];     // 16 registers, 1 byte each
    WORD m_AddressI;          // 16 bit address register I
    WORD m_PC;                // 16 bit program counter

    std::vector m_Stack;      // 16 bit stack
}
