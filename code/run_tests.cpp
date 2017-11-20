#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define internal static

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;

typedef char i8;
typedef short i16;
typedef int i32;
typedef long i64;

typedef float f32;

#include "mos6502.h"

#define DEBUG

#ifdef DEBUG
#define Assert(X) if(!(X)){ fprintf(stderr, "Assertion failed! line: %d", __LINE__); fflush(stderr); *(int*)0=1;}
#else
#define Assert(X)
#endif

internal u32
ConvertAddress(char* AddrStr)
{
   u32 Len = strlen(AddrStr);
   if (Len > 2)
   {
      if (AddrStr[0] == '0' && (AddrStr[1] == 'X' || AddrStr[1] == 'x'))
      {
         u16 Value = 0;
         for (i32 Index=2; Index<Len; Index++)
         {
            Value = Value*16 + (AddrStr[Index] - '0');
         }
         return Value;
      }
   }
   return atoi(AddrStr);
}

internal char*
GetFileName(char* Path)
{   
   char* C = Path;
   char* LastSlash = Path;
   while(*C != 0)
   {
      if (*C == '\\')
      {
         LastSlash = C + 1; // Skip the slash
      }
	  C++;
   }
   return LastSlash;
}

int main(int ArgCount, char** Args)
{
   if (3 != ArgCount)
   {
      fprintf(stderr, "\nUsage: %s ROM_PATH LOAD_ADDRESS\n", Args[0]);
      exit(1);
   }

   char* TestRomPath = Args[1];
   u16 LoadAddress = ConvertAddress(Args[2]);

   FILE* File = fopen(TestRomPath, "rb");
   if (!File)
   {
      fprintf(stderr, "Unable to open test ROM path: %s\n", TestRomPath);
      exit(1);
   }

   fseek(File, 0, SEEK_END);
   u32 Size = ftell(File);
   fseek(File, 0, SEEK_SET);

   u8* Rom = (u8*)malloc(Size);
   u32 ActualSize = fread(Rom, 1, Size, File);
   fclose(File);

   mos6502 Chip = {};
   InitChip(&Chip, Rom, ActualSize, LoadAddress);

   char* Filename = GetFileName(TestRomPath);   
   if (strcmp(Filename, "basic.bin") == 0)
   {
#include "basic_tests.h"
   }
   else
   {      
      while (1)
      {
         fprintf(stdout, "\nAddr: %04X, Instr: %02X\n", Chip.PC, Chip.Ram[Chip.PC]); 
         u8 Opcode = Chip.Ram[Chip.PC++];
         ExecInstruction(&Chip, Opcode);

         fprintf(stdout, "State: {A: %02X, X: %02X, Y: %02X, P: %02X, S: %02X, PC: %04X}\n", 
            Chip.A, Chip.X, Chip.Y, Chip.P.V, Chip.S, Chip.PC);
      }
   }

   fprintf(stdout, "Execution complete.\n");

   return 0;
}