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

enum OPCodes{
    _sll = 0b00000100,
    _srl = 0b00000110,

    _mul = 0b0000010,
    _add = 0b00000100000,
    _sub = 0b00000100010,
    _div = 0b00000011010,

    _and = 0b00000100100,
     _or = 0b00000100011,
    _xor = 0b00000100110,
    _not = 0b00000100111,

      _j = 0b000010,
    _jal = 0b000011,
     _jr = 0b001000
};

struct ASM_MIPS{
        uint64_t  count;
       __attribute__((aligned(64))) uint8_t * code;
};

class PSPDynarec{

private:
    int MAX_SZ = 7 * 1024 * 512; //3.5MB
    ASM_MIPS mips_code;

public:
    PSPDynarec(){
        int ret = sceKernelVolatileMemLock(0, reinterpret_cast<void**>(&mips_code.code), &MAX_SZ); 
        mips_code.count = 0;

        memset(mips_code.code,0,MAX_SZ);

        if (ret != 0) printf("Failed to allocate volatile mem ");
        else          printf("Volatile mem allocated correctly");
    }

    void EmitBYTE(u8 byte);
    void EmitWORD(u16 word);
    void EmitDWORD(u32 dword);

    void ExecuteBlock();

    template<REG_MIPS reg> void  LoadValueToReg(uint64_t  value);
    void GetValueFromReg(REG_MIPS reg,uint64_t &value);

    template<OPCodes _op> void OP_R(REG_MIPS reg1 = _zero, REG_MIPS reg2 = _zero, REG_MIPS reg3 = _zero);
    template<OPCodes _op> void OP_I(REG_MIPS reg1 = _zero, REG_MIPS reg2 = _zero, uint64_t value = 0);
    template<OPCodes _op> void OP_J(uint64_t addr);

    void InsertLabel(char* label);
    void JumpToLabel(char* label);

};


template<OPCodes _op> 
void PSPDynarec::OP_R(REG_MIPS reg1,REG_MIPS reg2,REG_MIPS reg3)
{
     u32 instruction = _op;
     instruction |= (int)reg1<<11;
     instruction |= (int)reg2<<21;
     instruction |= (int)reg3<<16;
     EmitDWORD(instruction);     
}

template<OPCodes _op> 
void PSPDynarec::OP_I(REG_MIPS reg1, REG_MIPS reg2, uint64_t value){

}

template<OPCodes _op> 
void PSPDynarec::OP_J(uint64_t addr){

}

template<REG_MIPS reg>
void PSPDynarec::LoadValueToReg(uint64_t value){
    EmitWORD(static_cast<uint16_t>(value));
    mips_code.code[mips_code.count++] = reg;
    mips_code.code[mips_code.count++] = 0b100100;
}




#endif