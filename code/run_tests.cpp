#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "mos6502.h"

internal u32
ConvertAddress(char* AddrStr)
{
   size_t Len = strlen(AddrStr);
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

inline u8
Read(mos6502* Chip, u16 Addr)
{
   u8 Result = Chip->Ram[Addr];
   return Result;
}

inline void 
Write(mos6502* Chip, u16 Addr, u8 Value)
{
   Chip->Ram[Addr] = Value;
}

int main(int ArgCount, char** Args)
{
   if (3 != ArgCount)
   {
      fprintf(stderr, "\nUsage: %s ROM_PATH LOAD_ADDRESS\n", Args[0]);
#if _DEBUG
	  __debugbreak();
#else
      exit(1);
#endif
   }

   char* TestRomPath = Args[1];
   u16 LoadAddress = ConvertAddress(Args[2]);

   FILE* File = fopen(TestRomPath, "rb");
   if (!File)
   {
      fprintf(stderr, "Unable to open test ROM path: %s\n", TestRomPath);
#if _DEBUG
	  __debugbreak();
#else
	  exit(1);
#endif
   }

   fseek(File, 0, SEEK_END);
   u32 Size = ftell(File);
   fseek(File, 0, SEEK_SET);

   u8* Rom = (u8*)malloc(Size);
   size_t ActualSize = fread(Rom, 1, Size, File);
   fclose(File);

   mos6502 Chip = {};
   Chip.Read = Read;
   Chip.Write = Write;
   Chip.Init(Rom, ActualSize, LoadAddress);

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
         Chip.Exec();

         fprintf(stdout, "State: {A: %02X, X: %02X, Y: %02X, P: %02X, S: %02X, PC: %04X}\n", 
            Chip.A, Chip.X, Chip.Y, Chip.P.O, Chip.S, Chip.PC);
      }
   }

   fprintf(stdout, "Execution complete.\n");

   return 0;
}