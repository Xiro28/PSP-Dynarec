#include <pspsuspend.h>
#include <pspkernel.h>

#include <stdio.h>
#include <cstring>

#include "Dynarec.h"

void PSPDynarec::EmitBYTE(u8 byte)
{
    mips_code.code[mips_code.count++] = byte;
}

void PSPDynarec::EmitWORD(u16 word)
{
    *(u16 *)(&mips_code.code[mips_code.count]) = word;
    mips_code.count += 2;
}

void PSPDynarec::EmitDWORD(u32 dword)
{
    *(u32 *)(&mips_code.code[mips_code.count]) = dword;
    mips_code.count += 4;
}

u32 labelAddr = 0; 

void PSPDynarec::InsertLabel(char* label)
{
    labelAddr = (u32)&mips_code.code[mips_code.count];
}

void PSPDynarec::JumpToLabel(char* label)
{
    //printf("Label addr: %x\n",(u32)&mips_code.code[mips_code.count]);
    EmitWORD(labelAddr);
    mips_code.code[mips_code.count++] = 0;
    mips_code.code[mips_code.count++] = 0b1100;
}

void PSPDynarec::ExecuteBlock(){

    if(mips_code.count > 0)
	{
        //Finalize the block before the execution
        EmitDWORD(0x03e00008); //j $ra

		sceKernelDcacheWritebackInvalidateAll();
        asm volatile("jal %0": : "r" (mips_code.code)); 
	}
}

//DO NOT TEMPLATE THIS

void PSPDynarec::GetValueFromReg(REG_MIPS reg,uint64_t &value){
    unsigned int tmp;
    switch (reg){
        case _s0:
            register unsigned s0 asm ("s0"); 
            value = s0;
        break;
        case _s1:
            register unsigned s1 asm ("s1"); 
            value = s1;
        break;
        case _s2:
            register unsigned s2 asm ("s2"); 
            value = s2;
        break;
        case _s3:
            register unsigned s3 asm ("s3"); 
            value = s3;
        break;
        case _s4:
            register unsigned s4 asm ("s4"); 
            value = s4;
        break;
        case _s5:
            register unsigned s5 asm ("s5"); 
            value = s5;
        break;
        default:
        break;
    }
}