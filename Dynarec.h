#ifndef Dyanrec_H
#define Dynarec_H

#include <malloc.h>
#include <cstring>

enum REG_MIPS{
    None = -1,
    _zero = 0,
    _at, 
    _v0, _v1,
    _a0, _a1, _a2, _a3,
    _t0, _t1, _t2, _t3,_t4, _t5, _t6, _t7,
    _s0, _s1, _s2, _s3,_s4, _s5, _s6, _s7,
    _t8, _t9
};

enum Type_J{
    _j = 0b000010,
    _jal = 0b000011,
    _jr = 0b001000
};

enum Type_I{
    _li  =  0b00100100,
    _mfhi = 0b00010000,
    _mflo = 0b00010010,

    _mult = 0b00011000,
};

enum Type_R{
    _add = 0b00000100000,
    _sub = 0b00000100010,
    _div = 0b00000011010,

    _and = 0b00000100100,
     _or = 0b00000100011,
    _xor = 0b00000100110,
    _not = 0b00000100111,

    _sll = 0b00000000000,
    _sllv = 0b00000000100,

    _srl = 0b00000000010,
    _srlv = 0b00000000110,
};

class PSPDynarec{

private:
    int MAX_SZ = 7 * 1024 * 512; //3.5MB
    __attribute__((aligned(64))) uint8_t * code;

public:
    uint64_t  count;

    void Init(){
        int ret = sceKernelVolatileMemLock(0, reinterpret_cast<void**>(&code), &MAX_SZ); 
        count = 0;

        memset(code,0,MAX_SZ);

        if (ret != 0) printf("Failed to allocate volatile mem ");
        else          printf("Volatile mem allocated correctly");
    }

    void   GetValueFromReg(REG_MIPS reg,uint64_t &value);

    void*       GetMemPoint(uint64_t addr){ return &code[addr]; }  
    uint64_t    GetMemPos(){ return count; } 
    void        SetMemPos(uint64_t pos){ count = pos; }   
};

class PSPD_Fun{
    private:
        PSPDynarec* _dynarec;
        uint64_t  memPosition;
        uint8_t* code;
        uint16_t codePointer;

        bool writePending = false;
        int instructionPending = -1;

        void EmitBYTE(u8 byte);
        void EmitWORD(u16 word);
        void EmitDWORD(u32 dword);

    public:
        //TODO Fix: This code will execute a nop as first operation. 
        PSPD_Fun(PSPDynarec *dynarec,uint64_t addr){
            memPosition =  addr;
            code = reinterpret_cast<uint8_t*>(dynarec->GetMemPoint(dynarec->count)); 
            codePointer = dynarec->GetMemPos()/8;
            _dynarec = dynarec;
        }

        void PrintInstrCount();
        void Finalize();
        void Execute();
        
        template<Type_J _op>                                     void OP_J(int addr);

        template<Type_I _op,REG_MIPS rs>                         void OP_I();
        template<Type_I _op,REG_MIPS rs, REG_MIPS rt>            void OP_I();
        template<Type_I _op,REG_MIPS rs>                         void OP_I(uint64_t value);
        template<Type_I _op,REG_MIPS rs, REG_MIPS rt>            void OP_I(uint64_t value);

        template<Type_R _op,REG_MIPS rs,REG_MIPS rd>             void OP_R(uint64_t value);
        template<Type_R _op,REG_MIPS rs,REG_MIPS rd,REG_MIPS rt> void OP_R(); 
};


template<Type_R _op,REG_MIPS rs,REG_MIPS rd,REG_MIPS rt> 
void PSPD_Fun::OP_R()
{
     u32 instruction = _op;
     instruction |= (int)rs<<11;
     instruction |= (int)rd<<21;
     instruction |= (int)rt<<16;
     if (instructionPending != -1) EmitDWORD(instructionPending); 
     instructionPending = instruction;   
}

template<Type_R _op,REG_MIPS rs,REG_MIPS rd> 
void PSPD_Fun::OP_R(uint64_t value)
{    
    u32 instruction = _op;
    instruction |= (int)rs<<11;
    instruction |= (int)rd<<16;
    instruction |= static_cast<uint8_t>(value<<21);
    if (instructionPending != -1) EmitDWORD(instructionPending); 
    instructionPending = instruction;
}

template<Type_I _op,REG_MIPS rs, REG_MIPS rd> 
void PSPD_Fun::OP_I(uint64_t value){
     u32 instruction = _op;
     instruction |= (int)rs<<21;
     instruction |= (int)rd<<16;
     EmitWORD(instruction);
     EmitWORD(static_cast<uint16_t>(value));
}

template<Type_I _op,REG_MIPS rs,REG_MIPS rd> 
void PSPD_Fun::OP_I(){
     u32 instruction = _op;
     instruction |= (int)rs<<21;
     instruction |= (int)rd<<16;
      if (instructionPending != -1) EmitDWORD(instructionPending); 
     instructionPending = instruction;
}

template<Type_I _op,REG_MIPS rs> 
void PSPD_Fun::OP_I(uint64_t value){
    EmitWORD(static_cast<uint16_t>(value));
    EmitBYTE(rs);
    EmitBYTE(_op);
}

template<Type_I _op,REG_MIPS rs> 
void PSPD_Fun::OP_I(){
    u32 instruction = _op;
    instruction |= (int)rs<<11;
    if (instructionPending != -1) EmitDWORD(instructionPending); 
    instructionPending = instruction;
}

template<Type_J _op> 
void PSPD_Fun::OP_J(int addr){
    u32 instruction = (addr>>2);
    instruction |= _op << 26;
    EmitDWORD(instruction);
    EmitDWORD(0); //Add a nop after the jump 0C900638
}                                              

#endif
