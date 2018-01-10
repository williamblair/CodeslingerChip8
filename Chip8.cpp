
#include "Chip8.hpp"

/* Constructor/Deconstructor */
Chip8::Chip8(void){}
Chip8::~Chip8(void){}

/* Reset member variables */
void Chip8::CPUReset(void)
{
    m_AddressI = 0;
    m_PC = 0x200;
    
    m_DelayTimer = 0;
    m_SoundTimer = 0;

    m_Stack.assign(1, 0); // 1 entry with a value of 0
    
    // rand() used by CXNN
    srand(time(0));
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

// decrease sound and delay timers (should be called at a rate
// of 60hz)
bool Chip8::DecreaseTimers(void)
{
    if(m_DelayTimer > 0) m_DelayTimer--;
    
    if(m_SoundTimer > 0) {
        // TODO - make beep here!
        m_SoundTimer--;
    }
    
    
    return true;
}

bool Chip8::SetKey(int key, int val)
{
    // TODO - should I check for valid values?
    m_Keys[key] = val;
    
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
            case 0x0:
                switch(op.getValue())
                {
                    case 0x00E0:
                        m_Op00E0(op);
                        break;
                    case 0x00EE:
                        m_Op00EE(op);
                        break;
                    default:
                        throw op;
                        break;
                }
                break;
            case 0x1:
                m_Op1NNN(op);
                break;
            case 0x2:
                m_Op2NNN(op);
                break;
            case 0x3:
                m_Op3XNN(op);
                break;
            case 0x4:
                m_Op4XNN(op);
                break;
            case 0x5:
                m_Op5XY0(op);
                break;
            case 0x6:
                m_Op6XNN(op);
                break;
            case 0x7:
                m_Op7XNN(op);
                break;
            case 0x8:
                switch(op.Num4())
                {
                    case 0x0:
                        m_Op8XY0(op);
                        break;
                    case 0x1:
                        m_Op8XY1(op);
                        break;
                    case 0x2:
                        m_Op8XY2(op);
                        break;
                    case 0x3:
                        m_Op8XY3(op);
                        break;
                    case 0x4:
                        m_Op8XY4(op);
                        break;
                    case 0x5:
                        m_Op8XY5(op);
                        break;
                    case 0x6:
                        m_Op8XY6(op);
                        break;
                    case 0x7:
                        m_Op8XY7(op);
                        break;
                    case 0xE:
                        m_Op8XYE(op);
                        break;
                    default:
                        throw op;
                        break;
                }
                break;
            case 0x9:
                m_Op9XY0(op);
                break;
            case 0xA:
                m_OpANNN(op);
                break;
            case 0xB:
                m_OpBNNN(op);
                break;
            case 0xC:
                m_OpCXNN(op);
                break;
            case 0xD:
                m_OpDXYN(op);
                break;
            case 0xE:
                switch(op.Num34())
                {
                    case 0x9E:
                        m_OpEX9E(op);
                        break;
                    case 0xA1:
                        m_OpEXA1(op);
                        break;
                    default:
                        throw op;
                }
                break;
            case 0xF:
                switch(op.Num34())
                {
                    case 0x07:
                        m_OpFX07(op);
                        break;
                    case 0x0A:
                        m_OpFX0A(op);
                        break;
                    case 0x15:
                        m_OpFX15(op);
                        break;
                    case 0x18:
                        m_OpFX18(op);
                        break;
                    case 0x1E:
                        m_OpFX1E(op);
                        break;
                    case 0x29:
                        m_OpFX29(op);
                        break;
                    case 0x33:
                        m_OpFX33(op);
                        break;
                    case 0x55:
                        m_OpFX55(op);
                        break;
                    case 0x65:
                        m_OpFX65(op);
                        break;
                    default:
                        throw op;
                }
                break;
            default:
                throw op;
        }

        // DEBUG
        //printf("0x%X  0x%X\n", m_PC, op.getValue());
    }
    catch(Opcode &o)
    {
        fprintf(stderr, "Chip8::RunNextInstruction: Exception: ");
        fprintf(stderr, "Unhandled Opcode: 0x%X\n", o.getValue());

        exit(EXIT_FAILURE);
    }

    return true;
}
