/* main.c - this file is part of DeSmuME
 *
 * Copyright (C) 2006-2015 DeSmuME Team
 * Copyright (C) 2007 Pascal Giard (evilynux)
 * Used under fair use by the DSonPSP team, 2019
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This file is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <psppower.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include <pspsuspend.h>
#include <pspkernel.h>
#include <psprtc.h>

#include "Dynarec.h"

PSP_MODULE_INFO("Dynarec Test", 0, 0, 1);

PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER | PSP_THREAD_ATTR_VFPU);
//PSP_HEAP_SIZE_KB(-256);

int Function(){
  printf("Ciao \n");
  return 0;
}

PSPDynarec _psp_dynarec;

void WriteLog(char* msg)
{
	FILE* fd;
	fd = fopen("debug_log.txt", "a");
	fprintf(fd, "%s\n", msg);
	fclose(fd);
}

int main(int argc, char **argv) { 
  //addr dynarec 08900590  
  uint64_t returnValue = 10;

  pspDebugScreenInitEx((void*)(0x44000000), PSP_DISPLAY_PIXEL_FORMAT_5551, 1);
 
  _psp_dynarec.Init();
  
  PSPD_Fun fun1(&_psp_dynarec,0);
  
  fun1.OP_I<_li,_a0>(100);
  fun1.OP_I<_li,_a1>(120);
  fun1.OP_I<_li,_a2>(2);
  fun1.OP_R<_add,_s3,_a0,_a1>();
  fun1.OP_I<_mult,_s3,_a2>(); 
  fun1.OP_I<_mflo,_s3>(); 

  fun1.Finalize();

  PSPD_Fun fun2(&_psp_dynarec,0);
  
  fun2.OP_I<_li,_a2>(2);
  fun2.OP_I<_mult,_s3,_a2>(); 
  fun2.OP_I<_mflo,_s3>(); 
  fun1.OP_I<_li,_a1>(100);
  fun2.OP_J<_jal>((int)&Function); 
  
  fun2.Finalize();
  
  fun1.Execute();
  fun2.Execute();

  _psp_dynarec.GetValueFromReg(_s3,returnValue);
  
  //printf("\nValue %lld\n",returnValue);
  pspDebugScreenPrintf("Value: %lld",returnValue);

  //_psp_dynarec.DeInit();

  return 0;
}
 