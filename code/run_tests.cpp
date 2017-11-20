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

   fprintf(stdout, "Read %d bytes.\n", ActualSize);

   mos6502 Chip = {};
   InitChip(&Chip, Rom, ActualSize, LoadAddress);

   char* Filename = GetFileName(TestRomPath);   
   if (strcmp(Filename, "basic.bin") == 0)
   {
      //
      // Load immediate / Store absolute
      // 

      // LDA #$01  { A9 }
      u8 Op = Chip.Ram[Chip.PC++];
      ExecInstruction(&Chip, Op);	
      Assert(Chip.A == 1);
      // STA $0200
      Op = Chip.Ram[Chip.PC++];
      ExecInstruction(&Chip, Op);
      Assert(Chip.Ram[0x0200] == 1);
      // LDX #$02
      Op = Chip.Ram[Chip.PC++];
      ExecInstruction(&Chip, Op);
      Assert(Chip.X == 2);
      // STX $0201
      Op = Chip.Ram[Chip.PC++];
      ExecInstruction(&Chip, Op);
      Assert(Chip.Ram[0x0201] == 2);
      // LDY #$03
      Op = Chip.Ram[Chip.PC++];
      ExecInstruction(&Chip, Op);
      Assert(Chip.Y == 3);

      // STY $0202
      Op = Chip.Ram[Chip.PC++];
      ExecInstruction(&Chip, Op);
      Assert(Chip.Ram[0x0202] == 3);

      //
      // Store zero page / Load zero page
      //

      // STA $04
      Op = Chip.Ram[Chip.PC++];
      ExecInstruction(&Chip, Op);
      Assert(Chip.Ram[0x0004] == 1);

      // STX $05
      Op = Chip.Ram[Chip.PC++];
      ExecInstruction(&Chip, Op);
      Assert(Chip.Ram[0x0005] == 2);

      // STY $06
      Op = Chip.Ram[Chip.PC++];
      ExecInstruction(&Chip, Op);
      Assert(Chip.Ram[0x0006] == 3);

      // LDA $06
      Op = Chip.Ram[Chip.PC++];
      ExecInstruction(&Chip, Op);
      Assert(Chip.A == 3);

      // LDX $04
      Op = Chip.Ram[Chip.PC++];
      ExecInstruction(&Chip, Op);
      Assert(Chip.X == 1);

      // LDY $05
      Op = Chip.Ram[Chip.PC++];
      ExecInstruction(&Chip, Op);
      Assert(Chip.Y == 2);

      //
      // Reset the registers
      //
      Op = Chip.Ram[Chip.PC++];
      ExecInstruction(&Chip, Op);
      Op = Chip.Ram[Chip.PC++];
      ExecInstruction(&Chip, Op);
      Op = Chip.Ram[Chip.PC++];
      ExecInstruction(&Chip, Op);
      Assert(Chip.A == 0);
      Assert(Chip.X == 0);
      Assert(Chip.Y == 0);

      //
      // Load absolute
      //

      // LDA $0200
      Op = Chip.Ram[Chip.PC++];
      ExecInstruction(&Chip, Op);      
      Assert(Chip.A == 1);

      // LDX $0201
      Op = Chip.Ram[Chip.PC++];
      ExecInstruction(&Chip, Op);
      Assert(Chip.X == 2);

      // LDY $0202
      Op = Chip.Ram[Chip.PC++];
      ExecInstruction(&Chip, Op);
      Assert(Chip.Y == 3);

      // Just resetting here...
      // LDA #$0
      Op = Chip.Ram[Chip.PC++];
      ExecInstruction(&Chip, Op);
      Assert(Chip.A == 0);

      //
      // Load zero page X/Y
      //

      // LDA $02,X
      Op = Chip.Ram[Chip.PC++];
      ExecInstruction(&Chip, Op);
      Assert(Chip.A == 1);

      // LDX $01,Y
      Op = Chip.Ram[Chip.PC++];
      ExecInstruction(&Chip, Op);
      Assert(Chip.X == 1);

      // LDY $03,X
      Op = Chip.Ram[Chip.PC++];
      ExecInstruction(&Chip, Op);
      Assert(Chip.Y == 1);

      //
      // Store zero page X/Y
      //

      // STA $00,X
      Op = Chip.Ram[Chip.PC++];
      ExecInstruction(&Chip, Op);
      Assert(Chip.Ram[0x0001] == 1);
      // STX $00,Y
      Op = Chip.Ram[Chip.PC++];
      ExecInstruction(&Chip, Op);
      Assert(Chip.Ram[0x0002] == 1);
      // STY $00,X
      Op = Chip.Ram[Chip.PC++];
      ExecInstruction(&Chip, Op);
      Assert(Chip.Ram[0x0003] == 1);

      //
      // Load absolute X/Y
      //

      // LDA $0201,X
      Op = Chip.Ram[Chip.PC++];
      ExecInstruction(&Chip, Op);
      Assert(Chip.A == 3);
      // LDX $0201,Y
      Op = Chip.Ram[Chip.PC++];
      ExecInstruction(&Chip, Op);
      Assert(Chip.X == 3);
      // LDA $01FF,X
      Op = Chip.Ram[Chip.PC++];
      ExecInstruction(&Chip, Op);
      Assert(Chip.Y == 3);
      // LDA $0200,Y
      Op = Chip.Ram[Chip.PC++];
      ExecInstruction(&Chip, Op);
      Assert(Chip.A == 0);

      //
      // Setup for indirect ops
      //
      Op = Chip.Ram[Chip.PC++];
      ExecInstruction(&Chip, Op);
      Assert(Chip.A == 0xBE);
      Op = Chip.Ram[Chip.PC++];
      ExecInstruction(&Chip, Op);
      Assert(Chip.Ram[0x0302] == 0xBE);
      Op = Chip.Ram[Chip.PC++];
      ExecInstruction(&Chip, Op);
      Assert(Chip.A == 0xAD);
      Op = Chip.Ram[Chip.PC++];
      ExecInstruction(&Chip, Op);
      Assert(Chip.Ram[0x0204] == 0xAD);
      Op = Chip.Ram[Chip.PC++];
      ExecInstruction(&Chip, Op);
      Assert(Chip.A == 0x0);

      //
      // Load indirect/indexed X/Y
      //

      // LDA ($02,X)
      Op = Chip.Ram[Chip.PC++];
      ExecInstruction(&Chip, Op);
      Assert(Chip.A == 0xBE);

      // LDA ($04),Y
      Op = Chip.Ram[Chip.PC++];
      ExecInstruction(&Chip, Op);
      Assert(Chip.A == 0xAD);
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

   return 0;
}