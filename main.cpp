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

  uint64_t returnValue = 0;
  
  _psp_dynarec.LoadValueToReg<_s1>(10);
  _psp_dynarec.LoadValueToReg<_s2>(15);
  _psp_dynarec.OP_R<_add>(_s5,_s1,_s2); 
  _psp_dynarec.ExecuteBlock();
 
  _psp_dynarec.GetValueFromReg(_s5,returnValue);
    
  printf("\nValue %lld\n",returnValue);

  return 0;
}
 