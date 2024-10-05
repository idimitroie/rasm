#include <stdio.h>
#include <malloc.h>

#include "storage.h"
#include "progc.h"
#include "main.h"
#include "decode.h"

void storage_add_instr(instr_t *opcode, unsigned curpc, unsigned curlineno)
{
  dmp_print("%02x %02x %02x %02x #\t %08x\tline %d\n",
     *(unsigned int*)opcode->data & 0xFF, 
     (*(unsigned int*)opcode->data >> 8) & 0xFF, 
     (*(unsigned int*)opcode->data >> 16) & 0xFF, 
     (*(unsigned int*)opcode->data >> 24) & 0xFF,
     curpc, curlineno
     );
    if (debug)
    {
		char *buff = 0;
        sprint_instr(&buff, opcode);
        dmp_print("%s\n", buff);
        free(buff);
    }
    free(opcode->data);
    free(opcode);
}

void storage_add_glob_label(char *lbl, unsigned curpc, unsigned curlineno)
{
  dmp_print("# label_globl \"%s\"\tpc = 0x%08x\tline %d\n", lbl, curpc, curlineno);
}

void storage_add_locl_label(unsigned lbl, unsigned curpc, unsigned curlineno)
{
dmp_print("# label_local \"%d\"\tpc = 0x%08x\tline %d\n", lbl, curpc, curlineno);
}

void storage_add_glob_identifier(char *lbl, unsigned curpc, unsigned ofs, unsigned absadr, unsigned curlineno)
{
  dmp_print("# identifier \"%s\" at 0x%08x offs %d absaddr 0x%08x\tline %d\n", lbl, curpc, ofs, absadr, curlineno);
}

void storage_add_locl_identifier(int lbl, unsigned curpc, unsigned ofs, unsigned absadr, unsigned curlineno)
{
  dmp_print("# local identifier \"%u\" at = %08x, ofs = %d, absadr = 0x%08X\tline %d\n", lbl, get_pc(), ofs, absadr, curlineno);
}
