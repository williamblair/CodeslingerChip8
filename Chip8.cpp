/* Follow along of Codeslinger.co.uk Chip8 emulator */

#include <cstdio>
#include <cstdlib>
#include <vector>

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

    BYTE m_GameMemory[0xFFF]; // 0xFFF bytes of memory
    BYTE m_Registers[16];     // 16 registers, 1 byte each
    WORD m_AddressI;          // 16 bit address register I
    WORD m_PC;                // 16 bit program counter

    BYTE m_ScreenData[64][32]; // screen pixels

    std::vector<WORD> m_Stack;      // 16 bit stack
};

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
            default:
                throw op;
        }
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
