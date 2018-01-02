/* Follow along of Codeslinger.co.uk Chip8 emulator */

#include <cstdio>
#include <cstdlib>
#include <vector>

#include "Chip8.hpp"

/* Constructor/Deconstructor */
Chip8::Chip8(void){}
Chip8::~Chip8(void){}

/* Reset member variables */
void Chip8::CPUReset(void)
{
    m_AddressI = 0;
    m_PC = 0x200;

    m_Stack.assign(1, 0); // 1 entry with a value of 0
}

/* Load the ROM */
bool Chip8::LoadROM(const char *fname)
{
    FILE *fp = fopen(fname, "rb");
    if(!fp){
        fprintf(stderr, "Chip8::LoadROM: Failed to open '%s'\n",
            fname);
        return false;
    }

    /* read the ROM into game memory */
    fread(&m_GameMemory[0x200], 0xFFF, 1, fp);

    /* close the file */
    fclose(fp);

    return true;
}

// Convert the 2 bytes at m_PC to a WORD
// and increment the PC by two (since we read to bytes)
Opcode Chip8::GetNextOpcode(void)
{
    // example - say our bytes at PC are 0xAB and 0xCD,
    // then the opcode needs to be 0xABCD

    WORD res = 0; // result; 0x0000 right now

    // combine next 2 bytes into 1 word
    res   = m_GameMemory[m_PC];  // 0x00AB
    res <<= 8;                   // 0xAB00
    res |= m_GameMemory[m_PC+1]; // 0xABCD (0xAB00 | 0x00CD)

    // increment the PC
    m_PC += 2;

    Opcode o(res);
    return o;
}

// get the next opcode, decode it, and execute it (call the associated
// function)
bool Chip8::RunNextInstruction(void)
{
    try
    {
        // get the next opcode
        Opcode op = GetNextOpcode();

        // decode
        switch(op.Num1())
        {
            case 0x0000:
                switch(op.Num4())
                {
                    case 0x0000:
                        m_Op00E0(op);
                        break;
                    case 0x000E:
                        m_Op00EE(op);
                        break;
                    default:
                        throw op;
                }
                break;
            case 0x1000:
                m_Op1NNN(op);
                break;
            case 0x2000:
                break;
            case 0x3000:
                m_Op3XNN(op);
                break;
            case 0x4000:
            case 0x5000:
            case 0x6000:
            case 0x7000:
            case 0x8000:
            case 0x9000:
            case 0xA000:
            case 0xB000:
            case 0xC000:
            case 0xD000:
            case 0xE000:
            case 0xF000:
            default:
                throw op;
        }

        // DEBUG
        printf("0x%X  0x%X\n", m_PC, op.getValue());
    }
    catch(Opcode &o)
    {
        fprintf(stderr, "Chip8::RunNextInstruction: Exception: ");
        fprintf(stderr, "Unhandled Opcode: 0x%X\n", o.getValue());

        exit(EXIT_FAILURE);
    }

    return true;
}

int main(int argc, char **argv)
{
    Chip8 chip;

    // make sure ROM filename was given
    if(argc != 2) {
        printf("Usage: %s [ROM file]\n", argv[0]);
        return 0;
    }

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
        getchar();
    }

    return 0;
}
