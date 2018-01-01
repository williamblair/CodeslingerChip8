/* Follow along of Codeslinger.co.uk Chip8 emulator */

#include <iostream>
#include <vector>

/* data types */
typedef unsigned char BYTE;
typedef unsigned short int WORD;

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
private:
    BYTE m_GameMemory[0xFFF]; // 0xFFF bytes of memory
    BYTE m_Registers[16];     // 16 registers, 1 byte each
    WORD m_AddressI;          // 16 bit address register I
    WORD m_PC;                // 16 bit program counter

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
        std::cerr << "Chip8::LoadROM: Failed to open '"
            << fname << "'\n";
        return false;
    }

    /* read the ROM into game memory */
    fread(&m_GameMemory[0x200], 0xFFF, 1, fp);

    /* close the file */
    fclose(fp);

    return true;
}

int main(int argc, char **argv)
{
    Chip8 chip;

    // make sure ROM filename was given
    if(argc != 2) {
        std::cout << "Usage: " << argv[0] << " [ROM file]\n";
        return 0;
    }

    // reset the CPU
    chip.CPUReset();

    // load the ROM
    if(!chip.LoadROM(argv[1])) {
        return -1;
    }

    return 0;
}
