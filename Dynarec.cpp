#include <pspsuspend.h>
#include <pspkernel.h>

#include <stdio.h>
#include <cstring>

#include "Dynarec.h"

void PSPD_Fun::EmitBYTE(u8 byte)
{
    code[codePointer++] = byte;
}

void PSPD_Fun::EmitWORD(u16 word)
{
    *(u16 *)(&code[codePointer]) = word;
    codePointer += 2;
}

void PSPD_Fun::EmitDWORD(u32 dword)
{
    *(u32 *)(&code[codePointer]) = dword;
    codePointer += 4;
}

void PSPD_Fun::Finalize(){
    EmitDWORD(0x03e00008);
    if (instructionPending != -1) EmitDWORD(instructionPending); 
    _dynarec->count += codePointer+4;
}

void PSPD_Fun::Execute(){
	sceKernelDcacheWritebackInvalidateAll();
    asm volatile("jal %0": : "r" (code)); 
}


//DO NOT TEMPLATE THIS

void PSPDynarec::GetValueFromReg(REG_MIPS reg,uint64_t &value){
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